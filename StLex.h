#ifndef STLEX_H
#define STLEX_H

#define MAX_NAME_SIZE

enum StLex_Tokens {
    TK_UNDEFINED = -1,
    TK_KEYWORD = 0,
    TK_ASSIGN,
    TK_COMPARISON,
    TK_TYPE_ASSIGN,
    TK_SEMICOLON,

    TK_NAME,
    TK_NUMBER,

    TK_OPEN_PARENTH,
    TK_CLOSE_PARENTH,

    TK_OPEN_BLOCK,
    TK_CLOSE_BLOCK,

    TK_START,
    TK_END,
    TOKEN_COUNT
};

enum StLex_Keywords {
    KW_IF, KW_ELSE, KW_ELSEIF,
    KW_FOR, KW_WHILE, KW_REPEAT,
    KW_CONTINUE, KW_BREAK,
    KW_RETURN,
    KEYWORD_COUNT
};

typedef struct _string {

} StLex_String;

typedef struct _token {
    union Value
    {
        char* str;
        char c;
        int number;
    } value;
    enum StLex_Tokens tokenType;
    struct _token *nextToken;
} StLex_Token;

typedef struct _lexState {
    const char* source;
    char lastCharacter;
    char current;
    int cursor;

    int tokenCount;
    StLex_Token* tokenStart;
    StLex_Token* currentToken;
} StLex_LexState;


StLex_Token* StLex_Lex(const char* source);

#endif