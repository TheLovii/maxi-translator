#ifndef MAXI_TRANSLATOR_LEXICAL_H
#define MAXI_TRANSLATOR_LEXICAL_H
#include <fstream>
#include <sstream>
#include <regex>
#include "../../utils/trie.h"
#include "../../utils/debug.h"
// TODO: доработать работу с Line

class Lexical {
public:
    explicit Lexical(const std::string& file_name) : debug("lexical"), position(0), number_of_line(1) {
        words = Trie();
        std::ifstream file("static/reserved_lexem.txt", std::ios::in);

        int type = 1;
        while (!file.eof()) {
            std::string word;
            file >> word;
            if (word == "type") {
                file >> type;
                continue;
            }
            words.insert(word, type);
        }
        file.close();
        debug.log("Success read reserved_lexem.txt");

        std::ifstream f(file_name, std::ios::in);
        std::stringstream ss;
        ss << f.rdbuf();
        line = ss.str();
        debug.log("Success read file");

        debug.log("Success create lexical analyzer");
    }
    Lexical(const Lexical&) = delete;
    ~Lexical() = default;

    std::pair<std::string, int> get_lex();

private:
    static bool is_literal(std::string &s) {
        std::regex reg(R"(^\w*$)");
        return std::regex_match(s, reg);
    }
    bool is_tabulation(char &s) {
        if (s == '\n') number_of_line++;
        if (s == ' ' || s == '\t' || s == '\n') return true;
        return false;
    }

    Trie words;
    Debug debug;
    std::string line;
    size_t position;
    size_t number_of_line;

    bool read_string_ = false;
};
#endif //MAXI_TRANSLATOR_LEXICAL_H