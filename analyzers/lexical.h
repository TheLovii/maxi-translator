#ifndef MAXI_TRANSLATOR_LEXICAL_H
#define MAXI_TRANSLATOR_LEXICAL_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include "../utils/trie.h"
#include "../utils/debug.h"

class Lexical {
public:
    explicit Lexical(const std::string& file_name) : debug("lexical"), position(0) {
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

    std::pair<std::string, int> get_lex() {
        std::pair<std::string, int> answer;
        if (line.empty()) throw std::logic_error("Line is empty");
        if (position >= line.size()) throw std::logic_error("Position is out of range");

        debug.log("Start getting lex. Position: " + std::to_string(position));
        std::string lex;
        bool read_type_2 = false;
        words.reset_cursor();
        while (position < line.size()) {
            char s = line[position];
            if (is_tabulation(s)) {++position; continue;}

            if (words.check_next_move_to(s) && !read_type_2) {
                ++position;
                words.move_to(s);
                lex += s;
                continue;
            }
            if (!words.check_next_move_to(s) && !read_type_2) {
                if (words.get_cursor_type() != -1) break;
                else if (is_type_3(lex)) {
                    read_type_2 = true;
                    words.reset_cursor();
                    continue;
                } else {
                    std::cout << "Lexical analyzer send a error on: " << lex;
                    exit(0);
                }
            }

            if (read_type_2 && words.check_next_move_to(s)) {
                words.move_to(s);
                if (words.get_cursor_type() != -1) break;
                else words.reset_cursor();
            }
            ++position;
            lex += s;
        }
        if (read_type_2 && is_type_3(lex)) {
            debug.log("Lex: " + lex + " Type: 3");
            answer =  {lex, 3};
        } else if (words.get_cursor_type() != -1) {
            debug.log("Lex: " + lex + " Type: " + std::to_string(words.get_cursor_type()));
            answer = {lex, words.get_cursor_type()};
        } else {
            std::cout << "Lexical analyzer send a error on: " << lex;
            exit(0);
        }
        debug.log("End getting lex. Position: " + std::to_string(position));
        return answer;
    }

private:
    static bool is_type_3(std::string &s) {
        std::regex reg(R"(^\w*$)");
        return std::regex_match(s, reg);
    }
    static bool is_tabulation(char &s) {
        if (s == ' ' || s == '\t' || s == '\n') return true;
        return false;
    }

    Trie words;
    Debug debug;
    std::string line;
    size_t position;
};
#endif //MAXI_TRANSLATOR_LEXICAL_H