#include <stdio.h>
#include <string.h>
#include <unistd.h>

char* packFile(const char* fNameOrPath);
char* unpackFile(const char* fNameOrPath);
char* AllNormalASCIISymbsCreate();

//tests des
// file.txt - just combination of eng lits and numbers
// file1.txt - NOT ASCII SYMBOL (russian)
// file2.txt - 24 symbols (3 symbols packed, tail len == 0)
// file3.txt - 30 symbols (3 symbols packed, tail len == 7)
// file4.txt - 7 symbols (0 symbols packed, tail == 7)
// file5.txt - long text
// file7.txt - empty file


int main()
{
  char* Name = "file.txt";

  // Name = AllNormalASCIISymbsCreate(); // file6 - ALL NORMAL ASCII SYMBS

  FILE* pF = fopen(Name, "rb");
  if(!pF)
    return 0;

  char* fn = packFile(Name);
  if(fn != "pkdFile.txt")
    {
      printf("%s\n", fn);
      fclose(pF);
      return 0;
    }

  char* funN = unpackFile(fn);
  if(funN != "unpkdFile.txt")
    {
      printf("%s\n", funN);
      fclose(pF);
      return 0;
    }

  FILE* nF = fopen(funN, "rb");
  if(!nF)
  {
    fclose(pF);
    return 0;
  }


  // TESTER
  char el1 = 0;
  char el2 = 0;
  while((fscanf(pF, "%c", &el1) != -1) && (fscanf(nF, "%c", &el2) != -1))
  {
    if(el1 != el2)
    {
      fclose(pF);
      fclose(nF);
      printf("NO\n");
        return 0;
    }
  }
  printf("YES\n");
  // TESTER END

  fclose(pF);
  fclose(nF);
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
      // printf("els[i] is %d\n", els[i]);
      if((els[i] & 128) != 0)
      {
        // printf("err el = %b\n", els[i]);
        // printf("older bit of el = %b\n", els[i] & 128);
        fclose(pF);
        remove("pkdFile");
        fclose(nF);
        return "ERROR: NOT ASCII SYMBOL";
      }
      // else
      //   printf("normal el = %b\n", els[i]);
    }
    if(i == 8)
    {
      // printf("%s\n", els);
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
      // printf("i is %d\n", i);
      // printf("els[i] is %d\n", els[i]);
      teilLen += i;
    }

    // els[i] = '\0';

    fprintf(nF, "%s", els);
    // printf("%s\n", els);
  }while(i != 0);

  rewind(nF);
  printf("teLen = %d\n", teilLen);
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

char* AllNormalASCIISymbsCreate()
{
  FILE* pF = fopen("file6.txt", "wb");
  if(!pF)
    return NULL;

  for(unsigned char i = 1; i < 128; i++)
    fprintf(pF, "%c", (char)i);

  fclose(pF);

  return "file6.txt";
}
