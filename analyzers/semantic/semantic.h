#include <vector>
#include <string>
#include <map>
#include <iostream>

class Semantic {
    public:
        explicit Semantic() {
            std::cout << 1;
        }
        void push_id(const std::string& id, const std::string& type);
        const std::string& check_id(const std::string& id);
        void create_TID();
        void delete_TID();

        void push_func_id(const std::string& id);
        const std::string& check_func_id(const std::string& id);
        void check_func_params(const std::vector<std::string>&  );
        void push_func_params(const std::vector<std::string>&);
    private:
        std::map<std::string, std::string> TID;
        std::map<std::string, std::string> fTID;

};