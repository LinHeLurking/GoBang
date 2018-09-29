//
// Created by Nine_yota on 2018-09-29.
//

#include "interact.h"
#include "drawBoard.h"
#include "statusBoard.h"
#include "boardEvaluate.h"

#define OFFSET 1

char player_side[3][10] = {
        " black",
        " ",
        " white"
};

void play() {
    printf("Welcome to Amadeus GoBang game!\nPlease input the code of corresponding mode:\n");
    int mode = -1;
    while (mode == -1) {
        printf("Human vs. human: 0\nHuman vs. computer: 1\n");
        scanf("%d", &mode);
        switch (mode) {
            case HUMAN_VS_HUMAN:
                human_vs_human();
                break;
            case HUMAN_VS_COMPUTER:
                human_vs_computer();
                break;
            default:
                mode = -1;
                printf("Wrong code! input the right one!\n");
        }
    }
}

void human_vs_human() {
    int player = BLACK;
    while (true) {
        output_board();
        printf("Round for %s, input the position you want to place the piece\n", player_side[player + OFFSET]);
        int i, j;
        read_pos(&i, &j);
        add_piece(i, j, player);
        player = 0 - player;
        int win_status = winner_check();
        if (win_status == WHITE) {
            printf("The player of %s won.\n", player_side[WHITE + OFFSET]);
            break;
        } else if (win_status == BLACK) {
            printf("The player of %s won.\n", player_side[BLACK + OFFSET]);
        }
    }
}

void human_vs_computer() {

}

int read_pos(int *i, int *j) {
    *i = *j = -1;
    char input[10] = {};
    int times = 0;
    while (!((*i >= 0 && *i < BOARD_SIZE) && (*j >= 0 && *j < BOARD_SIZE))) {
        scanf("%s", input);
        for (int k = 0; k < 10; ++k) {
            if (input[k] >= '0' && input[k] < '0' + BOARD_SIZE) {
                *j = input[k] - '0';
            }
            if (input[k] >= 'A' && input[k] < 'A' + BOARD_SIZE) {
                *i = input[k] - 'A';
            }
        }
        if (times++ != 0) {
            printf("Invalid position, input again\n");
        }
    }
    return 0;
}

#undef OFFSET