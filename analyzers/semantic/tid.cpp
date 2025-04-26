#include "tid.h"
#include <stdexcept>

void TIDNode::push_id(const string& id, const string& type) {
    if (var_table.find(id) != var_table.end())
        throw std::logic_error("Variable already exist: " + id);
    var_table[id] = type;
}
const string& TIDNode::get_var_type(const string& id) {
    if (!var_exist(id))
        throw std::logic_error("Variable not exist: " + id);
    return var_table[id];
}


void GlobalNode::push_func_id(const string& id, const string& return_type, const func_params& params) {
    if (func_exist(id, params))
        throw std::logic_error("Function already exist: " + id);
    func_table[{ id, params }] = return_type;
}
const string& GlobalNode::get_func_type(const string& id, const func_params& params) {
    if (!func_exist(id, params))
        throw std::logic_error("No such function: " + id);
    return func_table[{id, params}];
}
void GlobalNode::push_struct_id(const string& id, const map<string, string>& vars, const map<pair<string, func_params>, string>& funcs, const map<string, string>& access) {
    if (struct_exist(id)) {
        throw std::logic_error("Struct already exist: " + id);
    }
    std::map<std::pair<std::string, std::string>, std::string> vars_with_access;
    for (const auto& [var_id, var_type] : vars) {
        vars_with_access[{ var_id, access.at(var_id) }] = var_type;
    }
    std::map<std::pair<std::pair<std::string, std::string>, func_params>, std::string> funcs_with_access;
    for (const auto& [func_id, func_type] : funcs) {
        funcs_with_access[{ { func_id.first, access.at(func_id.first) }, func_id.second }] = func_type;
    }
    struct_table[id] = { vars_with_access, funcs_with_access };
}
bool GlobalNode::method_name_exist(const string& struct_id, const string& func_id) {
    if (!struct_exist(struct_id)) throw std::logic_error("Struct not exist: " + struct_id);
    for (const auto& [func, _] : struct_table[struct_id].second) {
        if (func.first.first == func_id) return true;
    }
    return false;
}
const string& GlobalNode::get_field_type(const string& struct_id, const string& var_id) {
    if (!struct_exist(struct_id)) throw std::logic_error("Struct not exist: " + struct_id);
    if (struct_table[struct_id].first.count({var_id, "public"}) == 0)
        throw std::logic_error("No such field:" + struct_id);
    return struct_table[struct_id].first[{var_id, "public"}];
}
const string& GlobalNode::get_method_type(const string& struct_id ,const string& func_id, const func_params& params) {
    if (!struct_exist(struct_id)) throw std::logic_error("Struct not exist: " + struct_id);
    if (struct_table[struct_id].second.count({{func_id, "public"}, params}) == 0)
        throw std::logic_error("No such method: " + struct_id);
    return struct_table[struct_id].second[{ {func_id, "public"}, params }];
}


void StructNode::push_id(const std::string &id, const std::string &type) {
    if (var_exist(id))
        throw std::logic_error("Field already exist: " + id);
    fields[id] = type;
    last_token_name_ = id;
}
const string& StructNode::get_var_type(const string& id) {
    if (!var_exist(id))
        throw std::logic_error("No such field: " + id);
    return fields[id];
}
void StructNode::push_method(const string& id, const string& return_type, const func_params& params) {
    if (methods.count({ id, params }) != 0) {
        throw std::logic_error("Method already exist: " + id);
    }
    methods[{ id, params }] = return_type;
    last_token_name_ = id;
}

[[maybe_unused]] const string& StructNode::get_func_id(const string& id, const func_params& params) {
    if (!func_exist(id, params))
        throw std::logic_error("No such function: " + id);
    return methods[{ id, params }];
}
void StructNode::manage_access(const string& entrance) {
    if (entrance == "public" || entrance == "private") {
        access[last_token_name_] = entrance;
    } else {
        throw std::logic_error("No such access: " + entrance);
    }
}


void TID::create_node(int x) {
    if (x == NodeType::Function) {
        TIDNode* nw = new FunctionNode(current);
        current = nw;
        return;
    }
    if (x == NodeType::Struct) {
        TIDNode* nw = new StructNode(current);
        current = nw;
        return;
    }
    if (x == NodeType::Body) {
        TIDNode* nw = new BodyNode(current);
        current = nw;
        return;
    }
    if (x == NodeType::Global) {
        TIDNode* nw = new GlobalNode();
        current = nw;
        return;
    }
    throw std::logic_error("No such node type");
}
void TID::delete_node() {
    TIDNode* new_curr = current->parent;
    delete current;
    current = new_curr;
}
bool TID::token_exist(TIDNode *cur, const std::string &id) {
    if (cur == nullptr) return false;
    if (cur->var_exist(id)) return true;
    return token_exist(cur->parent, id);
}
void TID::check_var_type(TIDNode *cur, const std::string &id, const std::string &type) {
    if (cur == nullptr) throw std::logic_error("No such token: " + id);
    if (cur->var_exist(id)) cur->check_var_type(id, type);
    else check_var_type(cur->parent, id, type);
}
const string& TID::get_token_type(TIDNode* cur, const string& id) {
    if (cur == nullptr) throw std::logic_error("No such token: " + id);

    if (cur->var_exist(id)) return cur->get_var_type(id);
    return get_token_type(cur->parent, id);
}
const string& TID::get_func_return_type(TIDNode* cur, const string& id, const func_params& params) {
    if (cur == nullptr) throw std::logic_error("No such function: " + id);

    if (cur->func_exist(id, params)) return cur->get_func_type(id, params);
    return get_func_return_type(cur->parent, id, params);
}
