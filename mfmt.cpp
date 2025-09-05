#include "mfmt.h"
#include <stdio.h>
static void printIndent(int dep) {
    while (dep--) printf("    ");
}
void printExpr(Node *node, int dep) {
    if (node->typ==NodeType::assignExpr) {
        printOrExpr(node->son, dep);
        printf(" = ");
        printExpr(node->son->bro, dep);
    } else {
        printOrExpr(node->son, dep);
    }
}
void printOrExpr(Node *node, int dep) {
    if (node->typ==NodeType::orOrExpr) {
        printOrExpr(node->son, dep);
        printf(" || ");
        printAndExpr(node->son->bro, dep);
    } else {
        printAndExpr(node->son, dep);
    }
}
void printAndExpr(Node *node, int dep) {
    if (node->typ==NodeType::andAndExpr) {
        printAndExpr(node->son, dep);
        printf(" && ");
        printCondExpr(node->son->bro, dep);
    } else {
        printCondExpr(node->son, dep);
    }
}
void printCondExpr(Node *node, int dep) {
    if (node->typ==NodeType::condEqExpr) {
        printCondExpr(node->son, dep);
        printf(" == ");
        printRelExpr(node->son->bro, dep);
    } else if (node->typ==NodeType::condNeqExpr) {
        printCondExpr(node->son, dep);
        printf(" != ");
        printRelExpr(node->son->bro, dep);
    } else {
        printRelExpr(node->son, dep);
    }
}
void printRelExpr(Node *node, int dep) {
    if (node->typ!=NodeType::relExpr) {
        printRelExpr(node->son, dep);
        switch (node->typ) {
        case NodeType::relLeqExpr:
            printf(" <= ");
            break;
        case NodeType::relLtExpr:
            printf(" < ");
            break;
        case NodeType::relGeqExpr:
            printf(" >= ");
            break;
        case NodeType::relGtExpr:
            printf(" > ");
            break;
        }
        printAddExpr(node->son->bro, dep);
    } else {
        printAddExpr(node->son, dep);
    }
}
void printAddExpr(Node *node, int dep) {
    if (node->typ!=NodeType::addExpr) {
        printAddExpr(node->son, dep);
        switch (node->typ) {
        case NodeType::addAddExpr:
            printf(" + ");
            break;
        case NodeType::subAddExpr:
            printf(" - ");
            break;
        }
        printFac(node->son->bro, dep);
    } else {
        printFac(node->son, dep);
    }
}
void printFac(Node *node, int dep) {
    if (node->typ!=NodeType::fac) {
        printFac(node->son, dep);
        switch (node->typ) {
        case NodeType::mulFac:
            printf(" * ");
            break;
        case NodeType::divFac:
            printf(" / ");
            break;
        }
        printAtomExpr(node->son->bro, dep);
    } else {
        printAtomExpr(node->son, dep);
    }
}
void printAtomExpr(Node *node, int dep) {
    int locP = scRes->tokenMap[node->tokenPos];
    switch (node->typ) {
    case NodeType::id:
        printf("%s", &scRes->symTable[locP]);
        break;
    case NodeType::neg:
        printf("-");
        printAtomExpr(node->son, dep);
        break;
    case NodeType::_not:
        printf("!");
        printAtomExpr(node->son, dep);
        break;
    case NodeType::litInt:
        printf("%d", *(int *)&scRes->symTable[locP]);
        break;
    case NodeType::litChar:
        printf("%c", scRes->symTable[locP]);
        break;
    case NodeType::litFloat:
        printf("%f", *(float *)&scRes->symTable[locP]);
        break;
    case NodeType::litString:
        printf("\"%s\"", &scRes->symTable[locP]);
        break;
    case NodeType::funcCall:
        printId(node->son, dep);
        printf("(");
        printArgs(node->son->bro, dep);
        printf(")");
        break;
    case NodeType::atomExpr:
        printf("(");
        printExpr(node->son, dep);
        printf(")");
        break;
    }
}
void printType(Node *node, int dep) {
    switch (node->typ) {
    case NodeType::typeInt:
        printf("int");
        break;
    case NodeType::typeChar:
        printf("char");
        break;
    case NodeType::typeFloat:
        printf("float");
        break;
    }
}
void printArgs(Node *node, int dep) {
    Node *p = node->son;
    while (p) {
        printExpr(p, dep);
        if (p->bro) printf(", ");
        p=p->bro;
    }
}
void printExprOpt(Node *node, int dep) {
    if (node->son) {
        printExpr(node->son, dep);
    }
}
void printCompStmt(Node *node, int dep) {
    printf("{\n");
    printLocalVarDefs(node->son, dep+1);
    printStmts(node->son->bro, dep+1);
    printIndent(dep);
    printf("}\n");
}
void printStmts(Node *node, int dep) {
    Node *p = node->son;
    while (p) {
        printStmt(p, dep);
        p=p->bro;
    }
}
void printStmt(Node *node, int dep) {
    static bool fInd=1;
    switch (node->typ) {
    case NodeType::stmt:
        printIndent(dep);
        printExpr(node->son, dep);
        printf(";\n");
        break;
    case NodeType::compStmt:
        printIndent(dep);
        printCompStmt(node->son, dep);
        break;
    case NodeType::returnStmt:
        printIndent(dep);
        printf("return ");
        printExpr(node->son, dep);
        printf(";\n");
        break;
    case NodeType::ifStmt:
        if (fInd) printIndent(dep);
        printf("if (");
        printExpr(node->son, dep);
        printf(")");
        if (node->son->bro->typ==NodeType::compStmt) {
            printf(" ");
            printCompStmt(node->son->bro, dep);
        } else {
            printf("\n");
            printStmt(node->son->bro, dep+1);
        }
        break;
    case NodeType::ifElseStmt:
        if (fInd) printIndent(dep);
        printf("if (");
        printExpr(node->son, dep);
        printf(")");
        if (node->son->bro->typ==NodeType::compStmt) {
            printf(" ");
            {
                Node *n = node->son->bro;
                printf("{\n");
                printLocalVarDefs(n->son, dep+1);
                printStmts(n->son->bro, dep+1);
                printIndent(dep);
                printf("}");
            }
            printf(" else ");
        } else {
            printf("\n");
            printStmt(node->son->bro, dep+1);
            printIndent(dep);
            printf("else ");
        }
        if (node->son->bro->bro->typ==NodeType::stmt||
            node->son->bro->bro->typ==NodeType::returnStmt) {
            printf("\n");
            printStmt(node->son->bro->bro, dep+1);
        } else {
            fInd=0;
            printStmt(node->son->bro->bro, dep);
            fInd=1;
        }
        break;
    case NodeType::whileStmt:
        if (fInd) printIndent(dep);
        printf("while (");
        printExpr(node->son, dep);
        printf(")");
        if (node->son->bro->typ==NodeType::compStmt) {
            printf(" ");
            printCompStmt(node->son->bro, dep);
        } else {
            printf("\n");
            printStmt(node->son->bro, dep+1);
        }
        break;
    case NodeType::doWhileStmt:
        if (fInd) printIndent(dep);
        printf("do ");
        if (node->son->typ==NodeType::compStmt) {
            {
                Node *n = node->son;
                printf("{\n");
                printLocalVarDefs(n->son, dep+1);
                printStmts(n->son->bro, dep+1);
                printIndent(dep);
                printf("}");
            }
        } else {
            printf("\n");
            printStmt(node->son->bro, dep+1);
        }
        printf(" while (");
        printExpr(node->son->bro, dep);
        printf(");\n");
        break;
    case NodeType::forStmt:
        if (fInd) printIndent(dep);
        printf("for (");
        printExprOpt(node->son, dep);
        printf("; ");
        printExprOpt(node->son->bro, dep);
        printf("; ");
        printExprOpt(node->son->bro->bro, dep);
        printf(")");
        Node *st = node->son->bro->bro->bro;
        if (st->typ==NodeType::compStmt) {
            printf(" ");
            printCompStmt(st, dep);
        } else {
            printf("\n");
            printStmt(st, dep+1);
        }
        break;
    }
}
void printId(Node *node, int dep) {
    printf("%s", &scRes->symTable[scRes->tokenMap[node->tokenPos]]);
}
void printParam(Node *node, int dep) {
    switch (node->son->typ) {
    case NodeType::typeInt:
        printf("int ");
        break;
    case NodeType::typeFloat:
        printf("float ");
        break;
    case NodeType::typeChar:
        printf("char ");
        break;
    }
    printId(node->son->bro, dep);
}
void printParams(Node *node, int dep) {
    Node *p = node->son;
    while (p) {
        printParam(p, dep);
        if (p->bro) printf(", ");
        p = p->bro;
    }
}
void printVarSeq(Node *node, int dep) {
    Node *p = node->son;
    while (p) {
        printId(p, dep);
        if (p->bro) printf(", ");
        p = p->bro;
    }
}
void printLocalVarDef(Node *node, int dep) {
    printIndent(dep);
    printType(node->son, dep);
    printf(" ");
    printVarSeq(node->son->bro, dep);
    printf(";\n");
}
void printLocalVarDefs(Node *node, int dep) {
    Node *p = node->son;
    while (p) {
        printLocalVarDef(p, dep);
        p = p->bro;
    }
}
void printExtVarDef(Node *node, int dep) {
    printType(node->son, dep);
    printf(" ");
    printVarSeq(node->son->bro, dep);
    printf(";\n");
}
void printFuncDef(Node *node, int dep) {
    printType(node->son, dep);
    printf(" ");
    printId(node->son->bro, dep);
    printf("(");
    printParams(node->son->bro->bro, dep);
    printf(") ");
    printCompStmt(node->son->bro->bro->bro, dep);
}
void printExtDef(Node *node, int dep) {
    switch (node->son->typ) {
    case NodeType::extVarDef:
        printExtVarDef(node->son, dep);
        break;
    case NodeType::funcDef:
        printFuncDef(node->son, dep);
        break;
    }
}
void printSrc(Node *node, int dep) {
    Node *p = node->son;
    while (p) {
        printExtDef(p, dep);
        p = p->bro;
    }
}
