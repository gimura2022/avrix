#include "drivers/std/d_fs.h"
#include "drivers/fs/d_axfs.h"

int mkdir(const char *path, __mode_t mode) {
    const u16          root_entry_sect = D_axfs_GetEntryByPath(path);
    const axfs_entry_t root_entry      = D_axfs_ReadEntry()
    const axfs_dir_t dir = {
        .count   = 0,
        .entries = {0}
    };

    D_axfs_CreateDirectory(root_entry_sect, &dir, "test", )
}

int fgetpos(FILE *stream, fpos_t *pos)                          { return -1; }
FILE *fopen(const char *path, const char *mode)                 { return NULL; }
FILE *freopen(const char *path, const char *mode, FILE *stream) { return NULL; }
// FILE *fdopen(int, const char *)                                 { return NULL; }
int fseek(FILE *stream, long offset, int whence)                { return -1; }
int fsetpos(FILE *stream, fpos_t *pos)                          { return -1; }
long ftell(FILE *stream)                                        { return -1; }
// int fileno(FILE *)                                              { return -1; }
void perror(const char *s)                                      {}
int remove(const char *pathname)                                { return -1; }
int rename(const char *oldpath, const char *newpath)            { return -1; }
void rewind(FILE *stream)                                       {}
void setbuf(FILE *stream, char *buf)                            {}
FILE *tmpfile(void)                                             { return NULL; }
char *tmpnam (char *s)                                          { return NULL; }

void D_FS_Init(void) {}