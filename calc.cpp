#include <stdio.h>
struct Token {
    enum {END, NUM, NL, ADD, SUB, MUL, DIV, LP, RP} type;
    int val;
} ;
// num-> -*[0-9]+
inline bool isDigit(char ch) {return '0'<=ch&&ch<='9';}
int readInt() {
    int ans=0;
    char ch;
    while (ch=getchar(), '0'<=ch&&ch<='9') ans=ans*10+ch-'0';
    ungetc(ch, stdin);
    return ans;
}
Token getToken() {
    char ch;
    while (ch=getchar(), ch==' ') ;
    if (ch==EOF) return {Token::END};
    if (ch=='\n') return {Token::NL};
    else if (ch=='-') {
        char cch=getchar();
        if (isDigit(cch)) {
            ungetc(ch, stdin);
            return {Token::NUM, -readInt()};
        } else return {Token::SUB};
    } else if ('0'<=ch&&ch<='9') {
        ungetc(ch, stdin);
        return {Token::NUM, readInt()};
    } else {
        switch (ch) {
        case '+': return {Token::ADD}; break;
        case '*': return {Token::MUL}; break;
        case '/': return {Token::DIV}; break;
        case '(': return {Token::LP}; break;
        case ')': return {Token::RP}; break;
        }
    }
}
// expr-> fac + expr | fac - expr | fac
// fac-> ( expr ) * fac | ( expr ) / fac | num * fac | num / fac | ( expr ) | num
inline bool match(Token tok) {return getToken().type==tok.type;}
bool fac(int dep) ;
bool expr(int dep) {
    for (int j=1; j<dep; ++j) printf("  ");
    puts("Expr:");
    if (!fac(dep+1)) return 0;
    char cch=getchar();
    if (cch!='+'&&cch!='-') {
        ungetc(cch, stdin);
        return 1;
    }
    for (int j=1; j<=dep; ++j) printf("  ");
    printf("Sym: %c\n", cch);
    if (!expr(dep)) return 0;
    return 1;
}
bool fac(int dep) {
    for (int j=1; j<dep; ++j) printf("  ");
    puts("Fac:");
    Token tok=getToken();
    if (tok.type==Token::LP) {
        if (!expr(dep+1)) return 0;
        if (!match({Token::RP})) return 0;
    } else if (tok.type==Token::NUM) {
        for (int j=1; j<=dep; ++j) printf("  ");
        printf("Num: %d\n", tok.val);
    } else return 0;
    char cch=getchar();
    if (cch!='*'&&cch!='/') {ungetc(cch, stdin); return 1;}
    for (int j=1; j<=dep; ++j) printf("  ");
    printf("Sym: %c\n", cch);
    if (!fac(dep+1)) return 0;
    return 1;
}
int main() {
    expr(1);
    return 0;
}
