#include "icld.h"
#include "interact.h"


void init();

int main() {
    init();
    play();
    return 0;
}

void init() {
    AC_build();
    status_init();
    evaluate_init();
    search_init();
    time_init();
}