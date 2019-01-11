//
// Created by Nine_yota on 2018-09-29.
//

/*
 * this file contains functions that finish the interactive tasks.
 * play() check the mode(human vs human or human vs computer), then calls corresponding functions.
 * */

#include "interact.h"

#define COLOR_OFFSET 1
#define BUFFERSIZE 100

extern boardStatus status;


static char player_name[3][10] = {
        "BLACK",
        "",
        "WHITE"
};

static char interact_buffer[BUFFERSIZE];

void play() {
    // this is the exact main loop of the whole program.
    // this function would interact with the user, call proper functions and throw interactive errors.
#ifndef DEBUG_DRAW
    system("clear");
#endif
    print_line(50, 1);
    printf("Welcome to Amadeus GoBang game!\n");
    printf("\tWork of: Lei Ci\n");
    print_line(50, 1);
    printf("Please input the code of corresponding mode:\n");

    printf("\tHuman vs. human:%2d\n", HUMAN_VS_HUMAN);
    printf("\tHuman vs. computer:%2d\n", HUMAN_VS_COMPUTER);
#ifdef DEBUG_DRAW
    printf("\tComputer vs. computer:%2d\n", COMPUTER_VS_COMPUTER);
#endif
    printf("\tInput 'quit'(without quotes) to quit this game\n");
    print_line(50, 1);

    int mode;

    while (true) {
        scanf("%s", interact_buffer);
        if (strcmp(interact_buffer, "quit") == 0) {
            return;
        } else if (interact_buffer[0] >= '0' && interact_buffer[0] <= '1') {
            mode = interact_buffer[0] - '0';
            break;
        }
#ifdef DEBUG_DRAW
        else if (interact_buffer[0] == '2') {
            mode = interact_buffer[0] - '0';
            break;
        }
#endif
        else {
            throw_interact_error(1);
        }
    }
    if (mode == HUMAN_VS_HUMAN) {
        human_vs_human();
    } else if (mode == HUMAN_VS_COMPUTER) {
        human_vs_computer();
    } else if (mode == COMPUTER_VS_COMPUTER) {
        computer_vs_computer();
    }
}


void human_vs_computer() {
    // well the name tells you what this can do.
    int human_player;
    int computer_player;
    int order_check;
    print_line(50, 1);
    printf("Human first or computer first?\n");
    printf("\tHuman first:%2d\n", HUMAN_FIRST);
    printf("\tComputer first:%2d\n", COMPUTER_FIRST);
    printf("\tInput 'quit'(without quotes) to quit this game\n");
    print_line(50, 1);
    while (true) {
        scanf("%s", interact_buffer);
        if (interact_buffer[0] == '1') {
            order_check = HUMAN_FIRST;
            break;
        } else if (interact_buffer[0] == '2') {
            order_check = COMPUTER_FIRST;
            break;
        } else if (strcmp(interact_buffer, "quit") == 0) {
            return;
        } else {
            throw_interact_error(1);
        }
    }
    if (order_check == COMPUTER_FIRST) {
        human_player = WHITE;
        computer_player = BLACK;
        add_piece(7, 7, computer_player);
    } else {
        human_player = BLACK;
        computer_player = WHITE;
    }
#ifdef DEBUG_DRAW
    print_line(94, 1);
    printf("This is in the debug mode, so some massages about searching will be printed onto the console.\n");
    print_line(94, 1);
#endif

    output_board(1);
    while (true) {
        round_announcement(human_player);

        int i, j;
        int st = -1;
        while (st != 1) {
            read_pos(&i, &j);
            st = add_piece(i, j, human_player);
            if (st == 2) {
                printf("Ban!\n");
                return;
            } else if (st == -1) {
                printf("There is already a piece in this place!\n");
                round_announcement(human_player);
            }
        }
        int win_status = winner_check();
        if (win_status == WHITE) {
            output_board(1);
            printf("The player of %s won.\n", player_name[WHITE + COLOR_OFFSET]);
            return;
        } else if (win_status == BLACK) {
            output_board(1);
            printf("The player of %s won.\n", player_name[BLACK + COLOR_OFFSET]);
            return;
        }
        drop_choice choice = deepening_search(computer_player, DFS_MAX_DEPTH);
        st = add_piece(choice.i, choice.j, computer_player);

        if (st == 2) {
            output_board(1);
            printf("Ban found! The player of white won.\n");
            output_board(0);
            break;
        } else if (st == -1) {
            printf("Search error!\n");
        } else {
            output_board(1);
        }
        win_status = winner_check();
        if (win_status == WHITE) {
            output_board(1);
            printf("The player of %s won.\n", player_name[WHITE + COLOR_OFFSET]);
            break;
        } else if (win_status == BLACK) {
            output_board(1);
            printf("The player of %s won.\n", player_name[BLACK + COLOR_OFFSET]);
            break;
        }
    }
}

void human_vs_human() {
    int player = BLACK;
    while (true) {
        output_board(1);
        round_announcement(player);
        int i, j;
        int st = -1;
        while (st == -1) {
            read_pos(&i, &j);
            st = add_piece(i, j, player);
            if (st == 2) {
                output_board(1);
                printf("Ban found! The player of white won.\n");
                return;
            } else if (st == -1) {
                printf("There is already a piece in this place!\n");
                round_announcement(player);
            }
        }

        int win_status = winner_check();
        if (win_status == WHITE) {
            output_board(1);
            printf("The player of %s won.\n", player_name[WHITE + COLOR_OFFSET]);
            break;
        } else if (win_status == BLACK) {
            output_board(1);
            printf("The player of %s won.\n", player_name[BLACK + COLOR_OFFSET]);
            break;
        }
        player = 0 - player;
    }
}

int read_pos(int *i, int *j) {
    // read the position input from the terminal and throw error if got invalid input.
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
            printf("Got input: i:%2d j:%2d\n", (*i), (*j));

            throw_interact_error(1);

            *i = *j = -1;
            memset(input, 0, sizeof(input));
            continue;
        }
    }
    return 0;
}


void computer_vs_computer() {
    // this is just for fun and would be invisible if the macro DEBUG_DRAW is not defined.
    // it turns out that results would not be good. skipping it is enough. LOL!
    printf("This is used only for test\n");
    printf("Input search depth for WHITE and BLACK in order:\n");
    int white_d = -1, black_d = -1;

    scanf("%s", interact_buffer);
    for (int i = 0; i < BUFFERSIZE; ++i) {
        if (interact_buffer[i] != '\0')
            white_d = interact_buffer[i] - '0';
    }
    scanf("%s", interact_buffer);
    for (int i = 0; i < BUFFERSIZE; ++i) {
        if (interact_buffer[i] != '\0')
            black_d = interact_buffer[i] - '0';
    }

    add_piece(7, 7, BLACK);
    output_board(0);

    int player = WHITE;
    int st;
    int winner_status;

    while (true) {
        printf("round for WHITE\n");
        drop_choice c = deepening_search(player, white_d);
        st = add_piece(c.i, c.j, player);
        if (st == -1) {
            printf("ERROR!\n");
            return;
        }
        output_board(0);
        if ((winner_status = winner_check()) == WHITE) {
            printf("The player of %s won.\n", player_name[WHITE + COLOR_OFFSET]);
            return;
        } else if (winner_status == BLACK) {
            printf("The player of %s won.\n", player_name[BLACK + COLOR_OFFSET]);
            return;
        }
        printf("round for BLACK\n");
        c = deepening_search(0 - player, black_d);
        st = add_piece(c.i, c.j, 0 - player);
        output_board(0);
        if (st == 2) {
            printf("Ban!\n");
            printf("The player of %s won.\n", player_name[WHITE + COLOR_OFFSET]);
            return;
        } else if (st == -1) {
            printf("ERROR!\n");
            printf("Invalid choice in %c%d", c.j + 'A', 15 - c.i);
            return;
        }
        if ((winner_status = winner_check()) == WHITE) {
            printf("The player of %s won.\n", player_name[WHITE + COLOR_OFFSET]);
            return;
        } else if (winner_status == BLACK) {
            printf("The player of %s won.\n", player_name[BLACK + COLOR_OFFSET]);
            return;
        }
    }
}

inline void round_announcement(int player) {
    // you know that writing repeatedly would be painful so that is why this function exists.
    printf("Round for %s, input the position you want to place the piece.\n", player_name[player + COLOR_OFFSET]);
    printf("Positions such as h8, H8, 8h, and 8H all could be accepted.\n");
    printf("Input 'quit'(without quotes) to quit this game.\n");
}

void throw_interact_error(int to_continue) {
    printf("Invalid input!\n");
    if (to_continue) {
        printf("Input again.\n");
    } else {
        printf("Program ends\n");
    }
}

void print_line(int cnt, int with_a_newline) {
    for (int i = 0; i < cnt; ++i) {
        printf("~");
    }
    if (with_a_newline) {
        printf("\n");
    }
}

#undef COLOR_OFFSET