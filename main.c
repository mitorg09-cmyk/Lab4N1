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

  char elsOrigPtr[9] = {};
  char* els = elsOrigPtr;
  size_t i = 0;
  char teilLen = 0; // Using as integer
  fprintf(nF, "%c", 1); // Reseved for teilLen
  do
  {
    els = elsOrigPtr;
    for(i = 0; i < 8 && fscanf(pF, "%c", els + i) != -1; i++);
    if(i == 8)
    {
      for(size_t j = 1; j < 8; j++)
      {
        els[j] = els[j] | (els[0] << (8 - j)) & 128;
      }
      els[0] = 0;
      els[8] = '\0';
      els = els + 1;
    }
    else
    {
      els[i] = '\0';
      teilLen += i;
    }

    fprintf(nF, "%s", els);
  }while(i != 0);

  rewind(nF);
  printf("teLen = %d\n", teilLen);
  fprintf(nF, "%c", teilLen);

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
