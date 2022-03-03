#include "StErr.h"

void StErr_ExpectedToken(enum StLex_Tokens token, StLex_Loc* loc) {
    printf("Expected token %s at %d:%d\n", StLex_GetTokenName(token), loc->startLine, loc->startColumn);
    raise(SIGABRT);
}