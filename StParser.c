#include "StParser.h"

#include "stdlib.h"
#include "stdio.h"
#include "signal.h"

struct StParser_ScopeEntry
{
    StLex_Token *token;
    struct StParser_ScopeEntry *previous;
    void *scope;
    enum StParse_ScopeType scopeType;
};

struct StParser_State
{
    int scopeDepth;
    struct StParser_ScopeEntry *scopeStack;
    StLex_Token *head;
};

void StParser_NextToken(struct StParser_State *state)
{
    // printf("Got token: %s %d\n", StLex_GetTokenName(state->head->tokenType), state->head->tokenType);
    state->head = state->head->nextToken;
}

void StParser_PreviousToken(struct StParser_State *state)
{
    // printf("Got token: %s %d\n", StLex_GetTokenName(state->head->tokenType), state->head->tokenType);
    state->head = state->head->lastToken;
}

StParse_Params *_parseParams(struct StParser_State *state)
{
    struct StParser_ScopeEntry *scopeLevel = malloc(sizeof(struct StParser_ScopeEntry));

    scopeLevel->previous = state->scopeStack;
    state->scopeStack = scopeLevel;
    state->scopeDepth++;

    int currentDepth = state->scopeDepth - 1;

    unsigned int paramCount = 0;
    StParse_Param *newParam = malloc(sizeof(StParse_Param) * paramCount);

    scopeLevel->scope = newParam;
    scopeLevel->scopeType = ST_PARAMS;

    while (state->scopeDepth != currentDepth)
    {
        StParser_NextToken(state);
        switch (state->head->tokenType)
        {
        case TK_CLOSE_PARENTH:
        {
            // printf("Close\n");
            state->scopeDepth--;
            break;
        }
        case TK_OPEN_PARENTH:
        {
            // Unexpected open parenth, throw error
            printf("Unexpected open parenth %d:%d\n", state->head->loc.startLine, state->head->loc.startColumn);
            raise(SIGABRT);
            break;
        }
        case TK_NAME:
        {
            char *paramName = state->head->value.str;
            StParser_NextToken(state);
            if (state->head->tokenType == TK_TYPE_ASSIGN)
            {
                StParser_NextToken(state);
                if (state->head->tokenType == TK_NAME)
                {
                    paramCount++;
                    scopeLevel->scope = realloc(scopeLevel->scope, paramCount);

                    StParse_Param *params = scopeLevel->scope;
                    params[paramCount - 1].name = paramName;
                    params[paramCount - 1].type = state->head->value.str;
                }
                else
                {
                    StErr_ExpectedToken(TK_TYPE_ASSIGN, &state->head->loc);
                    raise(SIGABRT);
                }
            }
            else
            {
                StErr_ExpectedToken(TK_TYPE_ASSIGN, &state->head->loc);
                raise(SIGABRT);
            }

            break;
        }
        default:
            break;
        }
    }

    StParse_Params *params = malloc(sizeof(StParse_Params));
    params->params = scopeLevel->scope;
    params->count = paramCount;
    return params;
}

StParse_Body *_parseBody(struct StParser_State *state)
{
    struct StParser_ScopeEntry *scopeLevel = malloc(sizeof(struct StParser_ScopeEntry));

    StParse_Body *body = malloc(sizeof(StParse_Body));

    scopeLevel->scope = body;
    scopeLevel->previous = state->scopeStack;
    state->scopeStack = scopeLevel;
    state->scopeDepth++;

    int openLevel = state->scopeDepth - 1;
    while (state->scopeDepth != openLevel)
    {
        StParser_NextToken(state);
        switch (state->head->tokenType)
        {
        case TK_OPEN_BLOCK:
        {
            StParse_Body *newBody = _parseBody(state);
            break;
        }
        case TK_CLOSE_BLOCK:
        {
            state->scopeDepth--;
            state->scopeStack = state->scopeStack->previous;
            return body;
        }
        case TK_NAME:
            if (
                state->head->nextToken->tokenType == TK_TYPE_ASSIGN &&
                state->head->nextToken->nextToken->tokenType == TK_NAME &&
                state->head->nextToken->nextToken->nextToken->tokenType == TK_ASSIGN)
            {
                // Variable decl
                const char *variableName = state->head->value.str;
                const char *typeName = state->head->nextToken->nextToken->value.str;
                StParser_NextToken(state);
                StParser_NextToken(state);
                StParser_NextToken(state);

                switch (state->head->tokenType)
                {
                case TK_NUMBER: {
                    // TODO check if variable type is a number (int or float)
                    break;
                }
                case TK_STRING: {
                    // TODO check if variable type is a string
                    break;
                }
                case TK_NAME: {
                    
                    break;
                }
                default:
                    break;
                }
            }
            else if (state->head->nextToken->tokenType == TK_ASSIGN)
            {
                // Variable assignment
            }
            else if (state->head->nextToken->tokenType == TK_OPEN_PARENTH)
            {
                // Function call
            }
            else
            {
            }
            break;
        default:
            break;
        }
    }
}

void _parseFunction(struct StParser_State *state)
{
    StParse_Function *newFunction = malloc(sizeof(StParse_Function));

    StLex_Loc *loc = &newFunction->loc;

    loc->startColumn = state->head->loc.startColumn;
    loc->startLine = state->head->loc.startLine;

    newFunction->name = state->head->value.str;

    StParser_NextToken(state);

    if (state->head->tokenType == TK_OPEN_PARENTH)
    {
        // Has args
        StParse_Params *params = _parseParams(state);
        int i;
        printf("Params: ");
        for (i = 0; i < params->count; i++)
        {
            printf("%s: %s, ", params->params[i].name, params->params[i].type);
        }
        printf("\n");
        StParser_NextToken(state);
        StLex_PrintToken(state->head);
        if (state->head->tokenType == TK_TYPE_ASSIGN)
        {
            StParser_NextToken(state);
        }
        else
        {
            StErr_ExpectedToken(TK_NAME, &state->head->loc);
        }

        if (state->head->tokenType == TK_NAME)
        {
            // TODO: check check if type exists.
            newFunction->type = state->head->value.str;
            StParser_NextToken(state);
        }
        else
        {
            StErr_ExpectedToken(TK_NAME, &state->head->loc);
        }

        if (state->head->tokenType == TK_OPEN_BLOCK)
        {
            _parseBody(state);
        }
        else
        {
            StErr_ExpectedToken(TK_OPEN_BLOCK, &state->head->loc);
        }

        newFunction->params = params;
    }
    else
    {
        // Has no args
        StErr_ExpectedToken(TK_OPEN_PARENTH, &state->head->loc);
    }

    loc->endColumn = state->head->loc.endColumn;
    loc->endLine = state->head->loc.endLine;
}

int StParse_Parse(StLex_Token *startToken)
{
    struct StParser_State *state = malloc(sizeof(struct StParser_State));
    state->head = startToken;
    state->scopeDepth = 0;
    state->scopeStack = 0;

    StParse_Program *program = malloc(sizeof(StParse_Program));

    while (state->head != 0 && state->scopeDepth == 0)
    {
        // printf("Got token: %s %d\n", StLex_GetTokenName(state->head->tokenType), state->head->tokenType);
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

    if (state->scopeDepth != 0)
    {
        printf("Expected block close");
    }
}