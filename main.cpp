#include "analyzers/syntax/syntax.h"

int main() {
    Syntax syntax("static/test.txt");
    syntax.analyze();
    return 0;
}