//
// Created by les on 18-12-25.
//

#ifndef GOBANG_BRUTEFORCE_H
#define GOBANG_BRUTEFORCE_H

#include "icld.h"
#include "customTypes.h"
#include "AC_Automachine.h"

void brute_force();

void process_print(int64_t cnt, int64_t MAX);

void line_pattern_parse(int8_t line[]);

uint64_t get_index(int8_t line[]);

#endif //GOBANG_BRUTEFORCE_H
