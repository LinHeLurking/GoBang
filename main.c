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
}

void test() {
    int test_status[BOARD_SIZE][BOARD_SIZE] = {
            {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            add_piece(i, j, test_status[i][j]);
        }
    }
    printf("status board:\n");
    output_board();
    //printf("dfs board:\n");
    //dfs_output_board();
    int win = winner_check();
    printf("Test result: ");
    switch (win) {
        case 1:
            printf("white wins\n");
            break;
        case -1:
            printf("black wins\n");
            break;
        default:
            printf("no one wins\n");
    }
    GRADE_DEBUG
}


int main() {
    init();
    play();
    //test();

    return 0;
}