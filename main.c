#include <stdio.h>
#include <string.h>
#include <unistd.h>

char* packFile(const char* fNameOrPath);
char* unpackFile(const char* fNameOrPath);

int main()
{
  char* fn = packFile("file1.txt");
  if(fn != "pkdFile.txt")
    printf("%s\n", fn);

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
    for(i = 0; i < 8 && fscanf(pF, "%c", els + i) != -1; i++)
    {
      if(els[i] & 128 != 0)
        fclose(pF);
        remove("pkdFile");
        fclose(nF);
        return "ERROR: NOT ASCII SYMBOL";
    }
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

    // els[i] = '\0';

    fprintf(nF, "%s", els);
    // printf("%s\n", els);
  }while(i != 0);

  rewind(nF);
  //printf("teLen = %d\n", teilLen);
  // teilLen = '5';
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

  char teilLen = 0;
  fscanf(pkdF, "%c", &teilLen); // Used as integer

  char el = 0;
  char unpkdEl = 0;
  size_t ix = 0;
  char elsOrigPtr[9] = {};
  char* els = elsOrigPtr;
  els[8] = '\0';
  while(fscanf(pkdF, "%c", &el) != -1)
  {
    unpkdEl = unpkdEl | ((el & 128) && 1) << ix;
    els[ix + 1] = el & 127;
    ix++;
    if(ix == 7)
    {
      els[0] = unpkdEl;
      if(teilLen)
        if(fscanf(pkdF, "%c", &el) == -1)
        {
          els++;
          //printf("+eah\n");
        }
        else
          fseek(pkdF, -1, SEEK_CUR);
      fprintf(unpkdF, "%s", els);
      ix = 0;
      unpkdEl = 0;
    }
  }
  els[ix + 1] = '\0';
  els[0] = 0;
  els++;
  fprintf(unpkdF, "%s", els);

  fclose(unpkdF);
  fclose(pkdF);

  return "unpkdFile.txt";
}
