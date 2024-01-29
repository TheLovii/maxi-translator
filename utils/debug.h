#ifndef MAXI_TRANSLATOR_DEBUG_H
#define MAXI_TRANSLATOR_DEBUG_H
#include <iostream>
#include <fstream>

class Debug {
public:
    //Debug() = delete;
    explicit Debug(const std::string& s) : name(s), file("../logs/debug_" + s + ".txt", std::ios::out) {
        file << "asasas";
        file << "Start debug " << name << '\n';

        std::ofstream ostrm("../logs/debug_" + s + ".txt", std::ios::out);
        ostrm << 123 ;                
    }
    ~Debug() {
        file.close();
    }
    void log(const std::string& s) {file << s << '\n';}
private:
    std::string name;
    std::ofstream file;
};
#endif //MAXI_TRANSLATOR_DEBUG_H
