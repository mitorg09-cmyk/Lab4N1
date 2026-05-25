#include <stdio.h>
#include <string.h>
#include <unistd.h>

int packFile(const char* fNameOrPath, const char* OutputName);
int unpackFile(const char* fNameOrPath, const char* OutputName);
int AllNormalASCIISymbsCreate();

//tests des
// file.txt - just combination of eng lits and numbers
// file1.txt - NOT ASCII SYMBOL (russian)
// file2.txt - 24 symbols (3 symbols packed, tail len == 0)
// file3.txt - 30 symbols (3 symbols packed, tail len == 7)
// file4.txt - 7 symbols (0 symbols packed, tail == 7)
// file5.txt - long text - 36912 symbols
// file7.txt - empty file


int main()
{
  char* Name = "file.txt";

  int code = AllNormalASCIISymbsCreate(); // file6 - ALL NORMAL ASCII SYMBS - 128 symbols

  FILE* pF = fopen(Name, "rb");
  if(!pF)
    return 0;

  int fn = packFile(Name, "pkdFile.txt");
  if(fn != 0)
    {
      printf("%d\n", fn);
      fclose(pF);
      return 0;
    }

  int funN = unpackFile("pkdFile.txt", "unpkdFile.txt");
  if(funN != 0)
    {
      printf("%d\n", funN);
      fclose(pF);
      return 0;
    }

  FILE* nF = fopen("unpkdFile.txt", "rb");
  if(!nF)
  {
    fclose(pF);
    return 0;
  }


  // TESTER
  char el1 = 0;
  char el2 = 0;
  int i = 0;
  while((fscanf(pF, "%c", &el1) != -1) && (fscanf(nF, "%c", &el2) != -1))
  {
    i++;
    if(el1 != el2)
    {
      fclose(pF);
      fclose(nF);
      printf("NO\n");
      return 0;
    }
  }
  printf("YES, %d\n", i);
  // TESTER END

  fclose(pF);
  fclose(nF);
  return 0;
}

int packFile(const char* fNameOrPath, const char* OutputName)
{
  if(!fNameOrPath) return -1;

  FILE* pF = fopen(fNameOrPath, "rb");
  if(!pF) return -1;
  FILE* nF = fopen("pkdFile.txt", "wb");
  if(!nF)
  {
    fclose(pF);
    return -1;
  }

  char elsOrigPtr[9] = {}; // Fixed bufer
  char* els = elsOrigPtr; // Pointer for cutting start a string
  size_t i = 0;
  char teilLen = 0; // Using as integer
  fprintf(nF, "%c", 1); // Reseved for teilLen
  do
  {
    els = elsOrigPtr; // Refresh pointer
    for(i = 0; i < 8 && fscanf(pF, "%c", els + i) != -1; i++) // Read 8 elements
    {
      // printf("els[i] is %d\n", els[i]);

      if((els[i] & 128) != 0) // ASCII checking
      {
        // printf("err el = %b\n", els[i]);
        // printf("older bit of el = %b\n", els[i] & 128);
        fclose(pF);
        remove("pkdFile");
        fclose(nF);
        return 1;
      }
      // else
      //   printf("normal el = %b\n", els[i]);
    }
    if(i == 8) // Packing 8 elements
    {
      // printf("%s\n", els);
      for(size_t j = 1; j < 8; j++)
      {
        els[j] = els[j] | (els[0] << (8 - j)) & 128;
      }
      els[0] = 0;
      els[8] = '\0'; // Hardcode setting stop-zero
      els = els + 1; // Cutting first element wich was packed
      i--;
    }
    else // For tail
    {
      // els[i] = '\0'; // Cutting end of tail string
      // printf("i is %d\n", i);
      // printf("els[i] is %d\n", els[i]);
      teilLen += i; // Setting tail len
    }

    // els[i] = '\0';

    for(short k = 0; k < i; k++) // Writing to pkdFile
    {
      fprintf(nF, "%c", els[k]);
    }
    // printf("%s\n", els);
  }while(i != 0);

  rewind(nF);
  printf("teLen = %d\n", teilLen); // FOR DEBUGGING
  // teilLen = '5';
  fprintf(nF, "%c", teilLen); // Writing tail len to pkdFile

  fclose(pF);
  fclose(nF);

  return 0;
}

int unpackFile(const char* fNameOrPath, const char* OutputName)
{
  if(!fNameOrPath) return -1;

  FILE* pkdF = fopen(fNameOrPath, "rb");
  if(!pkdF) return -1;
  FILE* unpkdF = fopen("unpkdFile.txt", "wb");
  if(!unpkdF)
  {
    fclose(pkdF);
    return -1;
  }

  char teilLen = 0;
  fscanf(pkdF, "%c", &teilLen); // Used as integer

  char el = 0;
  char unpkdEl = 0;
  size_t ix = 0;
  char elsOrigPtr[9] = {}; // Fixed bufer
  char* els = elsOrigPtr; // Pointer for cutting start a string
  els[8] = '\0'; // Hardcode setting stop-zero
  while(fscanf(pkdF, "%c", &el) != -1)
  {
    unpkdEl = unpkdEl | ((el & 128) && 1) << ix;
    els[ix + 1] = el & 127; // Clear older bit
    ix++;
    if(ix == 7) // If 7 new elements in bufer, unpack
    {
      els[0] = unpkdEl;
      if(teilLen) // If tail got len = 7
        if(fscanf(pkdF, "%c", &el) == -1)
        {
          els++; // Cutting start tail string
          //printf("+eah\n");
          ix--;
        }
        else
          fseek(pkdF, -1, SEEK_CUR);
      for(short k = 0; k < ix + 1; k++) // Writing to pkdFile
      {
        fprintf(unpkdF, "%c", els[k]);
      } // Writing to unpkdFile
      ix = 0;
      unpkdEl = 0;
    }
  }
  // els[ix + 1] = '\0'; // Setting end of tail string
  // els[0] = 0;
  els++; // Cutting start of tail string
  for(short k = 0; k < ix; k++) // Writing to pkdFile
  {
    fprintf(unpkdF, "%c", els[k]);
  } // Writing to unpkdFile

  fclose(unpkdF);
  fclose(pkdF);

  return 0;
}

int AllNormalASCIISymbsCreate()
{
  FILE* pF = fopen("file6.txt", "wb");
  if(!pF)
    return -1;

  for(unsigned char i = 0; i < 128; i++)
    fprintf(pF, "%c", (char)i);

  fclose(pF);

  return 0;
}
