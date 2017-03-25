#include <stdlib.h>
#include <stdio.h>

#include "morphology.h"

void erosion(int shape, int hs, char* ims_name, char* imd_name) {
  pnm ims = pnm_load(ims_name);
  
  pnm imd = pnm_new(pnm_get_width(ims), pnm_get_height(ims), PnmRawPpm);
  process(shape, hs, ims, imd, lesser);
  
  pnm_save(imd, PnmRawPpm, imd_name);  
  pnm_free(imd);
}

void usage(char* s)
{
  fprintf(stderr,"%s <shape> <halfsize> <ims> <imd>\n",s);
  exit(EXIT_FAILURE);
}

#define PARAM 4
int main(int argc, char* argv[])
{
  if(argc != PARAM+1)
    usage(argv[0]);
  
  erosion(atoi(argv[1]), atoi(argv[2]), argv[3], argv[4]);

  return EXIT_SUCCESS;
}
