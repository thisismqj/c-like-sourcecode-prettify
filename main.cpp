#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpa.h"
inline bool isAlpha(char ch) {return 'a'<=ch&&ch<='z'||'A'<=ch&&ch<='Z';}
inline bool isDigit(char ch) {return '0'<=ch&&ch<='9';}
Token readToken(FILE *fp, char **symTable) {
    char ch;
    while (ch=fgetc(fp), ch==' '||ch=='\n'||ch=='\t'||ch=='\r') ;
    if (ch==EOF) return TOK_EOF;
    if (ch=='_'||isAlpha(ch)) {
        char *p=*symTable;
        *p++=ch;
        while ((ch=fgetc(fp), ch=='_'||isDigit(ch)||isAlpha(ch))) *p++=ch;
        ungetc(ch, fp);
        *p=0;
        for (int i=0; i<12; ++i)
            if (strcmp(*symTable, tokenName[i])==0) return (Token) i;
        *symTable=p+1;
        return TOK_IDENT;
    } else if (isDigit(ch)||ch=='-') {
        int sym=1;
        if (ch=='-') {
            char cch=fgetc(fp);
            if (!(isDigit(cch)||ch=='.')) {
                if (cch=='>') {
                    return TOK_ARROW;
                } else {
                    ungetc(cch, fp);
                    return TOK_SUB;
                }
            } else {
                sym=-1;
                ch=cch;
            }
        }
        bool dot=0;
        char *p = *symTable;
        do {
            if (ch=='.') {
                if (dot) return TOK_ERROR;
                else dot=1;
            }
            *p++=ch;
            ch=fgetc(fp);
        } while (ch=='.'||isDigit(ch));
        ungetc(ch, fp);
        if (!dot) {
            int ans=0;
            for (char *q=*symTable; q!=p; ++q) {
                ans=ans*10+*q-'0';
            }
            *(int*)*symTable = sym*ans;
            *symTable+=4;
            **symTable = 0;
            ++*symTable;
            return TOK_LIT_INT;
        } else {
            float ans=strtof(*symTable, &p);
            *(float*)*symTable = sym*ans;
            *symTable+=4;
            **symTable = 0;
            ++*symTable;
            return TOK_LIT_FLOAT;
        }
    } else if (ch=='\'') {
        char cch=fgetc(fp);
        char ccch=fgetc(fp);
        if (ccch!='\'') return TOK_ERROR;
        else {
            *(*symTable)++ = cch;
            *(*symTable)++ = 0;
            return TOK_LIT_CHAR;
        }
    } else if (ch=='\"') {
        char cch;
        char *p=*symTable;
        while (cch=fgetc(fp), cch!=EOF&&cch!='\"') *p++ = cch;
        if (cch==EOF) return TOK_ERROR;
        else  {
            *p=0;
            *symTable=p+1;
            return TOK_LIT_STRING;
        }
    } else {
        char cch;
        switch (ch) {
        case '=':
            cch=fgetc(fp);
            if (cch=='=') return TOK_EQ;
            else {
                ungetc(cch, fp);
                return TOK_ASSIGN;
            }
            break;
        case '!':
            cch=fgetc(fp);
            if (cch=='=') return TOK_NEQ;
            else {
                ungetc(cch, fp);
                return TOK_NOT;
            }
            break;
        case '<':
            cch=fgetc(fp);
            if (cch=='=') return TOK_LEQ;
            else {
                ungetc(cch, fp);
                return TOK_LT;
            }
            break;
        case '>':
            cch=fgetc(fp);
            if (cch=='=') return TOK_GEQ;
            else {
                ungetc(cch, fp);
                return TOK_GT;
            }
            break;
        case '&':
            cch=fgetc(fp);
            if (cch=='&') return TOK_LOGICAL_AND;
            else {
                ungetc(cch, fp);
                return TOK_ERROR;
            }
            break;
        case '|':
            cch=fgetc(fp);
            if (cch=='|') return TOK_LOGICAL_OR;
            else {
                ungetc(cch, fp);
                return TOK_ERROR;
            }
            break;
        case '+':
            return TOK_ADD;
            break;
        case '*':
            return TOK_MUL;
            break;
        case '/':
            return TOK_DIV;
            break;
        case '%':
            return TOK_MOD;
            break;
        case '(':
            return TOK_LP;
            break;
        case ')':
            return TOK_RP;
            break;
        case '{':
            return TOK_LBRACE;
            break;
        case '}':
            return TOK_RBRACE;
            break;
        case '[':
            return TOK_LBRACKET;
            break;
        case ']':
            return TOK_RBRACKET;
            break;
        case ';':
            return TOK_SEMI;
            break;
        case ',':
            return TOK_COMMA;
            break;
        case '.':
            return TOK_DOT;
            break;
        default:
            return TOK_ERROR;
            break;
        }
    }
}
int sc(FILE *fp, SCRes *ans) {
    int cnt=0;
    char *p = ans->symTable, *oldP=p, *stP=p;
    Token tok;
    while ((tok=readToken(fp, &p))!=TOK_EOF) {
        if (tok==TOK_ERROR) return -1;
        ans->tokens[cnt] = tok;
        ans->tokenMap[cnt]=oldP-stP;
        oldP=p;
        ++cnt;
    }
    ans->tokenSz=cnt;
    return 0;
}
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
