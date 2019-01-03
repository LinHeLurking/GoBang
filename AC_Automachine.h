//
// Created by les on 18-10-10.
//

#ifndef GOBANG_AC_AUTOMACHINE_H
#define GOBANG_AC_AUTOMACHINE_H

#include "icld.h"
#include "customTypes.h"

//CAUTIONS!!! this PATTERNS must be exactly the same as the number of patterns
#define PATTERNS 44
#define END 107

void AC_build();

// values may need changing or the program would be too greedy. QAQ
#define FIVE_GRADE 500000L
#define CONTINUOUS_FOUR 20000L
#define CONTINUOUS_THREE 2000L
#define CONTINUOUS_TWO 100L
#define CONTINUOUS_ONE 10L

#define HALF_FOUR 10000L
#define HALF_THREE 800L
#define HALF_TWO 50L

#define SPLIT_ALIVE_THREE 1500L
#define SPLIT_ALIVE_FOUR_WITHOUT3 10000L
#define SPLIT_ALIVE_FOUR_WITH3 8000L

#define LONG_CONTINUOUS 100000L


#endif //GOBANG_AC_AUTOMACHINE_H
