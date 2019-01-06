#include "icld.h"
#include "statusBoard.h"
#include "drawBoard.h"
#include "boardEvaluate.h"
#include "interact.h"
#include "mathFunc.h"
#include "AC_Automachine.h"
#include "Zoribrist_hash.h"

void init() {
    AC_build();
    status_init();
    hash_init();
    evaluate_init();
    search_init();
    time_init();
}

int main() {
    init();
    play();
    return 0;
}