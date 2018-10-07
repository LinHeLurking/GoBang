//
// Created by Nine_yota on 2018-10-07.
//

#include "statusSearch.h"

extern boardStatus status;
extern boardStatus dfs_status;

/*
 *  DFS starts here
 * */
#define MAX_POS 225

void generate_possible_pos(drop_choice *drop_choice1, int *num) {
    *num = 0;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (dfs_status.board[i][j] == VOID) {
                drop_choice1[*num].i = i;
                drop_choice1[*num].j = j;
                ++(*num);
            }
        }
    }
}


//remember that the larger the grade is, the better the status is for WHITE. and vice versa.
drop_choice alpha_beta_dfs(int search_player_side, int search_depth) {
    drop_choice result;
    result.i = result.j = 0;
    result.grade = 0;

    if (search_depth == 0) {
        result.grade = grade_estimate(search_player_side) + grade_estimate(0 - search_player_side);
        return result;
    }
    drop_choice drop_choice1[MAX_POS];
    int pos_num = 0;
    //generate proper places to drop a piece
    generate_possible_pos(drop_choice1, &pos_num);

    //traverse the proper places
    if (search_player_side == WHITE) {
#ifdef DFS_BOARD_DEBUG
        printf("dfs board:\n");
        dfs_output_board();
        GRADE_DEBUG
#endif
        result.grade = 0 - INF;
        for (int k = 0; k < pos_num; ++k) {
            dfs_add_piece(drop_choice1[k].i, drop_choice1[k].j, WHITE);

            drop_choice tmp;
            tmp = alpha_beta_dfs(0 - search_player_side, search_depth - 1);
            tmp.i = drop_choice1[k].i, tmp.j = drop_choice1[k].j;
            if (tmp.grade > result.grade) {
                result = tmp;
            }
            dfs_add_piece(tmp.i, tmp.j, VOID);
        }
        return result;
    } else {
#ifdef DFS_BOARD_DEBUG
        printf("dfs board:\n");
        dfs_output_board();
        GRADE_DEBUG
#endif
        result.grade = INF;
        for (int k = 0; k < pos_num; ++k) {
            dfs_add_piece(drop_choice1[k].i, drop_choice1[k].j, BLACK);
            drop_choice tmp;
            tmp = alpha_beta_dfs(0 - search_player_side, search_depth - 1);
            tmp.i = drop_choice1[k].i, tmp.j = drop_choice1[k].j;
            if (tmp.grade < result.grade) {
                result = tmp;
            }
            dfs_add_piece(tmp.i, tmp.j, VOID);
        }
        return result;
    }
}

