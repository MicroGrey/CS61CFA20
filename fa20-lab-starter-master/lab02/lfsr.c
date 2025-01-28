#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lfsr.h"

void lfsr_calculate(uint16_t *reg) {
    /* YOUR CODE HERE */
    uint16_t temp = *reg;
    * reg = temp>>1 | ( (temp<<15 & 1<<15) ^ (temp<<13 & 1<<15) ^ (temp<<12 & 1<<15) ^ (temp<<10 & 1<<15) );
}

