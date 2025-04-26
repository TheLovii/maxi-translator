#include "syntax.h"

void Syntax::program() {
    // <program> ::= <statements> "EOF"
    statements();
    if (current_lex.first != "EOF") throw std::logic_error("Expected EOF");
}
void Syntax::statements() {
    // <statements> ::= (<statement>)+
    while (current_lex.first != "EOF" && current_lex.first != "}") {
        statement();
        endl();
    }
}
void Syntax::statement() {
    // <statement> ::= (<declaration> | <expression> | <if> | <while> | <for> | <return> | <break> | <continue> | <function> | <struct> | <print>) <endl>
    TIDNode *ptr = semantic.manage_tid().current_node();
    while (ptr->nodeType != NodeType::Global) ptr = ptr->parent;
    if (current_lex.second == 9 or (dynamic_cast<GlobalNode*>(ptr)->struct_exist(current_lex.first))) declaration();
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
    declaration_token(); // лагерь, каз,
}
void Syntax::declaration_token() {
    // <declaration_token>::=<type> <set_token> push_id()("," <set_token> push_id())*
    std::string token_type = current_lex.first;
    type();
    std::string token_name = current_lex.first;
    semantic.manage_tid().current_node()->push_id(token_name, token_type);
    set_token();
    while (current_lex.first == ",") {
        current_lex = lexical.get_lex();
        token_name = current_lex.first;
        semantic.manage_tid().current_node()->push_id(token_name, token_type);
        set_token();
    }
}
void Syntax::type() {
    // <type>::="int"|"float"|"string"|"char"|"bool"| <token>
    if (current_lex.second == 9) {
        current_lex = lexical.get_lex();
    } else if (current_lex.second == 3) {
        TIDNode* ptr = semantic.manage_tid().current_node();
        while(ptr->nodeType != NodeType::Global) ptr = ptr->parent;
        if (!dynamic_cast<GlobalNode*>(ptr)->struct_exist(current_lex.first)) throw std::logic_error("No such type: " + current_lex.first);
        token();
    } else throw std::logic_error("Excepted type");
}
void Syntax::set_token() {
    // <set_token> ::= <token> | <token> "=" <assigment_expression>
    std::string token_name = current_lex.first;
    token();
    if (current_lex.first == "=") {
        current_lex = lexical.get_lex();
        assigment_expression();
        semantic.manage_tid().check_var_type(token_name, semantic.get_first_stack());
        semantic.stack_clear();
    }
}
void Syntax::token() {
    // <token>::=<char>(<char>|<number>)*
    std::regex regex("[a-zA-Z_][a-zA-Z0-9_]*");
    if (!std::regex_match(current_lex.first, regex)) throw std::logic_error("Expected token");
    current_lex = lexical.get_lex();
}

void Syntax::if_statement() {
    // <if> ::= "if" "(" <expression> ")" tid::create_node() <body_start> <statements> <body_end> tid::delete_node() (<elif>)* ("else" tid::create_node() <body_start> <statements> <body_end> tid::delete_node())?
    if (current_lex.first != "if") throw std::logic_error("Expected if");
    current_lex = lexical.get_lex();
    if (current_lex.first != "(") throw std::logic_error("Expected (");
    current_lex = lexical.get_lex();
    expression();
    if (current_lex.first != ")") throw std::logic_error("Expected )");

    current_lex = lexical.get_lex();
    semantic.manage_tid().create_node(NodeType::Body);
    if (current_lex.first != "{") throw std::logic_error("Expected {");
    current_lex = lexical.get_lex();
    statements();
    if (current_lex.first != "}") throw std::logic_error("Expected }");
    semantic.manage_tid().delete_node();

    current_lex = lexical.get_lex();
    while (current_lex.first == "elif") {
        current_lex = lexical.get_lex();
        if (current_lex.first != "(") throw std::logic_error("Expected (");
        current_lex = lexical.get_lex();
        expression();
        if (current_lex.first != ")") throw std::logic_error("Expected )");

        current_lex = lexical.get_lex();
        semantic.manage_tid().create_node(NodeType::Body);
        if (current_lex.first != "{") throw std::logic_error("Expected {");
        current_lex = lexical.get_lex();
        statements();
        if (current_lex.first != "}") throw std::logic_error("Expected }");
        semantic.manage_tid().delete_node();

        current_lex = lexical.get_lex();
    }
    if (current_lex.first == "else") {
        current_lex = lexical.get_lex();
        semantic.manage_tid().create_node(NodeType::Body);
        if (current_lex.first != "{") throw std::logic_error("Expected {");
        current_lex = lexical.get_lex();
        statements();
        if (current_lex.first != "}") throw std::logic_error("Expected }");
        semantic.manage_tid().delete_node();

        current_lex = lexical.get_lex();
    }
}
void Syntax::while_statement() {
    // <while> ::= "while" "(" <expression> ")" tid::create_node() <body_start> <statements> <body_end> tid::delete_node()
    if (current_lex.first != "while") throw std::logic_error("Expected while");
    current_lex = lexical.get_lex();
    if (current_lex.first != "(") throw std::logic_error("Expected (");
    current_lex = lexical.get_lex();
    expression();
    if (current_lex.first != ")") throw std::logic_error("Expected )");

    current_lex = lexical.get_lex();
    semantic.manage_tid().create_node(NodeType::Body);
    if (current_lex.first != "{") throw std::logic_error("Expected {");
    current_lex = lexical.get_lex();
    statements();
    if (current_lex.first != "}") throw std::logic_error("Expected }");
    semantic.manage_tid().delete_node();

    current_lex = lexical.get_lex();
}
void Syntax::for_statement() {
    // <for> ::= "for" tid::create_node() "(" <declaration> ";" <expression> ";" <expression> ")" <body_start> <statements> <body_end> tid::delete_node()
    if (current_lex.first != "for") throw std::logic_error("Expected for");
    current_lex = lexical.get_lex();

    semantic.manage_tid().create_node(NodeType::Body);
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
    semantic.manage_tid().delete_node();

    current_lex = lexical.get_lex();
}
void Syntax::return_statement() {
    // <return> ::= "return" <assigment_expression>
    if (current_lex.first != "return") throw std::logic_error("Expected return");
    current_lex = lexical.get_lex();
    assigment_expression();
    dynamic_cast<FunctionNode*>(semantic.manage_tid().current_node())->check_return_type(semantic.get_first_stack());
    semantic.stack_clear();
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
    semantic.manage_tid().create_node(NodeType::Function);
    auto* node = dynamic_cast<FunctionNode*>(semantic.manage_tid().current_node());
    if (current_lex.first != "func") throw std::logic_error("Expected func");
    current_lex = lexical.get_lex();
    node->set_func_name(current_lex.first);
    token();
    if (current_lex.first != "(") throw std::logic_error("Expected (");

    current_lex = lexical.get_lex();
    arg_list();
    if (current_lex.first != ")") throw std::logic_error("Expected )");
    current_lex = lexical.get_lex();
    if (current_lex.first != "->") throw std::logic_error("Expected ->");
    current_lex = lexical.get_lex();
    node->set_return_type(current_lex.first);
    type();

    if (current_lex.first != "{") throw std::logic_error("Expected {");
    current_lex = lexical.get_lex();
    statements();
    if (current_lex.first != "}") throw std::logic_error("Expected }");
    current_lex = lexical.get_lex();
    semantic.manage_tid().delete_node();
}
void Syntax::arg_list() {
    // <arg_list>::= <type> <token>(","<type> <token>)* | EMPTY
    std::string token_type, token_name;
    auto* node = dynamic_cast<FunctionNode*>(semantic.manage_tid().current_node());
    if (current_lex.second == 9 || current_lex.second == 3) {
        token_type = current_lex.first;
        type();
        token_name = current_lex.first;
        token();
        node->push_param(token_type);
        node->push_id(token_name, token_type);
        while (current_lex.first == ",") {
            current_lex = lexical.get_lex();
            token_type = current_lex.first;
            type();
            token_name = current_lex.first;
            token();
            node->push_param(token_type);
            node->push_id(token_name, token_type);
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
    semantic.manage_tid().create_node(NodeType::Struct);
    if (current_lex.first != "struct") throw std::logic_error("Expected struct");
    current_lex = lexical.get_lex();
    dynamic_cast<StructNode*>(semantic.manage_tid().current_node())->set_struct_name(current_lex.first);
    token();
    if (current_lex.first != "{") throw std::logic_error("Expected {");
    current_lex = lexical.get_lex();
    struct_statements();
    if (current_lex.first != "}") throw std::logic_error("Expected }");
    current_lex = lexical.get_lex();
    semantic.manage_tid().delete_node();
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
    // <struct_statement> ::= (<access_type> <declaration_token> | <access_type> <function>) <endl>
    if (current_lex.first == "public" || current_lex.first == "private") {
        std::string access = current_lex.first;
        access_type();
        if (current_lex.first == "func") {
            function_statement();
        } else if (current_lex.second == 9) {
            declaration_token();
        } else throw std::logic_error("Excepted statement");
        dynamic_cast<StructNode*>(semantic.manage_tid().current_node())->manage_access(access);
    } else if (current_lex.first == "}") {
        return;
    } else throw std::logic_error("Excepted statement");
}
void Syntax::access_type() {
    // <access_type> ::= "public" | "private"
    if (current_lex.first != "public" && current_lex.first != "private") throw std::logic_error("Expected public or private");
    current_lex = lexical.get_lex();
}

void Syntax::print_statement() {
    // <print> ::= "print" "(" <expression> ")" ";"
    if (current_lex.first != "print") throw std::logic_error("Expected print");
    current_lex = lexical.get_lex();
    call_arg_list();
}

void Syntax::expression(bool end_clear_stack) {
    // <expression> ::= <assigment_expression> | <assigment_expression>","<expression> stack_clear()
    assigment_expression();
    if (current_lex.first == ",") {
        current_lex = lexical.get_lex();
        expression();
    }
    if (end_clear_stack) semantic.stack_clear();
}
void Syntax::assigment_expression() {
    // <assigment_expression> ::= <logical_or> <assign_operator> <assigment_expression> | <logical_or>
    logical_or();
    if (current_lex.second == 4) {
        semantic.push_op(current_lex.first);
        current_lex = lexical.get_lex();
        assigment_expression();
        semantic.check_bin();
    }
}
void Syntax::logical_or() {
    // <logical_or> ::= <logical_and> "||" push_type_op() <logical_or> check_bin() | <logical_and>
    logical_and();
    if (current_lex.first == "||") {
        semantic.push_op("||");
        current_lex = lexical.get_lex();
        logical_or();
        semantic.check_bin();
    }
}
void Syntax::logical_and() {
    // <logical_and> ::= <bit_or> "&&" push_type_op() <logical_and> check_bin() | <bit_or>
    bit_or();
    if (current_lex.first == "&&") {
        semantic.push_op("&&");
        current_lex = lexical.get_lex();
        logical_and();
        semantic.check_bin();
    }
}

void Syntax::bit_or() {
    // <bit_or> ::= <bit_xor> "|" push_type_op() <bit_or> check_bin() | <bit_xor>
    bit_xor();
    if (current_lex.first == "|") {
        semantic.push_op("|");
        current_lex = lexical.get_lex();
        bit_or();
        semantic.check_bin();
    }
}
void Syntax::bit_xor() {
    // <bit_xor> ::= <bit_and> "^" push_type_op() <bit_xor> check_bin() | <bit_and>
    bit_and();
    if (current_lex.first == "^") {
        semantic.push_op("^");
        current_lex = lexical.get_lex();
        bit_xor();
        semantic.check_bin();
    }
}
void Syntax::bit_and() {
    // <bit_and> ::= <equal_comparison> "&" push_type_op() <bit_and> check_bin() | <equal_comparison>
    equal_comparison();
    if (current_lex.first == "&") {
        semantic.push_op("&");
        current_lex = lexical.get_lex();
        bit_and();
        semantic.check_bin();
    }
}
void Syntax::equal_comparison() {
    // <equal_comparison> ::= <compare> ("==" | "!=") push_type_op() <equal_comparison> check_bin() | <compare>
    compare();
    if (current_lex.first == "==" || current_lex.first == "!=") {
        semantic.push_op(current_lex.first);
        current_lex = lexical.get_lex();
        equal_comparison();
        semantic.check_bin();
    }
}
void Syntax::compare() {
    // <compare> ::= <shift> (">" | ">=" | "<" | "<=") push_type_op() <compare> check_bin() | <shift>
    shift();
    if (current_lex.first == "<" || current_lex.first == ">" || current_lex.first == "<=" || current_lex.first == ">=") {
        semantic.push_op(current_lex.first);
        current_lex = lexical.get_lex();
        compare();
        semantic.check_bin();
    }
}
void Syntax::shift() {
    // <shift> ::= <addition> (">>" | "<<") push_type_op() <shift> check_bin() | <addition>
    addition();
    if (current_lex.first == "<<" || current_lex.first == ">>") {
        semantic.push_op(current_lex.first);
        current_lex = lexical.get_lex();
        shift();
        semantic.check_bin();
    }
}
void Syntax::addition() {
    // <addition> ::= <multiplication> <sign> push_type_op() <addition> check_bin() | <multiplication>
    multiplication();
    if (current_lex.first == "+" || current_lex.first == "-") {
        semantic.push_op(current_lex.first);
        current_lex = lexical.get_lex();
        addition();
        semantic.check_bin();
    }
}
void Syntax::multiplication() {
    // <multiplication> ::= <power> ("/" | "*" | "%") push_type_op() <multiplication> check_bin() | <power>
    power();
    if (current_lex.first == "*" || current_lex.first == "/" || current_lex.first == "%") {
        semantic.push_op(current_lex.first);
        current_lex = lexical.get_lex();
        multiplication();
        semantic.check_bin();
    }
}
void Syntax::power() {
    // <power> ::= <unary> "**" push_type_op() <power> check_bin() | <unary>
    unary();
    if (current_lex.first == "**") {
        semantic.push_op("**");
        current_lex = lexical.get_lex();
        power();
        semantic.check_bin();
    }
}
void Syntax::unary() {
    // <unary> ::= ("+" | "-" | "~" | "!" | "++" | "--") push_type_op() <unary> check_uno() | <postfix>
    if (current_lex.first == "+" || current_lex.first == "-" || current_lex.first == "~" || current_lex.first == "!" ||
        current_lex.first == "++" || current_lex.first == "--") {
        semantic.push_op(current_lex.first);
        current_lex = lexical.get_lex();
        unary();
        semantic.check_uno();
    } else postfix();
}
void Syntax::postfix() {
    // <postfix> ::= <point> ("++" | "--") push_type_op() check_postfix() | <point>
    point();
    if (current_lex.first == "++" || current_lex.first == "--") {
        semantic.push_op(current_lex.first);
        current_lex = lexical.get_lex();
        semantic.check_postfix();
    }
}
void Syntax::point() {
    // <point> ::= <token_exp> "." push_type_op() <point> check_bin() | <token_exp>
    token_exp();
    if (current_lex.first == ".") {
        semantic.push_op(".");
        current_lex = lexical.get_lex();
        point();
        semantic.check_bin();
    }
    else if (current_lex.first == "(") {
        current_lex = lexical.get_lex();
        semantic.push_op("(");
        call_arg_list();
        if (current_lex.first != ")") throw std::logic_error("Expected )");
        semantic.push_op(")");
        semantic.check_call_function();
    }
}
void Syntax::token_exp() {
    // <token_exp> ::= <token> push_type_op() | <literal> push_type_op() "(" <expression> ")" | "(" <expression> ")"
    if (('0' <= current_lex.first[0] && current_lex.first[0] <= '9')  || current_lex.first == "true" ||
        current_lex.first == "false" || current_lex.second == 11) {
        if (current_lex.second == 11) semantic.push_type("string");
        else if (current_lex.first == "true" || current_lex.first == "false") semantic.push_type("bool");
        else if (current_lex.first.find('.') != std::string::npos) semantic.push_type("float");
        else semantic.push_type("int");
        literal();

        if (current_lex.first == "(") {
            current_lex = lexical.get_lex();
            expression();
            if (current_lex.first != ")") throw std::logic_error("Expected )");
            current_lex = lexical.get_lex();
        }
    } else if (current_lex.second == 3) {
        std::string token_name = current_lex.first;
        token();
        if (semantic.manage_tid().token_exist(token_name))
            semantic.push_type(semantic.manage_tid().get_token_type(token_name));
        else
            semantic.push_name(token_name);
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
    if (current_lex.second == 3 || current_lex.second == 11 || current_lex.first == "true" || current_lex.first == "false") {
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
