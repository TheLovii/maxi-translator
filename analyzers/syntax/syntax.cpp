#include "syntax.h"

void Syntax::program() {
    // <program> ::= <preprocessor> <statements> "EOF"
    preprocessor();
    statements();
    if (current_lex.first != "EOF") throw std::logic_error("Expected EOF");
}
void Syntax::preprocessor() {
    // <preprocessor> ::= ("import"<token> <endl>)*
    while (current_lex.first == "import") {
        current_lex = lexical.get_lex();
        if (current_lex.second == 11) string();
        else token();
        endl();
    }
}
void Syntax::statements() {
    // <statements> ::= (<statement>)+
    while (current_lex.first != "EOF" && current_lex.first != "}") {
        statement();
        endl();
    }
}
void Syntax::statement() {
    // <statement> ::= (<declaration>  | <expression> | <if> | <while> | <for> | <return> | <break> | <continue> | <function> | <struct> | <print>) <endl>
    if (current_lex.second == 9) declaration();
    else if (current_lex.first == "if") if_statement();
    else if (current_lex.first == "while") while_statement();
    else if (current_lex.first == "for") for_statement();
    else if (current_lex.first == "return") return_statement();
    else if (current_lex.first == "break") break_statement();
    else if (current_lex.first == "continue") continue_statement();
    else if (current_lex.first == "func") function_statement();
    else if (current_lex.first == "struct") struct_s();
    else if (current_lex.first == "print") print_statement();
    else expression();
}

void Syntax::declaration() {
    // <declaration> ::= <declaration_token>
    declaration_token();
}
void Syntax::declaration_token() {
    // <declaration_token>::=<type> <set_token>("," <set_token>)*
    type();
    set_token();
    while (current_lex.first == ",") {
        current_lex = lexical.get_lex();
        set_token();
    }
}
void Syntax::type() {
    // <type>::="int"|"float"|"string"|"char"|"bool"| "List""<"<type>">" | "Set""<"<type>">" | <token>
    if (current_lex.second == 9) {
        if (current_lex.first == "List" || current_lex.first == "Set") {
            current_lex = lexical.get_lex();
            if (current_lex.first != "<") throw std::logic_error("Excepted <");
            current_lex = lexical.get_lex();
            type();
            if (current_lex.first != ">") throw std::logic_error("Excepted >");
            current_lex = lexical.get_lex();
        } else current_lex = lexical.get_lex();
    } else if (current_lex.second == 3) {
        token();
    } else throw std::logic_error("Excepted type");
}
void Syntax::set_token() {
    // <set_token> ::= <expression>
    expression();
}
void Syntax::token() {
    // <token>::=<char>(<char>|<number>)*
    std::regex regex("[a-zA-Z_][a-zA-Z0-9_]*");
    if (!std::regex_match(current_lex.first, regex)) throw std::logic_error("Expected token");
    current_lex = lexical.get_lex();
}

void Syntax::if_statement() {
    // <if> ::= "if" "(" <expression> ")" <body_start> <statements> <body_end> (<elif>)* ("else" <body_start> <statements> <body_end>)?
    if (current_lex.first != "if") throw std::logic_error("Expected if");
    current_lex = lexical.get_lex();
    if (current_lex.first != "(") throw std::logic_error("Expected (");
    current_lex = lexical.get_lex();
    expression();
    if (current_lex.first != ")") throw std::logic_error("Expected )");
    current_lex = lexical.get_lex();
    if (current_lex.first != "{") throw std::logic_error("Expected {");
    current_lex = lexical.get_lex();
    statements();
    if (current_lex.first != "}") throw std::logic_error("Expected }");
    current_lex = lexical.get_lex();
    while (current_lex.first == "elif") {
        current_lex = lexical.get_lex();
        if (current_lex.first != "(") throw std::logic_error("Expected (");
        current_lex = lexical.get_lex();
        expression();
        if (current_lex.first != ")") throw std::logic_error("Expected )");
        current_lex = lexical.get_lex();
        if (current_lex.first != "{") throw std::logic_error("Expected {");
        current_lex = lexical.get_lex();
        statements();
        if (current_lex.first != "}") throw std::logic_error("Expected }");
        current_lex = lexical.get_lex();
    }
    if (current_lex.first == "else") {
        current_lex = lexical.get_lex();
        if (current_lex.first != "{") throw std::logic_error("Expected {");
        current_lex = lexical.get_lex();
        statements();
        if (current_lex.first != "}") throw std::logic_error("Expected }");
        current_lex = lexical.get_lex();
    }
}
void Syntax::while_statement() {
    // <while> ::= "while" "(" <expression> ")" <body_start> <statements> <body_end>
    if (current_lex.first != "while") throw std::logic_error("Expected while");
    current_lex = lexical.get_lex();
    if (current_lex.first != "(") throw std::logic_error("Expected (");
    current_lex = lexical.get_lex();
    expression();
    if (current_lex.first != ")") throw std::logic_error("Expected )");
    current_lex = lexical.get_lex();
    if (current_lex.first != "{") throw std::logic_error("Expected {");
    current_lex = lexical.get_lex();
    statements();
    if (current_lex.first != "}") throw std::logic_error("Expected }");
    current_lex = lexical.get_lex();
}
void Syntax::for_statement() {
    // <for> ::= "for" "(" <expression> ";" <expression> ";" <expression> ")" <body_start> <statements> <body_end>
    if (current_lex.first != "for") throw std::logic_error("Expected for");
    current_lex = lexical.get_lex();

    if (current_lex.first != "(") throw std::logic_error("Expected (");
    current_lex = lexical.get_lex();
    declaration();
    endl();
    expression();
    endl();
    expression();
    if (current_lex.first != ")") throw std::logic_error("Expected )");
    current_lex = lexical.get_lex();
    if (current_lex.first != "{") throw std::logic_error("Expected {");
    current_lex = lexical.get_lex();
    statements();
    if (current_lex.first != "}") throw std::logic_error("Expected }");
    current_lex = lexical.get_lex();
}
void Syntax::return_statement() {
    // <return> ::= "return" <expression>
    if (current_lex.first != "return") throw std::logic_error("Expected return");
    current_lex = lexical.get_lex();
    expression();
}
void Syntax::break_statement() {
    // <break> ::= "break"
    if (current_lex.first != "break") throw std::logic_error("Expected break");
    current_lex = lexical.get_lex();
}
void Syntax::continue_statement() {
    // <continue> ::= "continue"
    if (current_lex.first != "continue") throw std::logic_error("Expected continue");
    current_lex = lexical.get_lex();
}

void Syntax::function_statement() {
    // <function> ::= "func" <token> "(" <arg_list> ")" "->" <type> <body_start> <statements> <body_end>
    if (current_lex.first != "func") throw std::logic_error("Expected func");
    current_lex = lexical.get_lex();
    token();
    if (current_lex.first != "(") throw std::logic_error("Expected (");
    current_lex = lexical.get_lex();
    arg_list();
    if (current_lex.first != ")") throw std::logic_error("Expected )");
    current_lex = lexical.get_lex();
    if (current_lex.first != "->") throw std::logic_error("Expected ->");
    current_lex = lexical.get_lex();
    type();
    if (current_lex.first != "{") throw std::logic_error("Expected {");
    current_lex = lexical.get_lex();
    statements();
    if (current_lex.first != "}") throw std::logic_error("Expected }");
    current_lex = lexical.get_lex();
}
void Syntax::arg_list() {
    // <arg_list>::= <type> <token>(","<type> <token>)* | EMPTY
    if (current_lex.second == 9 || current_lex.second == 3) {
        type();
        token();
        while (current_lex.first == ",") {
            current_lex = lexical.get_lex();
            type();
            token();
        }
    }
}
void Syntax::call_arg_list() {
    // <call_arg_list> ::= <assigment_expression>(","<assigment_expression>)* | EMPTY
    if (current_lex.first != "(") throw std::logic_error("Expected (");
    current_lex = lexical.get_lex();
    if (current_lex.first == ")") {
        current_lex = lexical.get_lex();
        return;
    }
    assigment_expression();
    while (current_lex.first == ",") {
        current_lex = lexical.get_lex();
        assigment_expression();
    }
    if (current_lex.first != ")") throw std::logic_error("Expected )");
    current_lex = lexical.get_lex();
}

void Syntax::struct_s() {
    // <struct_s> ::= "struct" <token> <body_start> <struct_statements> <body_end>
    if (current_lex.first != "struct") throw std::logic_error("Expected struct");
    current_lex = lexical.get_lex();
    token();
    if (current_lex.first != "{") throw std::logic_error("Expected {");
    current_lex = lexical.get_lex();
    struct_statements();
    if (current_lex.first != "}") throw std::logic_error("Expected }");
    current_lex = lexical.get_lex();
}
void Syntax::struct_statements() {
    // <struct_statements> ::= <struct_statement> (";" <struct_statement>)*
    struct_statement();
    while (current_lex.first == ";") {
        current_lex = lexical.get_lex();
        struct_statement();
    }
}
void Syntax::struct_statement() {
    // <struct_statement> ::= (<access_type> <declaration_token> | <access_type> <function> | <struct_property>) <endl>
    if (current_lex.first == "public" || current_lex.first == "private") {
        access_type();
        if (current_lex.first == "func") {
            function_statement();
        } else if (current_lex.second == 9) {
            declaration_token();
        } else throw std::logic_error("Excepted statement");
    } else if (current_lex.first == "property") {
        struct_property();
    } else if (current_lex.first == "}") {
        return;
    } else throw std::logic_error("Excepted statement");
}
void Syntax::access_type() {
    // <access_type> ::= "public" | "private"
    if (current_lex.first != "public" && current_lex.first != "private") throw std::logic_error("Expected public or private");
    current_lex = lexical.get_lex();
}
void Syntax::struct_property() {
    // <struct_property> ::= "property" <type> <token>
    if (current_lex.first != "property") throw std::logic_error("Expected property");
    current_lex = lexical.get_lex();
    type();
    token();
}

void Syntax::print_statement() {
    // <print> ::= "print" "(" <expression> ")" ";"
    if (current_lex.first != "print") throw std::logic_error("Expected print");
    current_lex = lexical.get_lex();
    call_arg_list();
}

void Syntax::expression() {
    // <expression> ::= <assigment_expression> | <assigment_expression>","<expression>
    assigment_expression();
    if (current_lex.first == ",") {
        current_lex = lexical.get_lex();
        expression();
    }
}
void Syntax::assigment_expression() {
    // <assigment_expression> ::= <logical_or> <assign_operator> <init_expression> | <logical_or>
    logical_or();
    if (current_lex.second == 4) {
        current_lex = lexical.get_lex();
        init_expression();
    }
}
void Syntax::init_expression() {
    // <init_expression> ::= <assigment_expression> | <init_list>
    if (current_lex.first == "{") {
        init_list();
    } else {
        assigment_expression();
    }
}
void Syntax::init_list() {
    // <init_list> ::= <body_start> <init_list_seq> <body_end>
    if (current_lex.first != "{") throw std::logic_error("Expected {");
    current_lex = lexical.get_lex();
    init_list_seq();
    // c.check_push_type_operator("List");
    if (current_lex.first != "}") throw std::logic_error("Expected }");
    current_lex = lexical.get_lex();
}
void Syntax::init_list_seq() {
    // <init_list_seq> ::= <expression> ("," <expression>)*
    assigment_expression();
    while (current_lex.first == ",") {
        current_lex = lexical.get_lex();
        assigment_expression();
    }
}
void Syntax::logical_or() {
    // <logical_or> ::= <logical_and> "||" <logical_or> | <logical_and>
    logical_and();
    if (current_lex.first == "||") {
        current_lex = lexical.get_lex();
        logical_or();
    }
}
void Syntax::logical_and() {
    // <logical_and> ::= <bit_or> "&&" <logical_and> | <bit_or>
    bit_or();
    if (current_lex.first == "&&") {
        current_lex = lexical.get_lex();
        logical_and();
    }
}

void Syntax::bit_or() {
    // <bit_or> ::= <bit_xor> "|" <bit_or> | <bit_xor>
    bit_xor();
    if (current_lex.first == "|") {
        current_lex = lexical.get_lex();
        bit_or();
    }
}
void Syntax::bit_xor() {
    // <bit_xor> ::= <bit_and> "^" <bit_xor> | <bit_and>
    bit_and();
    if (current_lex.first == "^") {
        current_lex = lexical.get_lex();
        bit_xor();
    }
}
void Syntax::bit_and() {
    // <bit_and> ::= <equal_comparison> "&" <bit_and> | <equal_comparison>
    equal_comparison();
    if (current_lex.first == "&") {
        current_lex = lexical.get_lex();
        bit_and();
    }
}
void Syntax::equal_comparison() {
    // <equal_comparison> ::= <compare> ("==" | "!=") <equal_comparison> | <compare>
    compare();
    if (current_lex.first == "==" || current_lex.first == "!=") {
        current_lex = lexical.get_lex();
        equal_comparison();
    }
}
void Syntax::compare() {
    // <compare> ::= <shift> ("<" | ">" | "<=" | ">=") <compare> | <shift>
    shift();
    if (current_lex.first == "<" || current_lex.first == ">" || current_lex.first == "<=" || current_lex.first == ">=") {
        current_lex = lexical.get_lex();
        compare();
    }
}
void Syntax::shift() {
    // <shift> ::= <addition> ("<<" | ">>") <shift> | <addition>
    addition();
    if (current_lex.first == "<<" || current_lex.first == ">>") {
        current_lex = lexical.get_lex();
        shift();
    }
}
void Syntax::addition() {
    // <addition> ::= <multiplication> ("+" | "-") <addition> | <multiplication>
    multiplication();
    if (current_lex.first == "+" || current_lex.first == "-") {
        current_lex = lexical.get_lex();
        addition();
    }
}
void Syntax::multiplication() {
    // <multiplication> ::= <unary> ("*" | "/" | "%") <multiplication> | <unary>
    power();
    if (current_lex.first == "*" || current_lex.first == "/" || current_lex.first == "%") {
        current_lex = lexical.get_lex();
        multiplication();
    }
}
void Syntax::power() {
    // <power> ::= <unary> "**" <power> | <unary>
    unary();
    if (current_lex.first == "**") {
        current_lex = lexical.get_lex();
        power();
    }
}
void Syntax::unary() {
    // <unary> ::= ("+" | "-" | "~" | "!" | "++" | "--") <unary> | <postfix>
    if (current_lex.first == "+" || current_lex.first == "-" || current_lex.first == "~" || current_lex.first == "!" ||
        current_lex.first == "++" || current_lex.first == "--") {
        current_lex = lexical.get_lex();
        unary();
    } else postfix();
}
void Syntax::postfix() {
    // <postfix> ::= <token_exp> ("++" | "--") | <token_exp>
    point();
    if (current_lex.first == "++" || current_lex.first == "--") {
        current_lex = lexical.get_lex();
    }
}
void Syntax::point() {
    // <point> ::= <token_exp> "." | <token_exp>
    namespace_expression();
    if (current_lex.first == ".") {
        current_lex = lexical.get_lex();
        point();
    }
}
void Syntax::namespace_expression() {
    // <namespace> ::= <token_exp> "::" | <token_exp>
    token_exp();
    if (current_lex.first == "::") {
        current_lex = lexical.get_lex();
        namespace_expression();
    }
}
void Syntax::token_exp() {
    // <token_exp> ::= <token> | <literal> | <expression> ")"
    if (('0' <= current_lex.first[0] && current_lex.first[0] <= '9')  || current_lex.first == "true" ||
        current_lex.first == "false" || current_lex.second == 11) {
        literal();
        if (current_lex.first == "(") {
            current_lex = lexical.get_lex();
            expression();
            if (current_lex.first != ")") throw std::logic_error("Expected )");
            current_lex = lexical.get_lex();
        }
    } else if (current_lex.second == 3) {
        token();
        if (current_lex.first == "(") {
            call_arg_list();
        }
    } else if (current_lex.first == "(") {
        current_lex = lexical.get_lex();
        expression();
        if (current_lex.first != ")") throw std::logic_error("Expected )");
        current_lex = lexical.get_lex();
    } else
        throw std::logic_error("Expected token or literal or (");
}

void Syntax::literal() {
    // <literal>::=<number>|<string>|<boolean>
    if (current_lex.second == 3 || current_lex.second == 11) {
        if (current_lex.first == "true" || current_lex.first == "false") boolean();
        else if (current_lex.second == 11)
            string();
        else number();
    } else throw std::logic_error("Expected literal");
}
void Syntax::boolean() {
    // <boolean>::="true" | "false"
    if (current_lex.first != "true" && current_lex.first != "false") throw std::logic_error("Expected boolean");
    current_lex = lexical.get_lex();
}
void Syntax::string() {
    // <string>::="\""<char>*"\""
    if (current_lex.second != 11) throw std::logic_error("Expected \"");
    current_lex = lexical.get_lex();
    while (current_lex.second != 11) {
        current_lex = lexical.get_lex();
    }
    current_lex = lexical.get_lex();
}
void Syntax::number() {
    // <number>::=<digit>+"."<digit>*|<digit>+"."|<digit>+
    digit();
    while (current_lex.second == 3) {
        digit();
    }
    if (current_lex.first == ".") {
        current_lex = lexical.get_lex();
        while (current_lex.second == 3) {
            digit();
        }
    }
}
void Syntax::digit() {
    for (char &s : current_lex.first) if (s < '0' || s > '9') throw std::logic_error("Expected digit");
    current_lex = lexical.get_lex();
}
void Syntax::endl() {
    if (current_lex.first != ";") throw std::logic_error("Expected endl");
    current_lex = lexical.get_lex();
}
