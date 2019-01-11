#include "icld.h"
#include "interact.h"


void init();

int main() {
    init();
    play();
    return 0;
}

void init() {
    // initializer that finishes the preparations before main loop begins.
    AC_build();
    status_init();
    evaluate_init();
    search_init();
    time_init();
}