#include "icld.h"
#include "statusBoard.h"
#include "drawBoard.h"
#include "boardEvaluate.h"
#include "interact.h"
#include "mathFunc.h"

void init() {
    AC_build();
    status_init();
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

void math_test() {
    int a[10] = {123, 1235, 123, 78456, 2345, 4586, 235, 345678, 1234, 23456};
    int_sort(a, 10);
    for (int i = 0; i < 10; ++i) {
        printf("%d  ", a[i]);
    }
    printf("\n");
}

int main() {
    init();
    play();
    //test();
    //math_test();

    return 0;
}