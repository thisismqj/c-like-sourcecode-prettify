#ifndef MPA_H
#define MPA_H
#include "mdefs.h"
#include "msc.h"
enum NodeType {empty, src, extDef, extVarDef, type, typeInt, typeFloat, typeChar, varSeq, funcDef, params, param, stmt, 
                ifStmt, ifElseStmt, whileStmt, doWhileStmt, forStmt, returnStmt, 
                compStmt, localVarDefs, localVarDef, stmts, expr, exprOpt, orExpr, orOrExpr, andExpr, andAndExpr, 
                condExpr, condEqExpr, condNeqExpr, relExpr, relLtExpr, relGtExpr, relLeqExpr, relGeqExpr, 
                addExpr, addAddExpr, subAddExpr, fac, mulFac, divFac, modFac, atomExpr, 
                id, litInt, litChar, litFloat, litString, funcCall, args, assignExpr, neg, _not};
/*
const char nodeTypeName [][20] = {
    "empty", "SRC", "EXT_DEF", "EXT_VAR_DEF", "type", "int", "float", "char", 
    "VAR_SEQ", "FUNC_DEF", "PARAMS", "PARAM", "STMT", "IF_STMT", "IF_ELSE_STMT", "WHILE_STMT", "DO_WHILE_STMT",
    "FOR_STMT", "RETURN_STMT", "COMP_STMT", "LOCAL_VAR_DEFS", "LOCAL_VAR_DEF", "STMTS", 
    "EXPR", "EXPR_OPT", "OR_EXPR", "OR_OR_EXPR", "AND_EXPR", "AND", "COND_EXPR",
    "EQ", "NEQ", "REL_EXPR", "LT", "GT", "LEQ",
    "GEQ", "ADD_EXPR", "ADD", "SUB", "FAC", "MUL", "DIV", "MOD", "atom expr", "id", "lit int",
    "lit char", "lit float", "lit string", "func call", "ARGS", "ASSIGN", "NEG", "NOT"
};
*/
const char nodeTypeName [][40] = {
    "空", "源程序", "外部定义", "外部变量定义", "类型", "int", "float", "char", 
    "变量序列", "函数定义", "形参列表", "形参", "语句", "if语句", "if-else语句", "while语句", "do-while语句",
    "for语句", "返回语句", "复合语句", "局部变量定义列表", "局部变量定义", "语句列表", 
    "表达式", "可选表达式", "或表达式", "或表达式", "且表达式", "且表达式", "条件表达式",
    "等于", "不等于", "关系表达式", "小于", "大于", "小于等于",
    "大于等于", "加表达式", "加", "减", "因式", "乘", "除", "取余", "原子表达式", "标识符", "整形字面量",
    "字符型字面量", "浮点型字面量", "字符串型字面量", "函数调用", "实参列表", "赋值表达式", "负", "非"
};
extern SCRes *scRes;
struct Node {
    NodeType typ;
    int tokenPos;
    Node *son=0, *bro=0, *fa=0;
    Node();
    ~Node();
    void addChild(Node *chd);
    Node *newChild();
    void preOrdTraverse(void (*fun)(Node *n, int d), int dep);
    void postOrdTraverse(void (*fun)(Node *n, int d), int dep);
};
bool parseExpr(Node *fa);
bool parseArgs(Node *fa);
bool parseAtomExpr(Node *fa);
bool parseType(Node *fa);
bool parseFac(Node *fa);
bool parseAddExpr(Node *fa);
bool parseRelExpr(Node *fa);
bool parseCondExpr(Node *fa);
bool parseAndExpr(Node *fa);
bool parseOrExpr(Node *fa);
bool parseExprOpt(Node *fa);
bool parseCompStmt(Node *fa);
bool parseStmt(Node *fa);
bool parseStmts(Node *fa);
bool parseId(Node *fa);
bool parseParam(Node *fa);
bool parseParams(Node *fa);
bool parseVarSeq(Node *fa);
bool parseLocalVarDef(Node *fa);
bool parseLocalVarDefs(Node *fa);
bool parseExtVarDef(Node *fa);
bool parseFuncDef(Node *fa);
bool parseExtDef(Node *fa);
bool parseSrc(Node *fa);

#endif
