#ifndef MAXI_TRANSLATOR_SEMANTIC_H
#define MAXI_TRANSLATOR_SEMANTIC_H
#include "tid.h"
#include "../../utils/debug.h"
#include <vector>
#include <string>
#include <map>
#include <stack>

class Semantic {
public:
    Semantic() : debug("semantic"), global_() {
        debug.log("Success create semantic analyzer");
    }
    Semantic(const Semantic&) = delete;
    ~Semantic() = default;

    void push_type(const std::string &type) {stack.push(type);}
    void push_name(const std::string &name) {stack.push(name);}
    void push_op(const std::string &op) {stack.push(op);}
    const string& get_first_stack() {return stack.top();}

    void check_bin();
    void check_uno();
    void check_postfix();
    void check_call_function();
    void stack_clear();

    TID& manage_tid() {return this->global_;}
private:
    Debug debug;
    TID global_;
    std::stack<std::string> stack;
};
#endif //MAXI_TRANSLATOR_SEMANTIC_H