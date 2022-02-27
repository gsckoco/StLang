#ifndef STEXPRESSION_H
#define STEXPRESSION_H

typedef struct _Loc {
    int startColumn;
    int endColumn;
    int startLine;
    int endLine;
    int offset;
} StParse_Loc;

typedef struct {

} StParse_Body;

enum StParse_ScopeType {
    ST_ARGS,
    ST_PARAMS,
    ST_BODY
};

typedef struct {
    char* name;
    char* type;
} StParse_Param;

// Function call
typedef struct {

} StParse_Args;

// Function decl
typedef struct {
    StParse_Param* params;
} StParse_Params;

typedef struct {
    char* name;
    StParse_Body body;
    StParse_Loc loc;
} StParse_Function;

typedef struct
{
    StParse_Function* functions;
} StParse_Program;


#endif /* STEXPRESSION_H */