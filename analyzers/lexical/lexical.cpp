#include <iostream>
#include <string>
#include "lexical.h"

std::pair<std::string, int> Lexical::get_lex() {
    std::pair<std::string, int> answer;
    if (line.empty()) throw std::logic_error("Line is empty");
    if (position >= line.size()) throw std::logic_error("Position is out of range");

    debug.log("Start getting lex. Position: " + std::to_string(position));
    std::string lex;
    bool read_literal = false, skip_checking = read_string_;
    words.reset_cursor();
    while (position < line.size()) {
        char s = line[position];
        if (lex.empty() && is_tabulation(s)) {++position; continue;}

        if (lex.empty() && s == '/') {
            if (position + 1 < line.size() && line[position + 1] == '/') {
                while (position < line.size() && line[position] != '\n') ++position;
                continue;
            }
        }
        if (lex.empty() && s == '"') {
            if (read_string_) read_string_ = skip_checking = false;
            else read_string_ = true;
            words.move_to(s);
            ++position;
            break;
        }
        if (skip_checking) {
            if (s == '"') break;
            lex += s;
            ++position;
            continue;
        }
        if (words.check_next_move_to(s) && !read_literal) {
            ++position;
            words.move_to(s);
            lex += s;
            continue;
        }
        if (!words.check_next_move_to(s) && !read_literal) {
            if (words.get_cursor_type() != -1 && is_tabulation(s)) {
                ++position;
                break;
            } else if (4 <= words.get_cursor_type() && words.get_cursor_type() <= 8) // Checking on operator and punctuation
                break;

            int tmp = words.get_cursor_type();
            words.reset_cursor();
            if (tmp != -1 && words.check_next_move_to(s)) { // Checking: <lex><any_punctuation>
                words.move_to(s);
                if (words.get_cursor_type() != -1) break;
                else words.reset_cursor();
            }
            else if (is_literal(lex)) {
                read_literal = true;
                continue;
            } else {
                debug.log("BIG ERROR IN LEX ANALYZ! Check console!");
                std::cout << "Lexical analyzer send a error on: " << lex;
                exit(0);
            }
        }

        if (read_literal && words.check_next_move_to(s)) {
            words.move_to(s);
            if (words.get_cursor_type() != -1) break;
            else words.reset_cursor();
        }
        if (read_literal && is_tabulation(s)) {
            ++position;
            break;
        }

        ++position; // Successful reading reserved_lex or type 3
        lex += s;
    }
    if (skip_checking) {
        debug.log("Lex: " + lex + " Type: 10");
        answer =  {lex, 10};
    } else if ((read_literal && is_literal(lex))) {
        debug.log("Lex: " + lex + " Type: 3");
        answer =  {lex, 3};
    } else if (words.get_cursor_type() != -1) {
        debug.log("Lex: " + lex + " Type: " + std::to_string(words.get_cursor_type()));
        answer = {lex, words.get_cursor_type()};
    } else {
        debug.log("BIG ERROR IN LEX ANALYZ! Check console!");
        std::cout << "Lexical analyzer send a return error on: " << lex;
        exit(0);
    }
    debug.log("End getting lex. Position: " + std::to_string(position));
    return answer;
}