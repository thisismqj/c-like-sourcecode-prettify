#ifndef MSC_H
#define MSC_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mdefs.h"
struct SCRes {
    char symTable[SYMTABLE_SIZE];
    Token tokens[TOKEN_SIZE];
    int tokenMap[TOKEN_SIZE];
    int tokenLn[TOKEN_SIZE];
    int tokenCol[TOKEN_SIZE];
    int tokenSz;
};
int sc(FILE *fp, SCRes *ans);

#endif
