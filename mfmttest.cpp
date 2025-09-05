#include <stdio.h>
#include "msc.h"
#include "mpa.h"
#include "mdefs.h"
#include "mfmt.h"
SCRes scResTest;
Node rt;
int main() {
    freopen("mfmttestOut.txt", "w", stdout);
    scRes=&scResTest;
    FILE *testFile = fopen("mfmttestIn.txt", "r");
    sc(testFile, scRes);
    fclose(testFile);
    parseSrc(&rt);
    printSrc(rt.son, 0);
    return 0;
}
