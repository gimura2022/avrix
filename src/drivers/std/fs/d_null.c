#include "drivers/std/d_fs.h"

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