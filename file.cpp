#include "file.h"


bool
open_file(const char *filename, File *result, bool write = false, int trunc_to = -1)
{
  bool success = true;

  int open_flags;
  int mmap_flags;
  int mmap_prot;
  if (write)
  {
    open_flags = O_RDWR | O_TRUNC;
    mmap_prot = PROT_READ | PROT_WRITE;
    mmap_flags = MAP_SHARED;
  }
  else
  {
    open_flags = O_RDONLY;
    mmap_prot = PROT_READ;
    mmap_flags = MAP_PRIVATE;
  }

  result->fd = open(filename, open_flags);
  if (result->fd == -1)
  {
    printf("Failed to open file: \"%s\"\n", filename);
    success = false;
  }
  else
  {
    if (trunc_to < 0)
    {
      struct stat sb;
      if (fstat(result->fd, &sb) == -1)
      {
        printf("Failed to fstat : \"%s\"\n", filename);
        success = false;
      }
      else
      {
        result->size = sb.st_size;
      }
    }
    else
    {
      result->size = trunc_to;
      if (ftruncate(result->fd, result->size) == -1)
      {
        printf("Failed to ftruncate file: \"%s\"\n", filename);
        success = false;
      }
    }

    if (success)
    {
      result->text = (char *)mmap(NULL, result->size, mmap_prot, mmap_flags, result->fd, 0);
      if (result->text == MAP_FAILED)
      {
        printf("Failed to map file: \"%s\"\n", filename);
        success = false;
      }
      else
      {
        if (write)
        {
          result->read_only = NULL;
        }
        else
        {
          result->read_only = result->text;
        }
      }
    }
  }

  if (!success)
  {
    close(result->fd);
    result->fd = -1;
    result->text = 0;
    result->read_only = 0;
    result->size = 0;
  }

  return success;
}