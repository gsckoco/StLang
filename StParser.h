#ifndef STPARSER_H
#define STPARSER_H

#include "StLex.h"

typedef struct _Loc {
    int startColumn;
    int endColumn;
    int startLine;
    int endLine;
    int offset;
} StParse_Loc;

typedef struct _Name {
    char* name;
    StParse_Loc loc;
} StParse_Name;

typedef struct _BlockStatement {
    StParser_Expr* list;
    int openBrace;
    int closeBrace;
} StParse_BlockStatement;

typedef struct _FuncDecl {
    StParse_Name name;

} StParse_DeclFunc;

/*
    Expressions
*/

enum StParse_Expressions {
    EXP_LITERAL = 0,
    EXP_CALL,
    EXP_ASSIGNMENT,
    EXPRESSION_COUNT
};

typedef struct _LiteralExpr {
    StParse_Name name;
} StParse_LiteralExpr;

typedef struct _CallExpr {
    StParse_Name name;
} StParse_CallExpr;

typedef struct _AssignExpr {
    StParse_Name name;
} StParse_AssignExpr;

typedef struct _expr {
    union {
        StParse_LiteralExpr literalExpr;
        StParse_CallExpr callExpr;
        StParse_AssignExpr assignExpr;
    } expr;
    enum StParse_Expressions type;
} StParser_Expr;

struct StParser_Node {
};

int StParse_Parse(StLex_Token* startToken);

#endif