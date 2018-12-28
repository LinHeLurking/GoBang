//
// Created by Nine_yota on 2018-09-29.
//

#include "statusBoard.h"

//status_board[i][j] -> oblique_line_sum[i+j][j]

//status_board[i][j] -> oblique_line_delta[i-j+BOARD_SIZE][j]

boardStatus status;
boardStatus dfs_status;
drop_record record[BOARD_SIZE * BOARD_SIZE + 5];

extern trie tr[TRIE_SIZE];
extern unsigned long long hash_key[2][BOARD_SIZE][BOARD_SIZE];
extern int subtree_height[CACHE_SIZE];
extern unsigned long long real_hash[CACHE_SIZE];
extern unsigned long long hash;

void __status_init(boardStatus *boardStatus1) {
    SET0(boardStatus1->board);
    SET0(boardStatus1->row_type);
    SET0(boardStatus1->col_type);
    SET0(boardStatus1->oblique_sum_type);
    SET0(boardStatus1->oblique_delta_type);
    SET0(boardStatus1->row_increment);
    SET0(boardStatus1->col_increment);
    SET0(boardStatus1->oblique_sum_increment);
    SET0(boardStatus1->oblique_delta_increment);
    SET0(boardStatus1->total_type);
    boardStatus1->steps = 0;
    boardStatus1->__WHITE = WHITE;
    boardStatus1->__BLACK = BLACK;
}

void status_init() {
    __status_init(&status);
    __status_init(&dfs_status);
    record[0].i = record[0].j = -1;
}


/*  return -1 if that position is occupied
 *  return 0 if pc==0, which means there's nothing to do
 *  return 1 -> success without ban
 *  return 2 -> success with ban
 * */

int add_piece(int i, int j, int player_side) {
    if (status.board[i][j] != VOID)
        return -1;
    status.board[i][j] = player_side;

    int check_code = dfs_add_piece(i, j, player_side);

    if (check_code != -1) {
        status.steps++;
        record[status.steps].i = i;
        record[status.steps].j = j;
        record[status.steps].player = player_side;
        if (is_ban()) {
            //printf("Ban found!\n");
            check_code = 2;
        }
    }

    return check_code;
}

/*  return -1 if that position is occupied
 *  return 1 -> successful
 * */
int dfs_add_piece(int i, int j, int player_side) {
    if (dfs_status.board[i][j] != VOID && player_side != VOID)
        return -1;
    dfs_status.board[i][j] = player_side;

    if (player_side == VOID) {
        dfs_status.steps--;
    }
    hash ^= hash_key[dfs_status.steps & 1][i][j];
    if (player_side != VOID) {
        dfs_status.steps++;
    }
    update_grade(i, j);
    return 1;
}

inline int32_t *board_acess(int32_t index1, int32_t index2, int32_t line_type) {
    if (line_type == row) {
        if (index1 >= 0 && index1 < BOARD_SIZE && index2 >= 0 && index2 < BOARD_SIZE)
            return &dfs_status.board[index1][index2];
    } else if (line_type == col) {
        if (index1 >= 0 && index1 < BOARD_SIZE && index2 >= 0 && index2 < BOARD_SIZE)
            return &dfs_status.board[index2][index1];
    } else {
        int32_t i;
        if (line_type == oblique_sum) {
            i = index1 - index2;
        } else {
            i = index2 + index1;
        }
        if (i >= 0 && i < BOARD_SIZE && index2 >= 0 && index2 < BOARD_SIZE)
            return &dfs_status.board[i][index2];
    }
    return NULL;
}
