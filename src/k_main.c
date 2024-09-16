#include "u_common.h"

#include <avr/interrupt.h>

#include "drivers/std/d_in.h"
#include "drivers/std/d_out.h"
#include "drivers/std/d_fs.h"

#include "drivers/fs/d_axfs.h"

#include <stdio.h>
#include <stdbool.h>

int main(void) {
    D_IN_Init();
    D_OUT_Init();
    stdin = &d_in;
    stdout = &d_out;

    D_axfs_Init();

    printf("Hello, world\n!");
}