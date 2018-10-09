//
// Created by Nine_yota on 2018-10-08.
//
#include "icld.h"

#ifndef GOBANG_MATHFUNC_H
#define GOBANG_MATHFUNC_H

long long max(long long x, long long y);

long long min(long long x, long long y);

int int_min(int x,int y);

int int_max(int x,int y);

void choice_sort(drop_choice *a, int len);

void int_sort(int *a, int len);

void int_min_heapfy(int *a, int stat, int end);


#define swap(a, b, type) type tmp=*(a); *(a)=*(b); *(b)=tmp;

#define abs(x) x<0?0-(x):x


#endif //GOBANG_MATHFUNC_H
