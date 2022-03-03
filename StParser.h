#ifndef STPARSER_H
#define STPARSER_H

#include "StLex.h"
#include "StExpressions.h"
#include "StErr.h"


int StParse_Parse(StLex_Token* startToken);

#endif