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

char* packFile(const char* fNameOrPath)
{
  if(!fNameOrPath) return NULL;

  FILE* pF = fopen(fNameOrPath, "rb");
  if(!pF) return NULL;
  FILE* nF = fopen("pkdFile.txt", "wb");
  if(!nF)
  {
    fclose(pF);
    return NULL;
  }

  fclose(pF);
  fclose(nF);

  return "pkdFile.txt";
}

char* unpackFile(const char* fNameOrPath)
{
  if(!fNameOrPath) return NULL;

  FILE* pkdF = fopen(fNameOrPath, "rb");
  if(!pkdF) return NULL;
  FILE* unpkdF = fopen("unpkdFile.txt", "wb");
  if(!unpkdF)
  {
    fclose(pkdF);
    return NULL;
  }

  fclose(unpkdF);
  fclose(pkdF);

  return "unpkdFile.txt";
}
