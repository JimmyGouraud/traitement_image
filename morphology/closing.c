#include <stdlib.h>
#include <stdio.h>

#include "morphology.h"

void closing(int shape, int hs, char* ims_name, char* imd_name) {
  pnm ims = pnm_load(ims_name);
  int width = pnm_get_width(ims);
  int height = pnm_get_height(ims);
  
  pnm ims_greater = pnm_new(width, height, PnmRawPpm);
  process(shape, hs, ims, ims_greater, greater);

  pnm imd = pnm_new(width, height, PnmRawPpm);
  process(shape, hs, ims_greater, imd, lesser);

  pnm_save(imd, PnmRawPpm, imd_name);

  pnm_free(ims);
  pnm_free(ims_greater);
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

  if(argc != PARAM+1) {
    usage(argv[0]);
  }

  closing(atoi(argv[1]), atoi(argv[2]), argv[3], argv[4]);

  return EXIT_SUCCESS;
}
