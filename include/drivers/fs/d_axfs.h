#ifndef _d_axfs_h
#define _d_axfs_h

#include <stdio.h>

#include "drivers/std/d_fs.h"
#include "u_common.h"

#define MAX_PATH_DEPTH 16
#define MAX_ENTRY_NAME 16
#define MAX_DIR_ENTRY  15
#define BLOCK_SIZE     32

typedef struct PACKED {
    u16 buf;   // u8*
    u8  unget;
    u8  flags;
    u16 size;
    u16 len;
    u16 put;   // int (*put)(char, struct __file *)
    u16 get;   // int (*get)(struct __file *)
    u16 udata; // void*

    u8 __pad[18];
} axfs_file_t;

typedef struct PACKED {
    u16 entries[MAX_DIR_ENTRY];
    u8  count;
} axfs_dir_t;

typedef enum {
    ET_NULL = 0,
    ET_FILE,
    ET_DIR
} axfs_entry_type_t;

typedef struct PACKED {
    u8   type;
    char name[MAX_ENTRY_NAME];

    u16 first_block;
    u8  blocks;

    u16 reader; // void*, reader func
    u16 writer; // void*, writer func
    u16 offset; // void*

    u8 __pad[6];
} axfs_entry_t;

typedef struct PACKED {
    u16 entry_count;
    u16 block_count;
    u16 max_blocks;

    u8 __pad[26];
} axfs_device_header_t;

FILE        D_axfs_AxfsFileToFile(axfs_file_t file);
axfs_file_t D_axfs_FileToAxfsFile(FILE file);

extern u8 root_device[BLOCK_SIZE * 3];

void D_axfs_InitDevice(mem_reader_t reader, mem_writer_t writer, void* offset);
void D_axfs_Init(void);

u8*  D_axfs_ReadSector(usize id, void* offset, mem_reader_t reader);
void D_axfs_WriteSector(usize id, void* offset, mem_writer_t writer, u8* data);

axfs_entry_t D_axfs_ReadEntry( u16 sector, mem_reader_t reader, void* offset);
void         D_axfs_WriteEntry(u16 sector, mem_writer_t writer, void* offset, axfs_entry_t* entry);

void D_axfs_Mount(
    u16 sector, mem_reader_t reader,     mem_writer_t writer,     void* offset,
                mem_reader_t new_reader, mem_writer_t new_writer, void* new_offset
);

void D_axfs_CreateDirectory(u16 root_dir_sect, axfs_dir_t* dir, const char* name, mem_reader_t reader, mem_writer_t writer, void* offset);
void D_axfs_CreateFile(u16 root_dir_sect, axfs_file_t* file, const char* name, mem_reader_t reader, mem_writer_t writer, void* offset);

u16 D_axfs_GetEntryByPath(const char* path);

#endif