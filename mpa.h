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
const char nodeTypeName [][20] = {
    "empty", "SRC", "EXT_DEF", "EXT_VAR_DEF", "type", "int", "float", "char", 
    "VAR_SEQ", "FUNC_DEF", "PARAMS", "PARAM", "STMT", "IF_STMT", "IF_ELSE_STMT", "WHILE_STMT", "DO_WHILE_STMT",
    "FOR_STMT", "RETURN_STMT", "COMP_STMT", "LOCAL_VAR_DEFS", "LOCAL_VAR_DEF", "STMTS", 
    "EXPR", "EXPR_OPT", "OR_EXPR", "OR_OR_EXPR", "AND_EXPR", "AND", "COND_EXPR",
    "EQ", "NEQ", "REL_EXPR", "LT", "GT", "LEQ",
    "GEQ", "ADD_EXPR", "ADD", "SUB", "FAC", "MUL", "DIV", "MOD", "atom expr", "id", "lit int",
    "lit char", "lit float", "lit string", "func call", "ARGS", "ASSIGN", "NEG", "NOT"
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
