//
// Created by Nine_yota on 2018-10-08.
//

#include "mathFunc.h"


long long min(long long x, long long y) {
    if (x < y)
        return x;
    else
        return y;
}

long long max(long long x, long long y) {
    if (x > y)
        return x;
    else
        return y;
}

int int_min(int x,int y){
    if (x < y)
        return x;
    else
        return y;
}

int int_max(int x,int y){
    if (x > y)
        return x;
    else
        return y;
}


void min_heapfy(drop_choice *a, int stat, int end) {
    int dad = stat;
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

void choice_sort(drop_choice *a, int len) {
    for (int i = len / 2 - 1; i >= 0; --i) {
        min_heapfy(a, i, len - 1);
    }
    for (int i = len - 1; i > 0; --i) {
        swap(&a[0], &a[i], drop_choice);
        min_heapfy(a, 0, i - 1);
    }
}

void int_min_heapfy(int *a, int stat, int end) {
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

void int_sort(int *a, int len) {
    for (int i = len / 2 - 1; i >= 0; --i) {
        int_min_heapfy(a, i, len - 1);
    }
    for (int i = len - 1; i > 0; --i) {
        swap(&a[0], &a[i], int);
        int_min_heapfy(a, 0, i - 1);
    }
}

