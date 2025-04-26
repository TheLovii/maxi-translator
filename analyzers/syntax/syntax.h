#ifndef MAXI_TRANSLATOR_SYNTAX_H
#define MAXI_TRANSLATOR_SYNTAX_H
#include "../lexical/lexical.h"
#include "../semantic/semantic.h"
#include "../../utils/debug.h"
/*
 * TODO: проблемы с грамматикой: generic_token кривой
 * TODO: исправить грамматику в соответствии синтаксическому анализатору
 */
class Syntax {
public:
    explicit Syntax(const std::string& file_name) : lexical(file_name), debug("syntax"), semantic() {
        debug.log("Success create syntax analyzer");
    }
    Syntax(const Syntax&) = delete;
    ~Syntax() = default;

    void analyze() {
        current_lex = lexical.get_lex();
        try{
            program();
        } catch (std::logic_error &e) {
            debug.log(e.what());
            std::cout << "Drop error: " << e.what() << std::endl << "See debug files.\n" << std::endl;
        }
        debug.log("Success analyze");
    }

    void program();
    void preprocessor();
    void statements();
    void statement();

    void declaration();
    void type();
    void declaration_token();
    void set_token();
    void token();

    void if_statement();
    void while_statement();
    void for_statement();
    void return_statement();
    void break_statement();
    void continue_statement();
    void function_statement();
    void arg_list();
    void call_arg_list();

    void struct_s();
    void struct_statements();
    void struct_statement();
    void access_type();

    void print_statement();

    void expression(bool end_clear_stack = true);
    void assigment_expression();
    void logical_or();
    void logical_and();

    void bit_or();
    void bit_xor();
    void bit_and();
    void equal_comparison();
    void compare();
    void shift();
    void addition();
    void multiplication();
    void power();
    void unary();
    void postfix();
    void point();
    void token_exp();

    void literal();
    void boolean();
    void string();
    void number();
    void digit();
    void endl();
private:
    Debug debug;
    Lexical lexical;
    Semantic semantic;
    std::pair<std::string, int> current_lex;
};
#endif //MAXI_TRANSLATOR_SYNTAX_H
