#include "semantic.h"
void Semantic::check_bin() {
    std::string type1 = stack.top();
    stack.pop();
    std::string op = stack.top();
    stack.pop();
    std::string type2 = stack.top();
    stack.pop();

    if (op == ".") {
        TIDNode* tmp = manage_tid().current_node();
        while(tmp->nodeType != NodeType::Global) tmp = tmp->parent;
        auto* ptr = dynamic_cast<GlobalNode*>(manage_tid().current_node());
        if (ptr->struct_exist(type2)) {
            if (ptr->field_exist(type2, type1))
                push_type(ptr->get_field_type(type2, type1));
            else if (ptr->method_name_exist(type2, type1))
                push_type(type2 + "." + type1);
            else {
                debug.log("Error: no such field or method: " + type1 + " in struct: " + type2);
                throw std::logic_error("Error: no such field or method: " + type1 + " in struct: " + type2);
            }
        }
        else {
            debug.log("Error: no such struct");
            throw std::logic_error("Error: no such struct");
        }
        return;
    }
    if (type1 == "float" || type2 == "float") {
        if ((type1 == "float" and (type2 != "float" or type2 != "int")) or (type2 == "float" and (type1 != "float" or type1 != "int"))) {
            debug.log("Error: different types in binary operation");
            throw std::logic_error("Error: different types in binary operation");
        } else push_type("float");
        return;
    }
    if (type1 != type2) {
        debug.log("Error: different types in binary operation");
        throw std::logic_error("Error: different types in binary operation");
    }
    push_type(type1);
}
void Semantic::check_uno() {
    std::string type = stack.top();
    stack.pop();
    std::string op = stack.top();
    stack.pop();

    if (type != "int" && type != "float") {
        debug.log("Error: unary operation with non-int and non-float type");
        throw std::logic_error("Error: unary operation with non-int and non-float type");
    }
    stack.push(type);
}
void Semantic::check_postfix() {
    std::string op = stack.top();
    stack.pop();
    std::string type = stack.top();
    stack.pop();

    if (type != "int" && type != "float") {
        debug.log("Error: postfix operation with non-int and non-float type");
        throw std::logic_error("Error: postfix operation with non-int and non-float type");
    }
    stack.push(type);
}
void Semantic::check_call_function() {
    stack.pop(); // popping ")"
    std::vector<std::string> func_types;
    while(!stack.empty() and stack.top() != "(") {
        func_types.push_back(stack.top());
        stack.pop();
    }
    if (stack.empty() or stack.top() != "(") {
        debug.log("Call function not closed");
        throw std::logic_error("Call function not closed");
    }
    stack.pop();
    std::reverse(func_types.begin(), func_types.end());

    std::string function_name = stack.top();
    stack.pop();
    if (function_name.find('.') != std::string::npos) {
        std::string struct_name = function_name.substr(0, function_name.find('.'));
        std::string func_name = function_name.substr(function_name.find('.') + 1);
        TIDNode* ptr = manage_tid().current_node();
        while(ptr->nodeType != NodeType::Global) ptr = ptr->parent;
        push_type(dynamic_cast<GlobalNode*>(ptr)->get_method_type(struct_name, func_name, func_types));
    }
    else
        push_type(manage_tid().get_func_return_type(function_name, func_types));
}
void Semantic::stack_clear() {
    while (!stack.empty()) {
        stack.pop();
    }
}