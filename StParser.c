#include "StParser.h"
#include "stdlib.h"
#include "stdio.h"
#include "signal.h"

struct StParser_ScopeEntry
{
    StLex_Token* token;
    struct StParser_ScopeEntry* previous;
    void* scope;
    enum StParse_ScopeType scopeType;
};

struct StParser_State
{
    int scopeDepth;
    struct StParser_ScopeEntry* scopeStack;
    StLex_Token* head;
};

void StParser_NextToken(struct StParser_State* state) {
    printf("Got token: %s %d\n", StLex_GetTokenName(state->head->tokenType), state->head->tokenType);
    state->head = state->head->nextToken;
}

void StParser_PreviousToken(struct StParser_State* state) {
    printf("Got token: %s %d\n", StLex_GetTokenName(state->head->tokenType), state->head->tokenType);
    state->head = state->head->lastToken;
}

void _parseParams(struct StParser_State* state) {
    struct StParser_ScopeEntry* scopeLevel = malloc(sizeof(struct StParser_ScopeEntry));

    scopeLevel->previous = state->scopeStack;
    state->scopeStack = scopeLevel;
    state->scopeDepth++;

    int currentDepth = state->scopeDepth-1;

    StParse_Params* newParams = malloc(sizeof(StParse_Params));

    scopeLevel->scope = newParams;
    scopeLevel->scopeType = ST_PARAMS;

    while (state->scopeDepth != currentDepth) {
        StParser_NextToken(state);
        switch (state->head->tokenType)
        {
        case TK_CLOSE_PARENTH: {
            printf("Close\n");
            state->scopeDepth--;
            break;
        }
        case TK_OPEN_PARENTH: {
            // Unexpected open parenth, throw error
            printf("Unexpected open parenth %d:%d\n", state->head->line, state->head->startColumn);
            raise(SIGABRT);
            break;
        }
        case TK_NAME: {
            char* paramName = state->head->value.str;
            StParser_NextToken(state);
            if (state->head->tokenType == TK_TYPE_ASSIGN) {
                StParser_NextToken(state);
                if (state->head->tokenType == TK_NAME) {

                }
            } else {
                printf("Expected type assigment\n");
                raise(SIGABRT);
            }

            break;
        }
        default:
            break;
        }
    }
}

void _parseFunction(struct StParser_State* state) {
    StParse_Function* newFunction = malloc(sizeof(StParse_Function));

    StParse_Loc* loc = &newFunction->loc;

    loc->startColumn = state->head->startColumn;
    loc->startLine = state->head->line;

    newFunction->name = state->head->value.str;

    StParser_NextToken(state);

    if (state->head->tokenType == TK_OPEN_PARENTH) {
        // Has args
        printf("Parse params\n");
        _parseParams(state);
    } else {
        // Has no args
        printf("No args\n");
    }
    
    loc->endColumn = state->head->endColumn;
    loc->endLine = state->head->line;
}

int StParse_Parse(StLex_Token* startToken)
{
    struct StParser_State* state = malloc(sizeof(struct StParser_State));
    state->head = startToken;
    state->scopeDepth = 0;
    state->scopeStack = 0;

    StParse_Program* program = malloc( sizeof(StParse_Program) );
    
    while (state->head != 0 && state->scopeDepth == 0)
    {
        printf("Got token: %s %d\n", StLex_GetTokenName(state->head->tokenType), state->head->tokenType);
        switch (state->head->tokenType)
        {
        case TK_NAME:
            printf("Parse function\n");
            _parseFunction(state);
            break;
        }
        // Goto next token
        state->head = state->head->nextToken;
    }

    if (state->scopeDepth != 0) {
        printf("Expected block close");
    }
}