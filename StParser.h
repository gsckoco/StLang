#ifndef STPARSER_H
#define STPARSER_H

#include "StLex.h"
#include "StExpressions.h"


struct StParser_Node {
};

int StParse_Parse(StLex_Token* startToken);

#endif