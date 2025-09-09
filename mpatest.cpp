#include <stdio.h>
#include "msc.h"
#include "mpa.h"
#include "mdefs.h"
void printIndent(int cnt) {
    while (cnt--) printf("  ");
}
SCRes scResTest;
void printNode(Node *n, int d) {
    printIndent(d);
    switch (n->typ) {
    case NodeType::id:
        printf("id: %s\n", &scRes->symTable[scRes->tokenMap[n->tokenPos]]);
        break;
    case NodeType::litInt:
        printf("lit int: %d\n", *(int *)&scRes->symTable[scRes->tokenMap[n->tokenPos]]);
        break;
    case NodeType::litChar:
        printf("lit char: %c\n", scRes->symTable[scRes->tokenMap[n->tokenPos]]);
        break;
    case NodeType::litFloat:
        printf("lit float: %f\n", *(float *)&scRes->symTable[scRes->tokenMap[n->tokenPos]]);
        break;
    case NodeType::litString:
        printf("lit string: %s\n", &scRes->symTable[scRes->tokenMap[n->tokenPos]]);
        break;
    default:
    /*
        switch (n->typ) {
        case NodeType::expr:
        case NodeType::exprOpt:
        case NodeType::orExpr:
        case NodeType::andExpr:
        case NodeType::andAndExpr:
        case NodeType::orOrExpr:
        case NodeType::fac:
        case NodeType::relExpr:
        case NodeType::relLtExpr:
        case NodeType::relGtExpr:
        case NodeType::relLeqExpr:
        case NodeType::relGeqExpr:
        case NodeType::condEqExpr:
        case NodeType::condExpr:
        case NodeType::addAddExpr:
        case NodeType::subAddExpr:
        case NodeType::condNeqExpr:
        case NodeType::addExpr:
        case NodeType::atomExpr:
        case NodeType::divFac:
        case NodeType::mulFac:
        case NodeType::modFac:
            if (n->bro) puts(nodeTypeName[n->typ]);
            break;
        default:
            puts(nodeTypeName[n->typ]);
        }
            */
        puts(nodeTypeName[n->typ]);
        break;
    }
}
void printNode2(Node *n, int d) {
    Node * p = n->son;
    switch (n->typ) {
    case NodeType::id:
        printIndent(d);
        printf("id: %s\n", &scRes->symTable[scRes->tokenMap[n->tokenPos]]);
        break;
    case NodeType::litInt:
        printIndent(d);
        printf("lit int: %d\n", *(int *)&scRes->symTable[scRes->tokenMap[n->tokenPos]]);
        break;
    case NodeType::litChar:
        printIndent(d);
        printf("lit char: %c\n", scRes->symTable[scRes->tokenMap[n->tokenPos]]);
        break;
    case NodeType::litFloat:
        printIndent(d);
        printf("lit float: %f\n", *(float *)&scRes->symTable[scRes->tokenMap[n->tokenPos]]);
        break;
    case NodeType::litString:
        printIndent(d);
        printf("lit string: %s\n", &scRes->symTable[scRes->tokenMap[n->tokenPos]]);
        break;
    default:

        switch (n->typ) {
        case NodeType::expr:
        case NodeType::exprOpt:
        case NodeType::orExpr:
        case NodeType::andExpr:
        case NodeType::andAndExpr:
        case NodeType::orOrExpr:
        case NodeType::fac:
        case NodeType::relExpr:
        case NodeType::relLtExpr:
        case NodeType::relGtExpr:
        case NodeType::relLeqExpr:
        case NodeType::relGeqExpr:
        case NodeType::condEqExpr:
        case NodeType::condExpr:
        case NodeType::addAddExpr:
        case NodeType::subAddExpr:
        case NodeType::condNeqExpr:
        case NodeType::addExpr:
        case NodeType::atomExpr:
        case NodeType::divFac:
        case NodeType::mulFac:
        case NodeType::modFac:
            if (p->bro) {
                printIndent(d);
                puts(nodeTypeName[n->typ]);
            } else --d;
            break;
        default:
            printIndent(d);
            puts(nodeTypeName[n->typ]);
            break;
        }
        while (p) {
            printNode2(p, d+1);
            p = p->bro;
        }
            
        /*
        puts(nodeTypeName[n->typ]);
        while (p) {
            printNode2(p, d+1);
            p = p->bro;
        }
        */
        break;
    }
}
Node rt;
int main() {
    freopen("mpatestOut.txt", "w", stdout);
    scRes=&scResTest;
    FILE *testFile = fopen("mpatestIn.txt", "r");
    if (sc(testFile, scRes)==-1) return 0;
    fclose(testFile);
    if (parseSrc(&rt))
        // rt.son->preOrdTraverse(printNode, 0);
        printNode2(rt.son, 0);
    return 0;
}
