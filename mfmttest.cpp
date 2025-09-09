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
    if (sc(testFile, scRes)==-1) {
        fclose(testFile);
        return 0;
    }
    if (parseSrc(&rt))
        printSrc(rt.son, 0);
    return 0;
}
