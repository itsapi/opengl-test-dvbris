#include "strings.h"


bool
strings_equal(char a[], char b[])
{
  bool equal = true;

  char *ap = a;
  char *bp = b;

  while (true)
  {
    if (*ap != *bp)
    {
      equal = false;
      break;
    }

    if (*ap == '\0' || *bp == '\0')
    {
      break;
    }

    ++ap;
    ++bp;
  }

  return equal;
}