/*
 * status means the status of the whole board, which includes pieces and their places in the board,
 * piece patterns of the board and the information of each void place.
 * the board is split into two parts, one is used for the real status and the other is for dfs.
 *
 * record is an array that stores the choice of each player from the beginning.
 * information in the record will be updated after each dfs drop is placed, which means piece dropping during dfs
 * has effect on the record.
 * */

#include "statusBoard.h"


boardStatus status;
boardStatus dfs_status;
drop_record record[BOARD_SIZE * BOARD_SIZE + 5];

extern trie tr[TRIE_SIZE];

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
    SET0(boardStatus1->total_increment);
    boardStatus1->steps = 0;
}

void status_init() {
    // call respond function to finish the initialization of the status_boards of real status and dfs status.
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
        // pay attention that is_ban() is only valid right after the piece is dropped.
        // calling is_ban() at other moments makes no sense.
        if (is_ban()) {
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
    if (player_side != VOID) {
        dfs_status.steps++;
    }
    update_grade(i, j);
    return 1;
}

