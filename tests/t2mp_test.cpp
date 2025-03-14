#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../include/t2mp.hpp"

using namespace t2mp;

TEST_CASE("ExpectTypeState valid") {
    T2MP t2mp;
    auto initial_state = t2mp.get_state();
    t2mp.process("int");

    // no exception expected, all OK

    auto sz = t2mp.curr_ptr.data.real_sz;

    REQUIRE(sz == sizeof(int));
    CHECK(initial_state != t2mp.get_state());
}

TEST_CASE("ExpectTypeState invalid type") {
    T2MP t2mp;

    CHECK_THROWS_AS(t2mp.process("UNSUPPORTED_TYPE"), t2mp::unsupported_type_error);
}

TEST_CASE("ExpectNameState valid") {
    T2MP t2mp;
    t2mp.update_state(std::make_shared<ExpectNameState>(&t2mp));
    auto initial_state = t2mp.get_state();

    std::vector<std::string> tokens = {"*", "*", "variable_name"}; // 2 stars
    t2mp.process(tokens, false);

    auto rank = t2mp.curr_ptr.data.rank;
    auto name = t2mp.curr_ptr.name;
    CHECK(initial_state != t2mp.get_state());

    REQUIRE(rank == 2);
    REQUIRE(name == "variable_name");
}

TEST_CASE("ExpectAssigmentState valid") {
    T2MP t2mp;
    t2mp.update_state(std::make_shared<ExpectAssigmentState>(&t2mp));
    auto initial_state = t2mp.get_state();

    // no exception expected, all OK

    t2mp.process("=");

    CHECK(initial_state != t2mp.get_state());
}

TEST_CASE("ExpectAssigmentState invalid") {
    T2MP t2mp;
    t2mp.update_state(std::make_shared<ExpectAssigmentState>(&t2mp));

    CHECK_THROWS_AS(t2mp.process("&"), t2mp::omitted_assignment_error);
}

TEST_CASE("ExpectValueState non-pointer immediate processing") {
    T2MP t2mp;
    t2mp.update_state(std::make_shared<ExpectValueState>(&t2mp));
    auto initial_state = t2mp.get_state();

    std::vector<std::string> tokens = {"-777"};
    t2mp.process(tokens, false);
    
    CHECK(initial_state != t2mp.get_state());
    REQUIRE((int16_t)t2mp.curr_ptr.data.value == (int16_t)-777);
}

/*
TEST_CASE("ExpectValueState pointer processing") {
    T2MP t2mp;
    t2mp.update_state(std::make_shared<ExpectValueState>(&t2mp));
    auto initial_state = t2mp.get_state();
}
*/

TEST_CASE("ExpectValueState invalid: double addr taking") {
    T2MP t2mp;
    t2mp.update_state(std::make_shared<ExpectValueState>(&t2mp));
    t2mp.process("&");

    CHECK_THROWS_AS(t2mp.process("&"), t2mp::unsupported_address_capture_error);
}

TEST_CASE("ExpectSemicolonState valid") {
    T2MP t2mp;
    t2mp.update_state(std::make_shared<ExpectSemicolonState>(&t2mp));
    auto initial_state = t2mp.get_state();
    size_t initial_memsize = t2mp.get_memory().size(), rsz = 1, val = 0x07;
    t2mp.curr_ptr.data.rank = 0, t2mp.curr_ptr.data.real_sz = rsz, t2mp.curr_ptr.data.value = val;
    t2mp.process(";");

    REQUIRE(initial_state != t2mp.get_state());
    // происходит aligning
    auto diff = t2mp.get_memory().size()-initial_memsize;

    REQUIRE(diff == rsz);
    REQUIRE(t2mp.get_memory()[0].byte_val == val);
    REQUIRE(t2mp.get_memory()[0].is_ptr == false);

}

TEST_CASE("ExpectSemicolonState invalid") {
    T2MP t2mp;
    t2mp.update_state(std::make_shared<ExpectSemicolonState>(&t2mp));

    CHECK_THROWS_AS(t2mp.process("NOT_SEMICOLON"), t2mp::omitted_end_of_statement_error);
}


TEST_CASE("Complex test: non-pointer") {
    T2MP t2mp; // mem padding = 1
    std::vector<std::string> tokens = {"int", "a", "=", "5", ";"};

    t2mp.process(tokens);

    auto memory = t2mp.get_memory();
    size_t it{};
    while(memory[it].vname == "") ++it; // skipping alignment

    for(size_t i{};i < sizeof(int);++i) {
        REQUIRE(memory[it+i].vname == "a");
        REQUIRE(memory[it+i].is_ptr == false);

        if(i == sizeof(int)-1)                      // check MSB order
            REQUIRE(memory[it+i].byte_val == 0x05);
        else 
            REQUIRE(memory[it+i].byte_val == 0x00);
    }
}


TEST_CASE("Complex test: pointer") {
    T2MP t2mp; // mem padding = 1
    std::vector<std::string> tokens = {"int", "a", "=", "5", ";", 
                                       "int", "*", "b", "=", "&", "a", ";"};

    t2mp.process(tokens);

    auto memory = t2mp.get_memory();
    size_t it{};
    while(memory[it].vname == "") ++it; // skipping alignment
    auto a_addr = memory[it].addr;
    it += sizeof(int); // skipping a variable

    address_t b_value{};
    for(size_t i{};i < sizeof(t2mp::address_t);++i) {
        REQUIRE(memory[it+i].vname == "b");
        REQUIRE(memory[it+i].is_ptr == true);
        b_value = (b_value << 8) | memory[it+i].byte_val;
    }

    REQUIRE(b_value == a_addr);
}
