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
        puts(nodeTypeName[n->typ]);
    }
}
Node rt;
int main() {
    freopen("mpatestOut.txt", "w", stdout);
    scRes=&scResTest;
    FILE *testFile = fopen("mpatestIn.txt", "r");
    sc(testFile, scRes);
    fclose(testFile);
    parseSrc(&rt);
    rt.son->preOrdTraverse(printNode, 0);
    return 0;
}
