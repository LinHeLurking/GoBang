//
// Created by Nine_yota on 2018-09-29.
//

#include "interact.h"

#define OFFSET 1

//extern int best_i, best_j;

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


void human_vs_computer() {
    int human_player = BLACK;
    int computer_player = WHITE;
    output_board();

    while (true) {

        printf("Round for %s, input the position you want to place the piece\n", player_side[human_player + OFFSET]);
        int i, j;
        read_pos(&i, &j);
        int st = -1;
        int cnt = 0;
        while (st == -1) {
            if (cnt++ != 0) {
                printf("There is already a piece in this place!\n");
                printf("Round for %s, input the position you want to place the piece\n",
                       player_side[human_player + OFFSET]);
                read_pos(&i, &j);
            }
            st = add_piece(i, j, human_player);
        }
        //output_board();
        int win_status = winner_check();
        if (win_status == WHITE) {
            output_board();
            printf("The player of%s won.\n", player_side[WHITE + OFFSET]);
            break;
        } else if (win_status == BLACK) {
            output_board();
            printf("The player of%s won.\n", player_side[BLACK + OFFSET]);
            break;
        }

        drop_choice choice = alpha_beta_dfs(computer_player, DFS_DEPTH);
        st = add_piece(choice.i, choice.j, computer_player);
        output_board();
        //dfs_output_board();
        if (st == -1) {
            printf("Search error!\n");
            exit(-1);
        }

#ifdef DEBUG
        printf("The grade estimate:\nBLACK: %d\nWHITE: %d\n", grade_estimate(BLACK), grade_estimate(WHITE));
        printf("dfs grade: %d\n", choice.grade);
#endif

        win_status = winner_check();
        if (win_status == WHITE) {
            output_board();
            printf("The player of%s won.\n", player_side[WHITE + OFFSET]);
            break;
        } else if (win_status == BLACK) {
            output_board();
            printf("The player of%s won.\n", player_side[BLACK + OFFSET]);
            break;
        }
    }
}

void human_vs_human() {
    int player = BLACK;
    while (true) {
        output_board();
#ifdef DEBUG
        printf("The grade estimate:\nBLACK: %d\nWHITE: %d\n", grade_estimate(BLACK), grade_estimate(WHITE));
#endif
        printf("Round for %s, input the position you want to place the piece\n", player_side[player + OFFSET]);
        int i, j;
        read_pos(&i, &j);
        int st = -1;
        int cnt = 0;
        while (st == -1) {

            if (cnt++ != 0) {
                printf("There is already a piece in this place!\n");
                printf("Round for %s, input the position you want to place the piece\n", player_side[player + OFFSET]);
                read_pos(&i, &j);
            }
            st = add_piece(i, j, player);
        }

        int win_status = winner_check();
        if (win_status == WHITE) {
            output_board();
            printf("The player of%s won.\n", player_side[WHITE + OFFSET]);
            break;
        } else if (win_status == BLACK) {
            output_board();
            printf("The player of%s won.\n", player_side[BLACK + OFFSET]);
            break;
        }
        player = 0 - player;
    }
}

int read_pos(int *i, int *j) {
    char input[10] = {};
    *i = *j = -1;
    while (true) {
        scanf("%s", input);
        for (int m = 0; m < 10; ++m) {
            if (input[m] >= '0' && input[m] <= '9') {
                *j = ((*j) == -1 ? 0 : *j) * 10 + input[m] - '0';
            }
            if (input[m] >= 'A' && input[m] <= 'O') {
                *i = input[m] - 'A';
            }

        }
        if (*i >= 0 && *i <= BOARD_SIZE && *j >= 1 && *j <= BOARD_SIZE) {
            *j -= 1;
            break;
        } else {
            printf("Invalid position, input again!\n");
            *i = *j = -1;
            memset(input, 0, sizeof(input));
            continue;
        }
    }
    return 0;
}

#undef OFFSET