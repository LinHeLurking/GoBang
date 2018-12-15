//
// Created by les on 18-10-10.
//

#ifndef GOBANG_AC_AUTOMACHINE_H
#define GOBANG_AC_AUTOMACHINE_H

void AC_build();

// values may need changing or the program would be too greedy. QAQ
// make sure that split alive grades are mot the same as continuous ones.
//todo: values need changing to make it less aggressive
#define FIVE_GRADE 1000000
#define CONTINUOUS_FOUR 100000
#define CONTINUOUS_THREE 1000
#define CONTINUOUS_TWO 100
#define CONTINUOUS_ONE 10

#define HALF_FOUR 50000
#define HALF_THREE 500
#define HALF_TWO 50

#define SPLIT_ALIVE_THREE 850
#define SPLIT_ALIVE_FOUR 35000

#endif //GOBANG_AC_AUTOMACHINE_H
