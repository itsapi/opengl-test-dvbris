#ifndef FILE_H_DEF
#define FILE_H_DEF

struct File
{
  int fd;
  char *text;
  const char *read_only;
  int size;
};

#endif