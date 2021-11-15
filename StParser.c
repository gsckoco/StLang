#include "StParser.h"

int StParse_Parse(StLex_Token* startToken)
{
    StLex_Token* head = startToken;

    while (startToken != 0)
    {
        
        // Goto next token
        head = head->nextToken;
    }
}