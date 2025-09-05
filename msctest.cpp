#include "msc.h"
#include <stdio.h>
SCRes scTestRes;
int main() {
    if (sc(stdin, &scTestRes)==-1) {
        printf("Error!\n");
    } else {
        for (int i=0; i<scTestRes.tokenSz; ++i) {
            Token tok=scTestRes.tokens[i];
            switch (tok) {
            case TOK_LIT_FLOAT:
                printf("Token %d: %s, %f\n",
                 i, tokenName[tok], *(float *)(scTestRes.symTable+scTestRes.tokenMap[i]));
                break;
            case TOK_LIT_INT:
                printf("Token %d: %s, %d\n",
                 i, tokenName[tok], *(int *)(scTestRes.symTable+scTestRes.tokenMap[i]));
                break;
            case TOK_LIT_CHAR:
                printf("Token %d: %s, %c\n",
                 i, tokenName[tok], scTestRes.symTable[scTestRes.tokenMap[i]]);
                break;
            case TOK_LIT_STRING: case TOK_IDENT:
                printf("Token %d: %s, %s\n",
                 i, tokenName[tok], &scTestRes.symTable[scTestRes.tokenMap[i]]);
                break;
            default:
                printf("Token %d: %s\n",
                 i, tokenName[tok]);
                break;
            }
        }
    }
    return 0;
}
