#include "u_common.h"
#include "drivers/d_tty.h"

#include <stdio.h>
#include <stdbool.h>

int main(void) {
    D_TTY_Init();
    stdin  = &d_tty_in;
    stdout = &d_tty_out;

    while (true) {
        printf("Hello, world!\n");
    }
}