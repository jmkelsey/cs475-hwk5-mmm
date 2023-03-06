#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "rtclock.h"
#include "mmm.h"

int main(int argc, char *argv[]) {
    mmm_init(2,5);
    print_both();
    mmm_par(NULL);
    print_both();
    return 0;
}