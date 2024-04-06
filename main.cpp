#include "analyzers/syntax/syntax.h"
#include <iostream>

int main() {
    Syntax syntax("static/test.txt");
    syntax.analyze();
    return 0;
}