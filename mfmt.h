#ifndef MFMT_H
#define MFMT_H
#include "mpa.h"
extern SCRes *scRes;
void printExpr(Node *node, int dep);
void printArgs(Node *node, int dep);
void printAtomExpr(Node *node, int dep);
void printType(Node *node, int dep);
void printFac(Node *node, int dep);
void printAddExpr(Node *node, int dep);
void printRelExpr(Node *node, int dep);
void printCondExpr(Node *node, int dep);
void printAndExpr(Node *node, int dep);
void printOrExpr(Node *node, int dep);
void printExprOpt(Node *node, int dep);
void printCompStmt(Node *node, int dep);
void printStmt(Node *node, int dep);
void printStmts(Node *node, int dep);
void printId(Node *node, int dep);
void printParam(Node *node, int dep);
void printParams(Node *node, int dep);
void printVarSeq(Node *node, int dep);
void printLocalVarDef(Node *node, int dep);
void printLocalVarDefs(Node *node, int dep);
void printExtVarDef(Node *node, int dep);
void printFuncDef(Node *node, int dep);
void printExtDef(Node *node, int dep);
void printSrc(Node *node, int dep);
#endif
