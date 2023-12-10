#include <iostream>
#include "analyzers/lexical.h"

int main() {
    Lexical lexical("static/test.txt");
    while (true) {
        try {
            auto lex = lexical.get_lex();
            std::cout << "Lex: " << lex.first << " Type: " << lex.second << '\n';
        } catch (std::logic_error &e) {
            std::cout << e.what() << '\n';
            break;
        }
    }
    return 0;
}