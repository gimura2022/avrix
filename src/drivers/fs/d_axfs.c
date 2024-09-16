#include "drivers/fs/d_axfs.h"
#include "drivers/fs/storage_d/d_ram.h"

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

i16 axfs_errno;

static axfs_device_t crnt_device;

static void D_axfs_WriteSector(u16 sector, u8* data);
static u8*  D_axfs_ReadSector(u16 sector);

static void                 D_axfs_WriteDeviceHeader(axfs_device_header_t* header);
static axfs_device_header_t D_axfs_ReadDeviceHeader(void);

static u16 D_axfs_FindFreeSectorsBottom(u8 count);
static u16 D_axfs_FindFreeEntrySector(void);

static u16          D_axfs_CreateEntry(axfs_entry_t* entry);
static void         D_axfs_WriteEntry(u16 sector, axfs_entry_t* entry);
static axfs_entry_t D_axfs_ReadEntry(u16 sector);

static void       D_axfs_WriteDirectory(u16 sector, axfs_dir_t* dir);
static axfs_dir_t D_axfs_ReadDirectory(u16 sector);

static void D_axfs_InitDevice(u16 max_sectors);

#define ROOT_DEVICE_SECTORS 3

static u8     root_device_buffer[BLOCK_SIZE * ROOT_DEVICE_SECTORS];
axfs_device_t root_device;

void D_axfs_Init(void) {
    root_device.offset = root_device_buffer;
    root_device.reader = D_ram_Read;
    root_device.writer = D_ram_Write;

    crnt_device = root_device;
    D_axfs_InitDevice(ROOT_DEVICE_SECTORS);
}

void D_axfs_CreateDirectory(u16 root_dir_sect, const char* name, axfs_dir_t* dir) {
    const u16 dir_sector = D_axfs_FindFreeSectorsBottom(1);
    D_axfs_WriteDirectory(dir_sector, dir);

    axfs_entry_t entry = {
        .first_block = dir_sector,
        .blocks      = 1,
        .name        = { '\0' },
        .type        = ET_DIR,
        .reader      = crnt_device.reader,
        .writer      = crnt_device.writer,
        .offset      = crnt_device.offset
    }; 
    memcpy(entry.name, name, strlen(name));
    const u16 dir_entry_sect = D_axfs_CreateEntry(&entry);

    if (root_dir_sect == 0) return;

    axfs_dir_t root_dir = D_axfs_ReadDirectory(root_dir_sect);
    root_dir.entries[root_dir.count++] = dir_entry_sect;
    D_axfs_WriteDirectory(root_dir_sect, &root_dir);
}

static void D_axfs_WriteSector(u16 sector, u8* data) {
    for (register u8 i = 0; i < BLOCK_SIZE; i++) {
        crnt_device.writer(((sector * BLOCK_SIZE) + i) + crnt_device.offset, data[i]);
    }
}

static u8* D_axfs_ReadSector(u16 sector) {
    u8* data = malloc(BLOCK_SIZE);

    for (register usize i = 0; i < BLOCK_SIZE; i++) {
        data[i] = crnt_device.reader(((sector * BLOCK_SIZE) + i) + crnt_device.offset);
    }
}

static void D_axfs_WriteDeviceHeader(axfs_device_header_t* header) {
    u8 data[BLOCK_SIZE];
    memcpy(data, header, BLOCK_SIZE);

    D_axfs_WriteSector(0, data);
}

static axfs_device_header_t D_axfs_ReadDeviceHeader(void) {
    u8*                  data = D_axfs_ReadSector(0);
    axfs_device_header_t header;

    memcpy(&header, data, BLOCK_SIZE);
    free(data);

    return header;
}

static void D_axfs_InitDevice(u16 max_sectors) {
    axfs_device_header_t device_header = {
        .entry_count  = 0,
        .sector_count = 0,
        .max_sectors  = max_sectors 
    };
    D_axfs_WriteDeviceHeader(&device_header);

    axfs_dir_t dir = {
        .count   = 0,
        .entries = {0}
    };
    D_axfs_CreateDirectory(0, "", &dir);
}

static u16 D_axfs_FindFreeSectorsBottom(u8 count) {
    const axfs_device_header_t device_header = D_axfs_ReadDeviceHeader();
    u16                        sector        = device_header.max_sectors;
    bool                       found         = false;

    while (sector > device_header.entry_count + 1) {
        for (u16 i = 0; i < device_header.entry_count; i++) {
            const axfs_entry_t entry = D_axfs_ReadEntry(i + 1);

            if (sector >= entry.first_block && sector + count <= entry.first_block + entry.blocks) {
                sector--;
                break;
            } else {
                found = true;
                goto done;
            }
        }
    }

    done:
        if (!found) axfs_errno = AXFSE_CANT_ALLOC_BLOCK;
        return sector;
}

static u16 D_axfs_FindFreeEntrySector(void) {
    const axfs_device_header_t device_header = D_axfs_ReadDeviceHeader();

    for (u16 i = 1; i < device_header.max_sectors - device_header.sector_count; i++) {
        if (i > device_header.entry_count) return i;

        const axfs_entry_t entry = D_axfs_ReadEntry(i);
        if (entry.type == ET_NULL) return i;
    }

    axfs_errno = AXFSE_CANT_ALLOC_ENTRY;
    return 0;
}

static void D_axfs_WriteEntry(u16 sector, axfs_entry_t* entry) {
    u8 data[BLOCK_SIZE];
    memcpy(data, entry, BLOCK_SIZE);

    D_axfs_WriteSector(sector, data);
}

static axfs_entry_t D_axfs_ReadEntry(u16 sector) {
    u8*          data = D_axfs_ReadSector(sector);
    axfs_entry_t entry;

    memcpy(&entry, data, BLOCK_SIZE);
    free(data);

    return entry;
}

static u16 D_axfs_CreateEntry(axfs_entry_t* entry) {
    axfs_device_header_t       device_header = D_axfs_ReadDeviceHeader();
    const u16                  entry_sector  = D_axfs_FindFreeEntrySector();
    if (axfs_errno != 0) return;

    if (entry_sector > device_header.entry_count) device_header.entry_count++;

    D_axfs_WriteEntry(entry_sector, entry);
    D_axfs_WriteDeviceHeader(&device_header);

    return entry_sector;
}

static void D_axfs_WriteDirectory(u16 sector, axfs_dir_t* dir) {
    u8 data[BLOCK_SIZE];
    memcpy(data, dir, BLOCK_SIZE);

    D_axfs_WriteSector(sector, data);
}

static axfs_dir_t D_axfs_ReadDirectory(u16 sector) {
    u8*        data = D_axfs_ReadSector(sector);
    axfs_dir_t dir;

    memcpy(&dir, data, BLOCK_SIZE);
    free(data);

    return dir;
}