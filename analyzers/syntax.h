#ifndef MAXI_TRANSLATOR_SYNTAX_H
#define MAXI_TRANSLATOR_SYNTAX_H
#include "../utils/debug.h"

class Syntax {
public:
    Syntax() : debug("syntax") {
        debug.log("Success create syntax analyzer");
    }
    ~Syntax() = default;
private:
    Debug debug;
};
#endif //MAXI_TRANSLATOR_SYNTAX_H
