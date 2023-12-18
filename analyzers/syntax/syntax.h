#ifndef MAXI_TRANSLATOR_SYNTAX_H
#define MAXI_TRANSLATOR_SYNTAX_H
#include "../../utils/debug.h"
#include "../lexical/lexical.h"
/*
 * TODO: проблемы с грамматикой: generic_token кривой
 * TODO: нет перегрузки операторов
 * TODO: property работают через жопу
 * TODO: доработать работу с Line
 * TODO: впринципе подумать над фичами и добавить что-то
 * TODO: разобраться с EOF
 * TODO: исправить грамматику в соответствии синтаксическому анализатору
 */
class Syntax {
public:
    explicit Syntax(const std::string& file_name) : lexical(file_name), debug("syntax") {
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
            std::cout << "Drop error. See debug_syntax.txt\n";
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
    void generic_token();
    void initial_list();

    void if_statement();
    void while_statement();
    void for_statement();
    void return_statement();
    void break_statement();
    void continue_statement();
    void function_statement();
    void arg_list();

    void class_s();
    void class_statements();
    void class_statement();
    void access_type();
    void class_property();

    void print_statement();

    void expression();
    void assigment_expression();
    void init_expression();
    void init_list();
    void init_list_seq();
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
    std::pair<std::string, int> current_lex;
};
#endif //MAXI_TRANSLATOR_SYNTAX_H
