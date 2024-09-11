#include "u_common.h"

#include "drivers/std/d_in.h"
#include "drivers/std/d_out.h"

#include <stdio.h>
#include <stdbool.h>

int main(void) {
    D_IN_Init();
    D_OUT_Init();
    stdin = &d_in;
    stdout = &d_out;

    while (true) {
        printf("Hello, world!\n");
    }
}