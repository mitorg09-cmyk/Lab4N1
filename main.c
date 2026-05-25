#include <stdio.h>
#include <string.h>
#include <unistd.h>

char* packFile(const char* fNameOrPath);
char* unpackFile(const char* fNameOrPath);

int main()
{
  char* fn = packFile("file.txt");

  char* funN = unpackFile(fn);

  return 0;
}
