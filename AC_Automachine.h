//
// Created by les on 18-10-10.
//

#ifndef GOBANG_AC_AUTOMACHINE_H
#define GOBANG_AC_AUTOMACHINE_H

#include "icld.h"
#include "customTypes.h"

//CAUTIONS!!! this PATTERNS must be exactly the same as the number of patterns
#define PATTERNS 48
#define END 107

void AC_build();

// values may need changing or the program would be too greedy. QAQ
#define FIVE_GRADE 500000L
#define CONTINUOUS_FOUR 2000L
#define CONTINUOUS_THREE 200L
#define CONTINUOUS_TWO 10L
#define CONTINUOUS_ONE 1L

#define HALF_FOUR 800L
#define HALF_THREE 80L
#define HALF_TWO 5L

#define SPLIT_ALIVE_THREE 150L
#define SPLIT_ALIVE_FOUR 1000L

#define LONG_CONTINUOUS 500000L


#endif //GOBANG_AC_AUTOMACHINE_H
