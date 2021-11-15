#ifndef STPARSER_H
#define STPARSER_H

#include "StLex.h"

struct StParser_Leaf
{
    
};


int StParse_Parse(StLex_Token* startToken);

#endif