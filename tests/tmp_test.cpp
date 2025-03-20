#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../include/tmp.hpp"

using namespace tmp;

TEST_CASE("ExpectTypeState valid") {
    TMP tmp;
    auto initial_state = tmp.get_state();
    tmp.process("int");

    // no exception expected, all OK

    auto sz = tmp.curr_ptr.data.real_sz;

    REQUIRE(sz == sizeof(int));
    CHECK(initial_state != tmp.get_state());
}

TEST_CASE("ExpectTypeState invalid type") {
    TMP tmp;

    CHECK_THROWS_AS(tmp.process("UNSUPPORTED_TYPE"), tmp::unsupported_type_error);
}

TEST_CASE("ExpectNameState valid") {
    TMP tmp;
    tmp.update_state(std::make_shared<ExpectNameState>(&tmp));
    auto initial_state = tmp.get_state();

    std::vector<std::string> tokens = {"*", "*", "variable_name"}; // 2 stars
    tmp.process(tokens, false);

    auto rank = tmp.curr_ptr.data.rank;
    auto name = tmp.curr_ptr.name;
    CHECK(initial_state != tmp.get_state());

    REQUIRE(rank == 2);
    REQUIRE(name == "variable_name");
}

TEST_CASE("ExpectAssigmentState valid") {
    TMP tmp;
    tmp.update_state(std::make_shared<ExpectAssigmentState>(&tmp));
    auto initial_state = tmp.get_state();

    // no exception expected, all OK

    tmp.process("=");

    CHECK(initial_state != tmp.get_state());
}

TEST_CASE("ExpectAssigmentState invalid") {
    TMP tmp;
    tmp.update_state(std::make_shared<ExpectAssigmentState>(&tmp));

    CHECK_THROWS_AS(tmp.process("&"), tmp::omitted_assignment_error);
}

TEST_CASE("ExpectValueState non-pointer immediate processing") {
    TMP tmp;
    tmp.update_state(std::make_shared<ExpectValueState>(&tmp));
    auto initial_state = tmp.get_state();

    std::vector<std::string> tokens = {"-777"};
    tmp.process(tokens, false);
    
    CHECK(initial_state != tmp.get_state());
    REQUIRE((int16_t)tmp.curr_ptr.data.value == (int16_t)-777);
}

/*
TEST_CASE("ExpectValueState pointer processing") {
    TMP tmp;
    tmp.update_state(std::make_shared<ExpectValueState>(&tmp));
    auto initial_state = tmp.get_state();
}
*/

TEST_CASE("ExpectValueState invalid: double addr taking") {
    TMP tmp;
    tmp.update_state(std::make_shared<ExpectValueState>(&tmp));
    tmp.process("&");

    CHECK_THROWS_AS(tmp.process("&"), tmp::unsupported_address_capture_error);
}

TEST_CASE("ExpectSemicolonState valid") {
    TMP tmp;
    tmp.update_state(std::make_shared<ExpectSemicolonState>(&tmp));
    auto initial_state = tmp.get_state();
    size_t initial_memsize = tmp.get_memory().size(), rsz = 1, val = 0x07;
    tmp.curr_ptr.data.rank = 0, tmp.curr_ptr.data.real_sz = rsz, tmp.curr_ptr.data.value = val;
    tmp.process(";");

    REQUIRE(initial_state != tmp.get_state());
    // происходит aligning
    auto diff = tmp.get_memory().size()-initial_memsize;

    REQUIRE(diff == rsz);
    REQUIRE(tmp.get_memory()[0].value == val);
    REQUIRE(tmp.get_memory()[0].is_pointer == false);

}

TEST_CASE("ExpectSemicolonState invalid") {
    TMP tmp;
    tmp.update_state(std::make_shared<ExpectSemicolonState>(&tmp));

    CHECK_THROWS_AS(tmp.process("NOT_SEMICOLON"), tmp::omitted_end_of_statement_error);
}


TEST_CASE("Complex test: non-pointer") {
    TMP tmp; // mem padding = 1
    std::vector<std::string> tokens = {"int", "a", "=", "5", ";"};

    tmp.process(tokens);

    auto memory = tmp.get_memory();
    size_t it{};
    while(memory[it].name == "") ++it; // skipping alignment

    for(size_t i{};i < sizeof(int);++i) {
        REQUIRE(memory[it+i].name == "a");
        REQUIRE(memory[it+i].is_pointer == false);

        if(i == sizeof(int)-1)                      // check MSB order
            REQUIRE(memory[it+i].value == 0x05);
        else 
            REQUIRE(memory[it+i].value == 0x00);
    }
}


TEST_CASE("Complex test: pointer") {
    TMP tmp; // mem padding = 1
    std::vector<std::string> tokens = {"int", "a", "=", "5", ";", 
                                       "int", "*", "b", "=", "&", "a", ";"};

    tmp.process(tokens);

    auto memory = tmp.get_memory();
    size_t it{};
    while(memory[it].name == "") ++it; // skipping alignment
    auto a_addr = memory[it].addr;
    it += sizeof(int); // skipping a variable

    address_t b_value{};
    for(size_t i{};i < sizeof(address_t);++i) {
        REQUIRE(memory[it+i].name == "b");
        REQUIRE(memory[it+i].is_pointer == true);
        b_value = (b_value << 8) | memory[it+i].value;
    }

    REQUIRE(b_value == a_addr);
}
