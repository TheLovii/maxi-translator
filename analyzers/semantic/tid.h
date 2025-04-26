#include <map>
#include <string>
#include <vector>

// [id, type]
using func_params = std::vector<std::string>;
// [vars, funcs] with access
using struct_fields =
        std::pair<
            std::map<std::pair<std::string, std::string>, std::string>,
            std::map<std::pair<std::pair<std::string, std::string>, func_params>, std::string>
        >;

using std::string;
using std::vector;
using std::map;
using std::pair;
enum NodeType { Global, Function, Struct, Body };

class TIDNode {
public:
    TIDNode* parent;
    NodeType nodeType;
    TIDNode() = delete;
    TIDNode(TIDNode* parent, NodeType type) : parent(parent), nodeType(type) {}
    virtual ~TIDNode() = default;

    virtual bool var_exist(const string& id) { return var_table.find(id) != var_table.end(); }
    virtual void push_id(const string& id, const string& type);
    virtual const string& get_var_type(const string& id);
    virtual void check_var_type(const string& id, const string& type) { if (get_var_type(id) != type) throw std::logic_error("Type mismatch"); }

    virtual bool func_exist(const string& id, const func_params& params) { return false; }
    virtual const string& get_func_type(const string& id, const func_params& params) { throw std::logic_error("Not implemented"); }

    virtual bool struct_exist(const string& id) { return false; }

private:
    std::map<string, string> var_table;
};

class GlobalNode : public TIDNode{
public:
    GlobalNode() : TIDNode(nullptr, NodeType::Global) {}
    ~GlobalNode() override = default;

    bool func_exist(const string& id, const func_params& params) override { return func_table.count({id, params}); }
    void push_func_id(const string& id, const string& type, const func_params& params);
    const string& get_func_type(const string& id, const func_params& params) override;

    bool struct_exist(const string& id) override { return struct_table.find(id) != struct_table.end(); }
    void push_struct_id(const string& id, const map<string, string>& vars, const map<pair<string, func_params>, string>& funcs, const map<string, string>& access);

    bool field_exist(const string& struct_id, const string& var_id) { return struct_table[struct_id].first.count({var_id, "public"}); }
    bool method_name_exist(const string& struct_id, const string& func_id);
    const string& get_field_type(const string& struct_id, const string& var_id);
    const string& get_method_type(const string& struct_id, const string& func_id, const func_params& params);

private:
    std::map<std::pair<string, func_params>, string> func_table;
    std::map<string, struct_fields> struct_table;
};

class StructNode : public TIDNode{
public:
    StructNode() = delete;
    explicit StructNode(TIDNode *parent) : TIDNode(parent, NodeType::Struct) {
        if (parent->nodeType != NodeType::Global)
            throw std::logic_error("You can't create structs not in global space");
    }
    ~StructNode() override {
        dynamic_cast<GlobalNode*>(parent)->push_struct_id(struct_name, fields, methods, access);
        parent = nullptr;
    }

    bool var_exist(const string& id) override { return fields.find(id) != fields.end(); }
    void push_id(const string& id, const string& type) override;
    const string& get_var_type(const string& id) override;

    bool func_exist(const string& id, const func_params& params) override { return methods.count({id, params}); }
    void push_method(const string& id, const string& type, const func_params& params);
    const string& get_func_id(const string& id, const func_params& params);

    void set_struct_name(const string& name) { struct_name = name; }

    void manage_access(const string& entrance);
private:
    string struct_name;
    string last_token_name_;

    std::map<string, string> access;
    std::map<string, string> fields;
    std::map<std::pair<string, func_params>, string> methods;
};

class FunctionNode : public TIDNode{
public:
    FunctionNode() = delete;
    explicit FunctionNode(TIDNode *parent) : TIDNode(parent, NodeType::Function) {
        if (parent->nodeType != NodeType::Global && parent->nodeType != NodeType::Struct)
            throw std::logic_error("You can't create funcs not in global or struct space");
    }
    ~FunctionNode() override {
        if (parent->nodeType == NodeType::Global)
            dynamic_cast<GlobalNode*>(parent)->push_func_id(func_name, return_type, params);
        else if (parent->nodeType == NodeType::Struct) {
            dynamic_cast<StructNode*>(parent)->push_method(func_name, return_type, params);
        }
        parent = nullptr;
    }

    void push_param(const string& type) { params.push_back(type); }
    void set_return_type(const string& type) { return_type = type; }
    void set_func_name(const string& name) { func_name = name; }
    void check_return_type(const string& type) {
        if ((return_type == "int" or return_type == "float") and (type == "int" or type == "float")) return;
        else if (return_type == "int" or return_type == "float" or return_type != type)
            throw std::logic_error("Return type mismatch");
    }
private:
    string func_name;
    func_params params;
    string return_type;
};

class BodyNode : public TIDNode {
public:
    BodyNode() = delete;
    explicit BodyNode(TIDNode *parent) : TIDNode(parent, NodeType::Body) {}
    ~BodyNode() override {parent = nullptr;}
};

class TID {
public:
    TID() : root(new GlobalNode()), current(root) {}
    void create_node(int);
    void delete_node();

    TIDNode* current_node() { return current; }

    bool token_exist(const string& id) { return token_exist(current, id); }
    void check_var_type(const string& id, const string& type) { return check_var_type(current, id, type); }
    const string& get_token_type(const string& id) { return get_token_type(current, id); }
    const string& get_func_return_type(const string& id, const func_params& params) { return get_func_return_type(current, id, params); }
private:
    TIDNode* root;
    TIDNode* current;

    bool token_exist(TIDNode* cur, const string& id);
    static void check_var_type(TIDNode* cur, const string& id, const string& type);
    const string& get_token_type(TIDNode* cur, const string& id);
    const string& get_func_return_type(TIDNode* cur, const string& id, const func_params& params);
};

