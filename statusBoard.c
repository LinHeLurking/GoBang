//
// Created by Nine_yota on 2018-09-29.
//

#include "statusBoard.h"
#include "mathFunc.h"



//status_board[i][j] -> oblique_line_sum[i+j][j]

//status_board[i][j] -> oblique_line_delta[i-j+BOARD_SIZE][j]

boardStatus status;
boardStatus dfs_status;
drop_record record[BOARD_SIZE * BOARD_SIZE + 5];
extern trie tr[TRIE_SIZE];;

void __status_init(boardStatus *boardStatus1) {
    SET0(boardStatus1->board);
    SET0(boardStatus1->oblique_line_sum);
    SET0(boardStatus1->oblique_line_delta);
    SET0(boardStatus1->row_grade);
    SET0(boardStatus1->col_grade);
    SET0(boardStatus1->oblique_sum_grade);
    SET0(boardStatus1->oblique_delta_grade);
    //SET0(boardStatus1->total_grade);

    boardStatus1->last_i = boardStatus1->last_j = -1;
    boardStatus1->steps = 0;
}

void status_init() {
    __status_init(&status);
    __status_init(&dfs_status);
}


void update_line_grade_row(int row_index, int player_side) {
    int _grade = 0;
    int cur = 0;
    for (int k = -1; k <= BOARD_SIZE; ++k) {
        int ch = k == -1 || k == BOARD_SIZE ? 0 - player_side : dfs_status.board[row_index][k];
        while (tr[cur].trans[ch + OFFSET] == -1 && cur != 0) {
            cur = tr[cur].fail;
        }
        cur = tr[cur].trans[ch + OFFSET];
        cur = cur == -1 ? 0 : cur;
        int tmp = cur;
        while (tmp != 0 && tr[tmp].grade != 0) {
            if ((player_side == WHITE && tr[tmp].grade > 0) || (player_side == BLACK && tr[tmp].grade < 0)) {
                _grade += tr[tmp].grade;
            }
            tmp = tr[tmp].fail;
        }
    }
    //dfs_status.total_grade[PLAYER_IN_LINE] -= dfs_status.row_grade[PLAYER_IN_LINE][row_index];
    //dfs_status.total_grade[PLAYER_IN_LINE] += _grade;

    dfs_status.row_grade[PLAYER_IN_LINE][row_index] = _grade;
}

void update_line_grade_col(int col_index, int player_side) {
    int cur = 0;
    int _grade = 0;
    for (int k = -1; k <= BOARD_SIZE; ++k) {
        int ch = k == -1 || k == BOARD_SIZE ? 0 - player_side : dfs_status.board[k][col_index];
        while (tr[cur].trans[ch + OFFSET] == -1 && cur != 0) {
            cur = tr[cur].fail;
        }
        cur = tr[cur].trans[ch + OFFSET];
        cur = cur == -1 ? 0 : cur;
        int tmp = cur;
        while (tmp != 0 && tr[tmp].grade != 0) {
            if ((player_side == WHITE && tr[tmp].grade > 0) || (player_side == BLACK && tr[tmp].grade < 0)) {
                _grade += tr[tmp].grade;
            }
            tmp = tr[tmp].fail;
        }
    }
    //dfs_status.total_grade[PLAYER_IN_LINE] -= dfs_status.col_grade[PLAYER_IN_LINE][col_index];
    //dfs_status.total_grade[PLAYER_IN_LINE] += _grade;

    dfs_status.col_grade[PLAYER_IN_LINE][col_index] = _grade;
}

void update_line_grade_oblique_sum(int oblique_sum_index, int player_side) {
    int cur = 0;
    int _grade = 0;
    for (int k = -1; k <= BOARD_SIZE; ++k) {
        int ch = k == int_max(-1, oblique_sum_index - BOARD_SIZE) || k == int_min(oblique_sum_index + 1, BOARD_SIZE) ?
                 0 - player_side : dfs_status.oblique_line_sum[oblique_sum_index][k];
        while (tr[cur].trans[ch + OFFSET] == -1 && cur != 0) {
            cur = tr[cur].fail;
        }
        cur = tr[cur].trans[ch + OFFSET];
        cur = cur == -1 ? 0 : cur;
        int tmp = cur;
        while (tmp != 0 && tr[tmp].grade != 0) {
            if ((player_side == WHITE && tr[tmp].grade > 0) || (player_side == BLACK && tr[tmp].grade < 0)) {
                _grade += tr[tmp].grade;
            }
            tmp = tr[tmp].fail;
        }
    }
    //dfs_status.total_grade[PLAYER_IN_LINE] -= dfs_status.oblique_sum_grade[PLAYER_IN_LINE][oblique_sum_index];
    //dfs_status.total_grade[PLAYER_IN_LINE] += _grade;

    dfs_status.oblique_sum_grade[PLAYER_IN_LINE][oblique_sum_index] = _grade;
}

void update_line_grade_oblique_delta(int oblique_delta_index, int player_side) {
    int cur = 0;
    int _grade = 0;
    for (int k = -1; k <= BOARD_SIZE; ++k) {
        int ch =
                k == int_max(-1, -1 - oblique_delta_index) || k == int_min(BOARD_SIZE - oblique_delta_index, BOARD_SIZE)
                ? 0 - player_side : dfs_status.oblique_line_delta[oblique_delta_index + BOARD_SIZE][k];
        while (tr[cur].trans[ch + OFFSET] == -1 && cur != 0) {
            cur = tr[cur].fail;
        }
        cur = tr[cur].trans[ch + OFFSET];
        cur = cur == -1 ? 0 : cur;
        int tmp = cur;
        while (tmp != 0 && tr[tmp].grade != 0) {
            if ((player_side == WHITE && tr[tmp].grade > 0) || (player_side == BLACK && tr[tmp].grade < 0)) {
                _grade += tr[tmp].grade;
            }
            tmp = tr[tmp].fail;
        }
    }
    //dfs_status.total_grade[PLAYER_IN_LINE] -= dfs_status.oblique_delta_grade[PLAYER_IN_LINE][oblique_delta_index];
    //dfs_status.total_grade[PLAYER_IN_LINE] += _grade;

    dfs_status.oblique_delta_grade[PLAYER_IN_LINE][oblique_delta_index + BOARD_SIZE] = _grade;

}

void update_grade(int i, int j) {
    update_line_grade_row(i, WHITE);
    update_line_grade_col(j, WHITE);
    update_line_grade_oblique_sum(i + j, WHITE);
    update_line_grade_oblique_delta(i - j, WHITE);

    update_line_grade_row(i, BLACK);
    update_line_grade_col(j, BLACK);
    update_line_grade_oblique_sum(i + j, BLACK);
    update_line_grade_oblique_delta(i - j, BLACK);
}


/*  return -1 if that position is occupied
 *  return 0 if pc==0, which means there's nothing to do
 *  return 1 -> successful
 * */

int add_piece(int i, int j, int piece_color) {
    if (status.board[i][j] != VOID)
        return -1;
    status.board[i][j] = piece_color;
    status.oblique_line_sum[i + j][j] = piece_color;
    status.oblique_line_delta[i - j + BOARD_SIZE][j] = piece_color;
    status.steps++;
    return dfs_add_piece(i, j, piece_color);
}

/*  return -1 if that position is occupied
 *  return 1 -> successful
 * */
int dfs_add_piece(int i, int j, int piece_color) {
    if (dfs_status.board[i][j] != VOID && piece_color != VOID)
        return -1;
    dfs_status.board[i][j] = piece_color;
    dfs_status.oblique_line_sum[i + j][j] = piece_color;
    dfs_status.oblique_line_delta[i - j + BOARD_SIZE][j] = piece_color;
    if (piece_color == VOID) {
        dfs_status.steps--;
    } else {
        dfs_status.steps++;
        record[dfs_status.steps].i = i;
        record[dfs_status.steps].j = j;
        record[dfs_status.steps].player = piece_color;
    }
    update_grade(i, j);
    return 1;
}

