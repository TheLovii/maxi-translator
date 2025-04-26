#include "analyzers/syntax/syntax.h"
#include <iostream>

int main() {
    string t;
    std::cin >> t;
    Syntax syntax("static/test.txt");
    syntax.analyze();
    return 0;
}