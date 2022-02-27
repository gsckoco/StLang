#include "StLex.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

const char* TokenNames[TOKEN_COUNT] = {
    "TK_KEYWORD",
    "TK_ASSIGN",
    "TK_COMPARISON",
    "TK_TYPE_ASSIGN",
    "TK_SEMICOLON",

    "TK_NAME",
    "TK_NUMBER",

    "TK_OPEN_PARENTH",
    "TK_CLOSE_PARENTH",

    "TK_OPEN_BLOCK",
    "TK_CLOSE_BLOCK",

    "TK_START",
    "TK_END"
};

/*

    KW_IF, KW_ELSE, KW_ELSEIF,
    KW_FOR, KW_WHILE, KW_REPEAT,
    KW_CONTINUE, KW_BREAK,
    KW_RETURN,

*/

const char* KeywordNames[KEYWORD_COUNT] = {
    "if", "else", "elseif",
    "for", "while", "repeat",
    "continue", "break",
    "return"
};

void nextCharacter(StLex_LexState* lexState)
{
    //lexState->lastCharacter = lexState->current;
    lexState->cursor++;
    lexState->current = lexState->source[lexState->cursor];
    // TODO Check for EOF
}

void previousCharacter(StLex_LexState* lexState)
{
    //lexState->lastCharacter = lexState->current;
    lexState->cursor--;
    lexState->current = lexState->source[lexState->cursor];
    // TODO Check for EOF
}

void addTokenToList(StLex_LexState* lexState, StLex_Token* token)
{
    //lexState->tokens;
    //token->nextToken = 0;
    lexState->currentToken->nextToken = token;
    token->lastToken = lexState->currentToken;
    lexState->currentToken = token;
    return;
}

int isCharAlphabetical(char c)
{
    if (
        (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '_')) {
        return 1;
    } else {
        return 0;
    }
}

int isCharNumerical(char c)
{
    if (
        (c >= '0' && c <= '9')) {
        return 1;
    } else {
        return 0;
    }
}

char* clone(char* temp, int size)
{
    
    char *str = malloc(size + 1);
    strncpy(str, temp, size + 1);
    return str;
}

const char* StLex_GetTokenName(enum StLex_Tokens token) {
    return TokenNames[token];
}

StLex_Token* StLex_Lex(const char* file)
{
    StLex_LexState* lexState = malloc(sizeof(StLex_LexState));

    lexState->source = file;
    lexState->cursor = 0;
    lexState->current = lexState->source[lexState->cursor];
    lexState->currentLine = 1;

    StLex_Token* startToken = malloc(sizeof(StLex_Token));

    startToken->tokenType = TK_START;

    lexState->tokenStart = startToken;
    lexState->currentToken = startToken;

    char* temp = "";

    for (;;) {
        //printf("%c", lexState->current);

        StLex_Token *newToken = malloc(sizeof(StLex_Token));
        newToken->tokenType = TK_UNDEFINED;

        newToken->line = lexState->currentLine;
        newToken->startColumn = lexState->cursor + 1;
        newToken->nextToken = 0;

        if (lexState->current == 0) {
            // EOF
            char* str = "<EOF>";
            //StLex_Token newToken = { .tokenType = TK_END, .value.str = str };

            newToken->tokenType = TK_END;
            newToken->value.str = str;

            addTokenToList(lexState, newToken);
            break;
        } else if (lexState->current == '\n') {
        }

        switch (lexState->current) {
        case '{': {
            newToken->tokenType = TK_OPEN_BLOCK;
            newToken->value.c = '{';
            break;
        }
        case '}': {
            newToken->tokenType = TK_CLOSE_BLOCK;
            newToken->value.c = '}';
            break;
        }
        case '(': {
            newToken->tokenType = TK_OPEN_PARENTH;
            newToken->value.c = '(';
            break;
        }
        case ')': {
            newToken->tokenType = TK_CLOSE_PARENTH;
            newToken->value.c = ')';
            break;
        }
        case ':': { // Type assignment
            newToken->tokenType = TK_TYPE_ASSIGN;
            newToken->value.c = ':';
            break;
        }
        case '=': { // Variable assignment
            nextCharacter(lexState);
            if (lexState->current == '=') {
                // Comparison
                char* str = "==";
                newToken->tokenType = TK_COMPARISON;
                newToken->value.str = str;
            } else {
                // Assignment
                newToken->tokenType = TK_ASSIGN;
                newToken->value.c = '=';
                previousCharacter(lexState);
            }
            break;
        }
        case '\n': {
            lexState->currentLine++;
            break;
        }
        default: { // Is alpha numeric
            char *temp = malloc(0);
            int size = 0;
            //temp[0] = 0;
            int tokenType = 0;
            if (isCharNumerical(lexState->current)) {
                tokenType = TK_NUMBER;
                while (isCharNumerical(lexState->current)) {
                    temp = realloc(temp, size + 1);
                    temp[size] = lexState->current;
                    size++;
                    nextCharacter(lexState);
                }
                previousCharacter(lexState);
                temp = realloc(temp, size);
                temp[size] = 0;
                break;
            } else if (isCharAlphabetical(lexState->current)) {
                tokenType = TK_NAME;
                while (isCharAlphabetical(lexState->current) || isCharNumerical(lexState->current)) {
                    temp = realloc(temp, size + 1);
                    temp[size] = lexState->current;
                    size++;
                    nextCharacter(lexState);
                }
                previousCharacter(lexState);
                temp = realloc(temp, size);
                temp[size] = 0;
            } else {
                break;
            }

            if (tokenType == TK_NAME) {
                int i;
                for (i = 0; i < KEYWORD_COUNT; i++) {
                    if (strncmp(temp, KeywordNames[i], size) == 0) {
                        //printf("%s == %s\n", temp, KeywordNames[i]);
                        tokenType = TK_KEYWORD;
                        break;
                    }
                }
                if (tokenType == TK_KEYWORD) {
                    newToken->value.number = i;
                } else {
                    newToken->value.str = temp;
                }
            } else {
                // Is a number
                //char* t = clone(temp, size);
                //newToken->tokenType = tokenType;
                //newToken->value.str = t;
                newToken->tokenType = TK_UNDEFINED;
            }
            newToken->tokenType = tokenType;
            break;
        }
        }
        newToken->endColumn = lexState->cursor + 1;
        if (newToken->tokenType != TK_UNDEFINED) {
            addTokenToList(lexState, newToken);
        }
        //printf("%s\n", TokenNames[lexState->currentToken->tokenType]);
        nextCharacter(lexState);
    }
    StLex_Token* currentHead = lexState->tokenStart;
    while (currentHead != 0 && currentHead->tokenType != TK_END) {
        currentHead = currentHead->nextToken;
        printf("%s ", TokenNames[currentHead->tokenType]);
        switch (currentHead->tokenType) {
        case TK_COMPARISON:
        case TK_NAME:
            printf("String: %s @ %d -> %d", (currentHead->value.str), currentHead->startColumn, currentHead->endColumn);
            break;
        case TK_KEYWORD:
            printf("Keyword: %s @ %d -> %d", KeywordNames[currentHead->value.number], currentHead->startColumn, currentHead->endColumn);
            break;
        case TK_ASSIGN:
        case TK_TYPE_ASSIGN:
        case TK_SEMICOLON:
        case TK_OPEN_PARENTH:
        case TK_CLOSE_PARENTH:
        case TK_OPEN_BLOCK:
        case TK_CLOSE_BLOCK:
            printf("%c @ %d -> %d", currentHead->value.c, currentHead->startColumn, currentHead->endColumn);
            break;
        case TK_START:
            printf("<FILE START>");
            break;
        case TK_END:
            printf("<EOF>");
            break;
        default:
            break;
        }
        printf("\n");
    }

    return lexState->tokenStart->nextToken;
}