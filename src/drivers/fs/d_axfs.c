#include "drivers/fs/d_axfs.h"
#include "drivers/fs/storage_d/d_ram.h"

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

FILE D_axfs_AxfsFileToFile(axfs_file_t file) {
    FILE std_file = {
        .buf   = file.buf,
        .flags = file.flags,
        .get   = file.get,
        .len   = file.len,
        .put   = file.put,
        .size  = file.size,
        .udata = file.udata,
        .unget = file.unget
    };

    return std_file;
}

axfs_file_t D_axfs_FileToAxfsFile(FILE file) {
    axfs_file_t axfs_file = {
        .buf   = file.buf,
        .flags = file.flags,
        .get   = file.get,
        .len   = file.len,
        .put   = file.put,
        .size  = file.size,
        .udata = file.udata,
        .unget = file.unget
    };

    return axfs_file;
}

u8 root_device[BLOCK_SIZE * 3];

static axfs_device_header_t D_axfs_ReadDeviceHeader( mem_reader_t reader, void* offset);
static void                 D_axfs_WriteDeviceHeader(mem_writer_t writer, void* offset, axfs_device_header_t* header);

static u16          D_axfs_CreateEntry(axfs_entry_t* entry, mem_reader_t reader, mem_writer_t writer, void* offset);

static axfs_dir_t D_axfs_ReadDirectory(u16 sector, mem_reader_t reader, void* offset);
static u16        D_axfs_GetRootDirSector(void);
static axfs_dir_t D_axfs_ReadRootDirectory(void);
static void       D_axfs_WriteDirectory(u16 sector, mem_writer_t writer, void* offset, axfs_dir_t* dir);

static u16 D_axfs_FindFreeSectors(mem_reader_t reader, void* offset, u8 count);

void D_axfs_CreateDirectory(u16 root_dir_sect, axfs_dir_t* dir, const char* name, mem_reader_t reader, mem_writer_t writer, void* offset) {
    const axfs_device_header_t device_header = D_axfs_ReadDeviceHeader(reader, offset);

    const u16 dir_sector = D_axfs_FindFreeSectors(reader, offset, 1);
    D_axfs_WriteDirectory(dir_sector, writer, offset, &dir);

    axfs_entry_t entry = {
        .first_block = dir_sector,
        .blocks      = 1,
        .name        = {'\0'},
        .reader      = reader,
        .writer      = writer,
        .offset      = offset,
        .type        = ET_DIR
    };
    memcpy(entry.name, name, strlen(name));
    const u16 entry_sector = D_axfs_CreateEntry(&entry, reader, writer, offset);

    axfs_dir_t root_dir = D_axfs_ReadDirectory(root_dir_sect, reader, offset);
    root_dir.entries[root_dir.count++] = entry_sector;
}

void D_axfs_CreateFile(u16 root_dir_sect, axfs_file_t* file, const char* name, mem_reader_t reader, mem_writer_t writer, void* offset) {
    const axfs_device_header_t device_header = D_axfs_ReadDeviceHeader(reader, offset);


}

void D_axfs_Mount(
    u16 sector, mem_reader_t reader,     mem_writer_t writer,     void* offset,
                mem_reader_t new_reader, mem_writer_t new_writer, void* new_offset
) {
    axfs_entry_t entry = D_axfs_ReadEntry(sector, reader, offset);
    entry.reader = new_reader;
    entry.writer = new_writer;
    entry.offset = offset;
    D_axfs_WriteEntry(sector, writer, offset, &entry);
}

u16 D_axfs_GetEntryByPath(const char* path) {
    char path_copy[MAX_PATH_DEPTH * MAX_ENTRY_NAME];
    memcpy(path_copy, path, sizeof(path));

    char tok_path[MAX_PATH_DEPTH][MAX_ENTRY_NAME];
    u8   tok_path_entry = 0;
    memset(tok_path, '\0', sizeof(tok_path));

    for (const char* entry_name = strtok(path_copy, "/"); path != NULL; path = strtok(NULL, "/")) {
        memcpy(tok_path[tok_path_entry++], entry_name, strlen(entry_name));
    }

    const axfs_dir_t root_dir = D_axfs_ReadRootDirectory();
    axfs_entry_t     entry;
    u16              entry_sector;

    for (u8 i = 0; i < tok_path_entry; i++) {
        const char* path_chunk = tok_path[i];
        bool        entry_in_path = false;

        for (u8 j = 0; j < root_dir.count; j++) {
            entry = D_axfs_ReadEntry(root_dir.entries[i], D_ram_Read, root_device);
            entry_sector = root_dir.entries[i];

            if (strcmp(entry.name, path_chunk) == 0) {
                entry_in_path = true;
                break;
            }
        }

        if (!entry_in_path) break;
    }

    return entry_sector;
}

void D_axfs_InitDevice(mem_reader_t reader, mem_writer_t writer, void* offset) {
    u8 data[BLOCK_SIZE];
    axfs_device_header_t device_header = {
        .entry_count = 0,
        .block_count = 1,
        .max_blocks  = 3
    };
    memcpy(data, &device_header, sizeof(axfs_device_header_t));
    D_axfs_WriteSector(0, offset, writer, data);

    axfs_entry_t root_entry = {
        .blocks      = 1,
        .first_block = 2,
        .name        = NULL,
        .reader      = reader,
        .writer      = writer,
        .offset      = offset,
        .type        = ET_DIR
    };
    D_axfs_CreateEntry(
        &root_entry,
        reader,
        writer,
        offset
    );

    axfs_dir_t root_dir = {
        .entries = { 0 },
        .count   = 0
    };
    memcpy(data, &root_dir, sizeof(axfs_dir_t));
    D_axfs_WriteSector(2, offset, writer, data);
}

void D_axfs_Init(void) {
    D_axfs_InitDevice(D_ram_Read, D_ram_Write, root_device);
}

u8* D_axfs_ReadSector(usize id, void* offset, mem_reader_t reader) {
    u8* data = malloc(BLOCK_SIZE);

    for (register usize i = 0; i < BLOCK_SIZE; i++) {
        data[i] = reader(((id * BLOCK_SIZE) + i) + offset);
    }
}

void D_axfs_WriteSector(usize id, void* offset, mem_writer_t writer, u8* data) {
    for (register usize i = 0; i < BLOCK_SIZE; i++) {
        writer(((id * BLOCK_SIZE) + i) + offset, data[i]);
    }
}

static axfs_device_header_t D_axfs_ReadDeviceHeader(mem_reader_t reader, void* offset) {
    u8* data = D_axfs_ReadSector(0, offset, reader);
    axfs_device_header_t device_header;

    memcpy(&device_header, data, BLOCK_SIZE);
    free(data);

    return device_header;
}

static void D_axfs_WriteDeviceHeader(mem_writer_t writer, void* offset, axfs_device_header_t* header) {
    u8 data[BLOCK_SIZE];
    memcpy(data, header, BLOCK_SIZE);

    D_axfs_WriteSector(0, offset, writer, data);
}

static u16 D_axfs_CreateEntry(axfs_entry_t* entry, mem_reader_t reader, mem_writer_t writer, void* offset) {
    axfs_device_header_t device_header = D_axfs_ReadDeviceHeader(reader, offset);

    u8 data[BLOCK_SIZE];
    memcpy(data, entry, BLOCK_SIZE);

    const u16 entry_sector = device_header.entry_count++;
    D_axfs_WriteSector(entry_sector, offset, writer, data);
    D_axfs_WriteDeviceHeader(writer, offset, &device_header);

    return entry_sector;
}

axfs_entry_t D_axfs_ReadEntry(u16 sector, mem_reader_t reader, void* offset) {
    u8* data = D_axfs_ReadSector(sector, offset, reader);
    axfs_entry_t entry;

    memcpy(&entry, data, BLOCK_SIZE);
    free(data);

    return entry;
}

void D_axfs_WriteEntry(u16 sector, mem_writer_t writer, void* offset, axfs_entry_t* entry) {
    u8 data[BLOCK_SIZE];
    memcpy(data, entry, BLOCK_SIZE);

    D_axfs_WriteSector(sector, offset, writer, data);
}

static axfs_dir_t D_axfs_ReadDirectory(u16 sector, mem_reader_t reader, void* offset) {
    u8* data = D_axfs_ReadSector(sector, offset, reader);
    axfs_dir_t dir;
    
    memcpy(&dir, data, BLOCK_SIZE);
    free(data);

    return dir;
}

static u16 D_axfs_GetRootDirSector(void) {
    const axfs_entry_t root_entry = D_axfs_ReadEntry(1, D_ram_Read, root_device);
    return root_entry.first_block;
}

static axfs_dir_t D_axfs_ReadRootDirectory(void) {
    const axfs_dir_t root_dir = D_axfs_ReadDirectory(D_axfs_GetRootDirSector(), D_ram_Read, root_device);
    return root_dir;
}

static void D_axfs_WriteDirectory(u16 sector, mem_writer_t writer, void* offset, axfs_dir_t* dir) {
    u8 data[BLOCK_SIZE];
    memcpy(data, dir, BLOCK_SIZE);

    D_axfs_WriteSector(sector, offset, writer, data);
}

static u16 D_axfs_FindFreeSectors(mem_reader_t reader, void* offset, u8 count) {
    const axfs_device_header_t device_header = D_axfs_ReadDeviceHeader(reader, offset);
    u16                        block         = device_header.max_blocks;

    while (block > device_header.entry_count + 1) {
        for (u16 i = 0; i < device_header.entry_count; i++) {
            axfs_entry_t entry = D_axfs_ReadEntry(i + 1, reader, offset);
            if (block >= entry.first_block && block + count <= entry.first_block + entry.blocks) {
                block--; // FIXME: non optimal (super slow)
                break;
            }
        }
    }

    return block;
}