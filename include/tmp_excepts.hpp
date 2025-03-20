#pragma once

#include <stdexcept>
#include <memory>

namespace tmp {

    /**
    * An error caused when a FSM encounters an unsupported data type
    * may occur in ExpectTypeState class
    */
    struct unsupported_type_error : public std::runtime_error {
        unsupported_type_error(const std::string& type)
        : std::runtime_error("Type " + type + " is unsupported\n") {}
    };

    /**
    * An error caused when the FSM does not encounter an assignment token
    * may occur in ExpectAssigmentState class
    */
    struct omitted_assignment_error : public std::runtime_error {
        omitted_assignment_error(const std::string& wrong_token)
        : std::runtime_error("expected `=` , but got " + wrong_token + "\n") {}
    };


    /**
    * An error caused when the FSM does not encounter an end of statement
    * may occur in ExpectSemicolonState class
    */
    struct omitted_end_of_statement_error : public std::runtime_error {
        omitted_end_of_statement_error(const std::string& wrong_token)
        : std::runtime_error("expected `;` , but got " + wrong_token + "\n") {}
    };


    /**
    * An error caused when the FSM encounters an expression other than &variable_name
    * may occur in ExpectValueState class
    */
    struct unsupported_address_capture_error : public std::runtime_error {
        unsupported_address_capture_error()
        : std::runtime_error("FSM supports taking an address only from an lvalue variable") {}
    };


    /**
    * An error caused when the variables are incompatible by type
    * may occur in ExpectValueState class
    */
    struct bad_assigment_error : public std::runtime_error {
        bad_assigment_error(const std::string& lhs, const std::string& rhs)
        : std::runtime_error("it is not possible to assign " + lhs + " to " + rhs +" with the specified modifiers") {}
    };

    /**
    * the error caused if the FSM encounters the lvalue variable for the first time
    * may occur in ExpectValueState class
    */
    struct undeclared_variable_error : public std::runtime_error {
        undeclared_variable_error(const std::string& var)
        : std::runtime_error("The FSM has not previously encountered the variable " + var) {}
    };
}; // namespace tmp
