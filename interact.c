//
// Created by Nine_yota on 2018-09-29.
//

#include "interact.h"

#define COLOR_OFFSET 1

extern boardStatus status;


char player_side[3][10] = {
        "black",
        "",
        "white"
};

void play() {
#ifndef DEBUG_DRAW
    system("clear");
#endif

    printf("Welcome to Amadeus GoBang game!\nPlease input the code of corresponding mode:\n");

    int mode = -1;
    char tmp;
    while (mode == -1) {

        printf("Human vs. human: 0\nHuman vs. computer: 1\n");
        scanf("%c", &tmp);
        //printf("tmp:%c", tmp);
        if (tmp == 'q')exit(0);
        mode = tmp - '0';
        //printf("mode:%d", mode);
        switch (mode) {
            case HUMAN_VS_HUMAN:
                human_vs_human();
                break;
            case HUMAN_VS_COMPUTER:
                human_vs_computer();
                break;
            default:
                mode = -1;
                printf("Wrong code(%d)! input the right one!\n", tmp);
        }

    }
}


void human_vs_computer() {
    int human_player = BLACK;
    int computer_player = WHITE;
    int order_check = HUMAN_FIRST;

    printf("Human first or computer first?\nHuman first: 1\nComputer first: 2\n");
    scanf("%d", &order_check);
    if (order_check == COMPUTER_FIRST) {
        human_player = WHITE;
        computer_player = BLACK;
        add_piece(7, 7, computer_player);
    }
    output_board(1);


#ifdef PRUNE_DEBUG
    extern int prune_cnt;
#endif

    while (true) {

        printf("\n\nRound for %s, input the position you want to place the piece\n",
               player_side[human_player + COLOR_OFFSET]);
        printf("Input 'quit'(without quotes) to quit this game\n");


        int i, j;
        read_pos(&i, &j);
        int st = -1;
        int cnt = 0;
        while (st == -1) {
            if (cnt++ != 0) {
                i = j = -1;

                printf("There is already a piece in this place!\n");
                printf("Round for %s, input the position you want to place the piece\n",
                       player_side[human_player + COLOR_OFFSET]);

                while (!(i >= '0' && i <= '0' + BOARD_SIZE && j >= '0' && j <= '0' + BOARD_SIZE)) {
                    read_pos(&i, &j);
                }
            }
            st = add_piece(i, j, human_player);
            if (st == 2) {
                output_board(1);
                printf("Ban found! The player of white won.\n");
                //printf("Press any key to quit\n");
                //getchar();
                return;
            }
        }
        int win_status = winner_check();
        if (win_status == WHITE) {
            output_board(1);
            printf("The player of %s won.\n", player_side[WHITE + COLOR_OFFSET]);
            //printf("Press any key to quit\n");
            //getchar();
            return;
        } else if (win_status == BLACK) {
            output_board(1);
            printf("The player of %s won.\n", player_side[BLACK + COLOR_OFFSET]);
            //printf("Press any key to quit\n");
            //getchar();
            return;
        }
        drop_choice choice = alpha_beta_dfs(computer_player, DFS_MAX_DEPTH, 0 - INF, INF);
        st = add_piece(choice.i, choice.j, computer_player);

        output_board(1);

        if (st == -1) {
            printf("Search error!\n");
            //exit(-1);
        }

#ifdef PRUNE_DEBUG
        prune_cnt = 0;
#endif

        win_status = winner_check();
        if (win_status == WHITE) {
            output_board(1);
            printf("The player of %s won.\n", player_side[WHITE + COLOR_OFFSET]);
            break;
        } else if (win_status == BLACK) {
            output_board(1);
            printf("The player of %s won.\n", player_side[BLACK + COLOR_OFFSET]);
            break;
        }
    }
}

void human_vs_human() {
    int player = BLACK;
    while (true) {
        output_board(1);
#ifdef DEBUG_DRAW
        printf("The grade estimate:\nBLACK: %"PRId64"\n", grade_estimate());
#endif
        printf("Round for %s, input the position you want to place the piece\n", player_side[player + COLOR_OFFSET]);
        printf("Input 'quit'(without quotes) to quit this game\n");
        int i, j;
        read_pos(&i, &j);
        int st = -1;
        int cnt = 0;
        while (st == -1) {
            if (cnt++ != 0) {
                printf("There is already a piece in this place!\n");
                printf("Round for %s, input the position you want to place the piece\n",
                       player_side[player + COLOR_OFFSET]);
                read_pos(&i, &j);
            }
            st = add_piece(i, j, player);
            if (st == 2) {

                output_board(1);
                printf("Ban found! The player of white won.\n");
                //printf("Press any key to quit\n");
                //getchar();
                return;
            }
        }

        int win_status = winner_check();
        if (win_status == WHITE) {
            output_board(1);
            printf("The player of %s won.\n", player_side[WHITE + COLOR_OFFSET]);
            //printf("Press any key to quit\n");
            //getchar();
            return;
        } else if (win_status == BLACK) {
            output_board(1);
            printf("The player of %s won.\n", player_side[BLACK + COLOR_OFFSET]);
            //printf("Press any key to quit\n");
            //getchar();
            return;
        }
        player = 0 - player;
    }
}

int read_pos(int *i, int *j) {
    char input[10] = {};
    *i = *j = -1;
    while (true) {
        scanf("%s", input);
        if (input[0] == 'q' && input[1] == 'u' && input[2] == 'i' && input[3] == 't') {
            exit(0);
        }
        for (int m = 0; m < 10; ++m) {
            if (input[m] >= '0' && input[m] <= '9') {
                *i = ((*i) == -1 ? 0 : *i) * 10 + input[m] - '0';
            }
            if (*j == -1) {
                if (input[m] >= 'A' && input[m] <= 'O')*j = input[m] - 'A';
                else if (input[m] >= 'a' && input[m] <= 'o')*j = input[m] - 'a';
            }
        }
        if (*i >= 1 && *i <= BOARD_SIZE && *j >= 0 && *j < BOARD_SIZE) {
            *i = 15 - *i;
            break;
        } else {
            printf("i:%d j:%d", (*i), (*j));

            printf("Invalid position, input again!\n");

            *i = *j = -1;
            memset(input, 0, sizeof(input));
            continue;
        }
    }
    return 0;
}

#undef COLOR_OFFSET