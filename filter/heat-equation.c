#include <stdlib.h>
#include <stdio.h>


#include <bcl.h>


static void 
process(int n, char* ims_name, char* imd_name)
{
  (void) n;
  (void) ims_name;
  (void) imd_name;
   
}

void usage (char *s){
  fprintf(stderr, "Usage: %s <n> <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 3
int main(int argc, char *argv[]){
  if (argc != PARAM+1) 
    usage(argv[0]);

  int   idx     = 1;
  int   n       = atoi(argv[idx++]);
  char* ims     = argv[idx++];
  char* imd     = argv[idx++];

  process(n, ims, imd);

  return EXIT_SUCCESS;
}
