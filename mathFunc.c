//
// Created by Nine_yota on 2018-10-08.
//

#include "mathFunc.h"


inline long long min(long long x, long long y) {
    if (x < y)
        return x;
    else
        return y;
}

inline long long max(long long x, long long y) {
    if (x > y)
        return x;
    else
        return y;
}

inline int int_min(int x, int y) {
    if (x < y)
        return x;
    else
        return y;
}

inline int int_max(int x, int y) {
    if (x > y)
        return x;
    else
        return y;
}


inline void min_heapfy(drop_choice *a, int start, int end) {
    int dad = start;
    int son = 2 * dad + 1;
    while (son <= end) {
        if (son + 1 <= end && a[son].grade_estimate > a[son + 1].grade_estimate)
            ++son;
        if (a[dad].grade_estimate < a[son].grade_estimate)
            return;
        else {
            swap(&a[dad], &a[son], drop_choice);
            dad = son;
            son = dad * 2 + 1;
        }
    }
}

inline void max_heapfy(drop_choice *a, int start, int end) {
    int dad = start;
    int son = 2 * dad + 1;
    while (son <= end) {
        if (son + 1 <= end && a[son].grade_estimate < a[son + 1].grade_estimate)
            ++son;
        if (a[dad].grade_estimate > a[son].grade_estimate)
            return;
        else {
            swap(&a[dad], &a[son], drop_choice);
            dad = son;
            son = dad * 2 + 1;
        }
    }
}

inline void choice_sort(drop_choice *a, int len, int player_side) {
    if (player_side == WHITE) {
        for (int i = len / 2 - 1; i >= 0; --i) {
            min_heapfy(a, i, len - 1);
        }
        for (int i = len - 1; i > 0; --i) {
            swap(&a[0], &a[i], drop_choice);
            min_heapfy(a, 0, i - 1);
        }
    } else {
        for (int i = len / 2 - 1; i >= 0; --i) {
            max_heapfy(a, i, len - 1);
        }
        for (int i = len - 1; i > 0; --i) {
            swap(&a[0], &a[i], drop_choice);
            max_heapfy(a, 0, i - 1);
        }
    }
}

inline void int_min_heapfy(int *a, int stat, int end) {
    int dad = stat;
    int son = 2 * dad + 1;
    while (son <= end) {
        if (son + 1 <= end && a[son] > a[son + 1])
            ++son;
        if (a[dad] < a[son])
            return;
        else {
            swap(&a[dad], &a[son], int);
            dad = son;
            son = dad * 2 + 1;
        }
    }
}

inline void int_sort(int *a, int len) {
    for (int i = len / 2 - 1; i >= 0; --i) {
        int_min_heapfy(a, i, len - 1);
    }
    for (int i = len - 1; i > 0; --i) {
        swap(&a[0], &a[i], int);
        int_min_heapfy(a, 0, i - 1);
    }
}

