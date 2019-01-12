#ifndef GOBANG_MATHFUNC_H
#define GOBANG_MATHFUNC_H

#include "icld.h"
#include "statusSearch.h"

long long long_long_max(long long x, long long y);

long long long_long_min(long long x, long long y);

int int_min(int x,int y);

int int_max(int x,int y);

void choice_sort(drop_choice *a, int len, int player_side);

void int_sort(int *a, int len);

void int_min_heapfy(int *a, int stat, int end);

void min_heapfy(drop_choice *a, int start, int end);

void max_heapfy(drop_choice *a, int start, int end);


#define swap(a, b, type) type tmp=*(a); *(a)=*(b); *(b)=tmp;

#define abs(x) x<0?(0-(x)):(x)


#endif //GOBANG_MATHFUNC_H
