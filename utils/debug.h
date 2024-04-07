#ifndef MAXI_TRANSLATOR_DEBUG_H
#define MAXI_TRANSLATOR_DEBUG_H
#include <iostream>
#include <fstream>

class Debug {
public:
    Debug() = delete;
    explicit Debug(const std::string& s) : name(s), file("../logs/debug_" + s + ".txt") {
        file << "Start debug " << name << std::endl;
    }
    ~Debug() {
        file.close();
    }
    void log(const std::string& s) {file << s << std::endl;}
private:
    std::string name;
    std::ofstream file;
};
#endif //MAXI_TRANSLATOR_DEBUG_H
