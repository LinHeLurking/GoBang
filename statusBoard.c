//
// Created by Nine_yota on 2018-09-29.
//

#include "statusBoard.h"

//status_board[i][j] -> oblique_lines_1[i+j][j]

//status_board[i][j] -> oblique_lines_2[i-j+BOARD_SIZE][j]

boardStatus status;
boardStatus dfs_status;

void __status_init(boardStatus *boardStatus1) {
    memset(boardStatus1->board, 0, sizeof(boardStatus1->board));
    memset(boardStatus1->oblique_lines_1, 0, sizeof(boardStatus1->oblique_lines_1));
    memset(boardStatus1->oblique_lines_2, 0, sizeof(boardStatus1->oblique_lines_2));
    boardStatus1->last_i = boardStatus1->last_j = -1;
}

void status_init() {
    __status_init(&status);
    __status_init(&dfs_status);
}

/*  return -1 if that position is occupied
 *  return 0 if pc==0, which means there's nothing to do
 *  return 1 -> successful
 * */
int add_piece(int i, int j, int pc) {
    if (status.board[i][j] != VOID)
        return -1;
    status.board[i][j] = pc;
    status.oblique_lines_1[i + j][j] = pc;
    status.oblique_lines_2[i - j + BOARD_SIZE][j] = pc;
    status.last_i = i;
    status.last_j = j;
    return dfs_add_piece(i, j, pc);
}

/*  return -1 if that position is occupied
 *  return 1 -> successful
 * */
int dfs_add_piece(int i, int j, int piece_color) {
    if (dfs_status.board[i][j] != VOID && piece_color != VOID)
        return -1;
    dfs_status.board[i][j] = piece_color;
    dfs_status.oblique_lines_1[i + j][j] = piece_color;
    dfs_status.oblique_lines_2[i - j + BOARD_SIZE][j] = piece_color;
    dfs_status.last_i = i;
    dfs_status.last_j = j;
    return 1;
}