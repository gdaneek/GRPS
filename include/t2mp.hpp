#pragma once

#include <cstdlib>
#include <ctime>
#include <memory>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include "t2mp_excepts.hpp"

/**
 * functions and classes of a FSM 
 * that converts a sequence of string tokens into a memory structure (memory part)
 */
namespace t2mp {

    class State {
    public:
        virtual ~State() = default;
        virtual void process(const std::string& token) = 0;
    };

    using address_t = uint32_t; //<  data type that should be considered as a memory address

    /**
     * implementation of a finite state machine
     * T2MP - Tokens to Memory Part
     */
    class T2MP {
        // using address_t = uint32_t;
    protected:

        /**
         * a unit of data that a FSM converts into a memory structure
         * contains all the data specific to a pointer of any rank
         * variables can also be interpreted as pointers, but in the degenerate case (rank == 0)
         * so, there is a special field <real_sz> for them that shows how many bytes are actually used
         */
        struct Pointer {

            mutable std::string name;

            struct Data {
                uint8_t rank : 4;
                uint8_t real_sz : 4;
                address_t addr;
                uint64_t value;
            } data;

            Pointer() = default;
            Pointer(const std::pair<const decltype(name), Data> p)
            : name{p.first}, data{p.second} {}

            operator std::pair<const decltype(name), Data>() const {
                return std::make_pair(name, data);
            }
        };

        /**
         * A unit for building a MemoryPart
         */
        struct MemoryByte {
            address_t addr;
            uint8_t byte_val;
            std::string vname;
            bool is_ptr;

            friend std::ostream& operator<<(std::ostream& os, const MemoryByte& mb) {
                os << mb.addr << " " << (uint16_t)mb.byte_val << " " << mb.vname << " " << mb.is_ptr;
                return os;
            }
        };


        /**
         * a part of the memory showing how the transferred tokens are stored
         */
        class MemoryPart : public std::vector<MemoryByte> {

            address_t start_addr;
        public:

            MemoryPart() : std::vector<MemoryByte>{} {
                std::srand(std::time(0));
                start_addr = 0x7ff000000000 + std::rand() % (long)1e6;
            }

            address_t align_addr(uint8_t mod) {
                while(start_addr % mod)
                    this->push_back(MemoryByte{start_addr++, (uint8_t)std::rand(), "", false});

                return start_addr;
            }

            template<typename T>
            address_t push_bytes(std::string& name, T value, uint8_t bytes_num, bool is_ptr = false) {
                auto binded_addr {align_addr(bytes_num)};
                for(auto i{0};i < bytes_num; ++i)   // MSB order
                    this->push_back(MemoryByte{start_addr++, (uint8_t)(value >> ((bytes_num-i-1) << 3)), name, is_ptr});

                return binded_addr;
            }

        };

    private:

        std::shared_ptr<State> m_state;                                         //< current state
        std::unordered_map<decltype(Pointer::name), Pointer::Data> pointers;    //< table with variables/pointers
        MemoryPart m_memory;                                                    //< memory structure

    public:

        Pointer curr_ptr;

        T2MP() : m_memory{}, pointers{}, curr_ptr{} {
            set_entry_state();
        }


        /**
        * a function that allows the last state to find
        * the initial state without knowing the implementing class.
        */
        void set_entry_state();

        void update_state(std::shared_ptr<State> new_state) {
            m_state = new_state;
        }

        std::shared_ptr<State> get_state() const {
            return m_state;
        }

        void update_memory() {
            bool is_ptr = (curr_ptr.data.rank)? true : false;
            auto value = curr_ptr.data.value;
            auto real_sz =  is_ptr? sizeof(address_t) : curr_ptr.data.real_sz;
            curr_ptr.data.addr = m_memory.push_bytes(curr_ptr.name, value, real_sz, is_ptr);
            pointers[curr_ptr.name] = curr_ptr.data;
            curr_ptr = Pointer();
        }

        void reset() {
            m_memory.clear();
            pointers.clear();
            set_entry_state();
            curr_ptr = Pointer();
        }

        const MemoryPart& get_memory() {
            return m_memory;
        }

        Pointer::Data get_ptr_by_name(const decltype(Pointer::name) expected_name) const {
            return pointers.at(expected_name);
        }

        Pointer::Data get_ptr_by_addr(const decltype(Pointer::Data::addr) expected_addr) const {
            auto eq_addr_cond {[expected_addr](Pointer ptr){return ptr.data.addr == expected_addr;}};
            auto it = std::find_if(pointers.begin(), pointers.end(), eq_addr_cond);
            if(it == pointers.end())
                throw undeclared_variable_error{std::to_string(expected_addr)};

            return Pointer(*it).data;
        }

        void process(const std::string& token) {
            m_state->process(token);
        }

        const MemoryPart& process(const std::vector<std::string>& tokens, bool need_reset = true) {
            if(need_reset) {
                reset();
                m_memory.reserve(tokens.size());
            }

            for(auto& token : tokens)
                process(token);

            return get_memory();
        }

    }; // T2MP

    /**
     * Concrete states represented from the final state to the initial one
    */

    /**
     * waiting for the end of a statement
     * if successful, the memory structure is updated
     */
    class ExpectSemicolonState : public State {
        T2MP* fsm;
    public:
        ExpectSemicolonState(T2MP* fsm_) : fsm{fsm_} {}
        void process(const std::string& token) override {
            if(token != ";")
                throw omitted_end_of_statement_error{token};

            fsm->update_memory();
            fsm->set_entry_state();
        }
    };


    /**
     * processing the value of a variable.
     * dereference of any depth is allowed,
     * but taking the address is supported only for expressions like &value.
     * setting an immediate value is allowed only for non-pointer values
     */
    class ExpectValueState : public State {
        T2MP* fsm;
        int64_t depth_of_deref {}; //< depth of dereference
    public:
        ExpectValueState(T2MP* fsm_) : fsm{fsm_} {}
        void process(const std::string& token) override {

            if(token == "*") {
                depth_of_deref++;
                return;
            }

            else if (token == "&") {
                if(depth_of_deref--)
                    throw unsupported_address_capture_error{};
                return;
            }

            // immediate processing (rvalue) for non-pointers

            if(!fsm->curr_ptr.data.rank) {
                auto rv = std::stoi(token); // get integral value
                fsm->curr_ptr.data.value = *reinterpret_cast<decltype(fsm->curr_ptr.data.value)*>(&rv);  // append raw bits
                fsm->update_state(std::make_shared<ExpectSemicolonState>(fsm));
                return;
            }

            // other cases (lvalue)

            auto rhs = fsm->get_ptr_by_name(token);

            if((rhs.rank - depth_of_deref) != fsm->curr_ptr.data.rank)
                throw bad_assigment_error{fsm->curr_ptr.name, token};

            if(depth_of_deref == -1)         // address taking
                fsm->curr_ptr.data.value = rhs.addr;

            else {
                auto curr_val = rhs.value;
                while(depth_of_deref--)
                    curr_val = fsm->get_ptr_by_addr(curr_val).value;

                fsm->curr_ptr.data.value = curr_val;
            }

            depth_of_deref = 0;

            fsm->update_state(std::make_shared<ExpectSemicolonState>(fsm));
        }

    };


    /**
     * waiting for a C-style assignment
     */
    class ExpectAssigmentState : public State {
        T2MP* fsm;
    public:
        ExpectAssigmentState(T2MP* fsm_) : fsm{fsm_} {}
        void process(const std::string& token) override {
            if(token != "=")
                throw omitted_assignment_error{token};

            fsm->update_state(std::make_shared<ExpectValueState>(fsm));
        }

    };

    /**
     * waiting for the variable name. 
     * Since the "*" symbol does not belong to the type (int* a = &b, c = &b not valid for c), 
     * it is processed in this state
     */
    class ExpectNameState : public State {
        T2MP* fsm;
    public:
        ExpectNameState(T2MP* fsm_) : fsm{fsm_} {}
        void process(const std::string& token) override {
            if(token == "*") {
                fsm->curr_ptr.data.rank++;
                return;
            }
            //name validation is possible here, but in general it does not affect anything
            fsm->curr_ptr.name = token;
            fsm->update_state(std::make_shared<ExpectAssigmentState>(fsm));
        }
    };

    /**
     * waiting for a supported type
     * the types are listed in a static table `supported_types`
     */
    class ExpectTypeState : public State {
        T2MP* fsm;
        // need constexpr dict
        inline static std::unordered_map<std::string, uint8_t> supported_types = {{"int", sizeof(int)}};
    public:
        ExpectTypeState(T2MP* fsm_) : fsm{fsm_} {}
        void process(const std::string& token) override {
            if(!supported_types.count(token))
                throw unsupported_type_error{token};

            fsm->curr_ptr.data.real_sz = supported_types[token];
            fsm->update_state(std::make_shared<ExpectNameState>(fsm));
        }
    };


    inline void T2MP::set_entry_state() {
        m_state = std::make_shared<ExpectTypeState>(this);
    }

}; // namespace t2mp
