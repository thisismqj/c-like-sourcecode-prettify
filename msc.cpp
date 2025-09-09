#include "msc.h"
static inline bool isAlpha(char ch) {return 'a'<=ch&&ch<='z'||'A'<=ch&&ch<='Z';}
static inline bool isDigit(char ch) {return '0'<=ch&&ch<='9';}
static const int MAXFILESIZE = 32768;
struct LnCol {
    int ln[MAXFILESIZE], col[MAXFILESIZE];
};
static LnCol lnCol;
static Token readToken(FILE *fp, char **symTable,  int &pos, bool numFlag=1) {
    char ch;
    while (ch=fgetc(fp), ch==' '||ch=='\n'||ch=='\t'||ch=='\r') ;
    if (ch==EOF) return TOK_EOF;
    if (ch=='_'||isAlpha(ch)) {
        pos = ftell(fp)-1;
        char *p=*symTable;
        *p++=ch;
        while ((ch=fgetc(fp), ch=='_'||isDigit(ch)||isAlpha(ch))) *p++=ch;
        ungetc(ch, fp);
        *p=0;
        for (int i=0; i<12; ++i)
            if (strcmp(*symTable, tokenName[i])==0) return (Token) i;
        *symTable=p+1;
        return TOK_IDENT;
    } else if (numFlag&&(isDigit(ch)||ch=='-')) {
        int q = ftell(fp)-1;
        int sym=1;
        if (ch=='-') {
            char cch=fgetc(fp);
            if (!(isDigit(cch)||ch=='.')) {
                if (cch=='>') {
                    pos=q;
                    return TOK_ARROW;
                } else {
                    pos=q;
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
                pos=q;
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
            pos = q;
            return TOK_LIT_INT;
        } else {
            float ans=strtof(*symTable, &p);
            *(float*)*symTable = sym*ans;
            *symTable+=4;
            **symTable = 0;
            ++*symTable;
            pos = q;
            return TOK_LIT_FLOAT;
        }
    } else if (ch=='\'') {
        int q = ftell(fp)-1;
        char cch=fgetc(fp);
        char ccch=fgetc(fp);
        if (ccch!='\'') {
            pos=q;
            return TOK_ERROR;
        } else {
            *(*symTable)++ = cch;
            *(*symTable)++ = 0;
            pos = q;
            return TOK_LIT_CHAR;
        }
    } else if (ch=='\"') {
        int q = ftell(fp)-1;
        char cch;
        char *p=*symTable;
        while (cch=fgetc(fp), cch!=EOF&&cch!='\"') *p++ = cch;
        if (cch==EOF) {
            pos=q;
            return TOK_ERROR;
        } else  {
            *p=0;
            *symTable=p+1;
            pos = q;
            return TOK_LIT_STRING;
        }
    } else {
        int q = ftell(fp)-1;
        char cch;
        switch (ch) {
        case '=':
            cch=fgetc(fp);
            if (cch=='=') return TOK_EQ;
            else {
                ungetc(cch, fp);
                pos=q;
                return TOK_ASSIGN;
            }
            break;
        case '!':
            cch=fgetc(fp);
            if (cch=='=') return TOK_NEQ;
            else {
                ungetc(cch, fp);
                pos=q;
                return TOK_NOT;
            }
            break;
        case '<':
            cch=fgetc(fp);
            if (cch=='=') {
                pos=q;
                return TOK_LEQ;
            } else {
                ungetc(cch, fp);
                pos=q;
                return TOK_LT;
            }
            break;
        case '>':
            cch=fgetc(fp);
            if (cch=='=') {
                pos=q;
                return TOK_GEQ;
            } else {
                ungetc(cch, fp);
                pos=q;
                return TOK_GT;
            }
            break;
        case '&':
            cch=fgetc(fp);
            if (cch=='&') {
                pos=q;
                return TOK_LOGICAL_AND;
            } else {
                ungetc(cch, fp);
                pos=q;
                return TOK_ERROR;
            }
            break;
        case '|':
            cch=fgetc(fp);
            if (cch=='|') {
                pos=q;
                return TOK_LOGICAL_OR;
            } else {
                ungetc(cch, fp);
                pos=q;
                return TOK_ERROR;
            }
            break;
        case '+':
            return TOK_ADD;
            break;
        case '-':
            cch=fgetc(fp);
            if (cch=='>') {
                pos=q;
                return TOK_ARROW;
            } else {
                ungetc(cch, fp);
                pos=q;
                return TOK_SUB;
            }
            break;
        case '*':
            pos=q;
            return TOK_MUL;
            break;
        case '/':
            pos=q;
            return TOK_DIV;
            break;
        case '%':
            pos=q;
            return TOK_MOD;
            break;
        case '(':
            pos=q;
            return TOK_LP;
            break;
        case ')':
            pos=q;
            return TOK_RP;
            break;
        case '{':
            pos=q;
            return TOK_LBRACE;
            break;
        case '}':
            pos=q;
            return TOK_RBRACE;
            break;
        case '[':
            pos=q;
            return TOK_LBRACKET;
            break;
        case ']':
            pos=q;
            return TOK_RBRACKET;
            break;
        case ';':
            pos=q;
            return TOK_SEMI;
            break;
        case ',':
            pos=q;
            return TOK_COMMA;
            break;
        case '.':
            pos=q;
            return TOK_DOT;
            break;
        default:
            pos=q;
            return TOK_ERROR;
            break;
        }
    }
}
int sc(FILE *fp, SCRes *ans) {
    char ch;
    int ln=0, col=0;
    while (ch=fgetc(fp), ch!=EOF) {
        int pos = ftell(fp)-1;
        lnCol.ln[pos] = ln;
        lnCol.col[pos] = col;
        if (ch=='\n') {
            ++ln;
            col=0;
        } else ++col;
    }
    rewind(fp);
    int cnt=0;
    char *p = ans->symTable, *oldP=p, *stP=p;
    Token tok;
    bool numFlag=1;
    int pos;
    while ((tok=readToken(fp, &p, pos, numFlag))!=TOK_EOF) {
        if (tok==TOK_ERROR) {
            printf("Scanning error on Ln: %d, Col: %d!", 1+lnCol.ln[pos], 1+lnCol.col[pos]);
            return -1;
        }
        if (tok==TOK_LIT_INT||tok==TOK_LIT_FLOAT||tok==TOK_IDENT) {
            numFlag=0;
        } else numFlag=1;
        ans->tokens[cnt] = tok;
        ans->tokenMap[cnt]=oldP-stP;
        ans->tokenLn[cnt] = lnCol.ln[pos];
        ans->tokenCol[cnt] = lnCol.col[pos];
        oldP=p;
        ++cnt;
    }
    ans->tokenSz=cnt;
    return 0;
} 
