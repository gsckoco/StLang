#ifndef STEXPRESSION_H
#define STEXPRESSION_H

#include "StLex.h"

enum StParse_ScopeType {
    ST_ARGS,
    ST_PARAMS,
    ST_BODY
};

typedef struct {
    char* name;
    char* type;
} StParse_Param;

typedef struct {
    StParse_Param* params;
    unsigned int count;
} StParse_Params;

typedef struct {
    
} StParse_Argument;

typedef struct {
    char* funcName;

} StParse_FuncCall;

typedef struct {
    void* node;
} StParse_Node;

typedef struct {
    StParse_Node* nodes;
} StParse_Body;

typedef struct {
    char* name;
    StParse_Params* params;
    StParse_Body body;
    StLex_Loc loc;
    char* type;
} StParse_Function;

typedef struct
{
    StParse_Function* functions;
} StParse_Program;


#endif /* STEXPRESSION_H */