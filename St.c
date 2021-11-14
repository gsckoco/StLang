#include "St.h"

/*

Lexer -> (Tokens) -> Parser -> (AST) -> Semantic Analyzer

*/

void main(int argc, char* argv[])
{
    if (argc > 0) {
        FILE* sourceFile = fopen(argv[1], "rb");

        int err = fseek(sourceFile, 0, SEEK_END);

        if (err != 0) {
            printf("An error has occured while getting the file size 0\n");
            return;
        }
        int fileSize = (int)ftell(sourceFile);
        if (err != 0) {
            printf("An error has occured while getting the file size 1\n");
            return;
        }

        char* sourceString = malloc(fileSize + 1);
        int i;
        for (i = 0; i <= fileSize; i++) {
            sourceString[i] = '\0';
        }

        rewind(sourceFile);
        int sizeOfFile = fread(sourceString, fileSize, 1, sourceFile);
        sourceString[fileSize] = '\0';
        fclose(sourceFile);
        StLex_Lex(sourceString);
    }
}