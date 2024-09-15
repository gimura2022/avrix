#ifndef _d_fs_h
#define _d_fs_h

#include <stdio.h>

#include "u_common.h"

typedef u8   (*mem_reader_t)(void*);
typedef void (*mem_writer_t)(void*, u8);

void D_FS_Init(void);

#endif