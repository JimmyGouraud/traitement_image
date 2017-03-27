#include <stdlib.h>
#include <stdio.h>

#include "morphology.h"

void add(char* ims1_name, char* ims2_name, char* imd_name) {
  pnm ims1 = pnm_load(ims1_name);
  pnm ims2 = pnm_load(ims2_name);

  int width = pnm_get_width(ims1);
  int height = pnm_get_height(ims1);
  pnm imd = pnm_new(width, height, PnmRawPpm);
  
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      for (int k = 0; k < 3; ++k) {
        unsigned short value = pnm_get_component(ims1, i, j, k) + pnm_get_component(ims2, i, j, k);
        if (value > 255) { value = 255; }
        pnm_set_component(imd, i, j, k, value);
      }
    }
  }

  pnm_save(imd, PnmRawPpm, imd_name);
  
  pnm_free(ims1);
  pnm_free(ims2);
  pnm_free(imd);
}

void usage(char* s)
{
  fprintf(stderr,"%s <ims-1> <ims-2> <imd>\n",s);
  exit(EXIT_FAILURE);
}

#define PARAM 3
int main(int argc, char* argv[])
{
  if(argc != PARAM+1)
  usage(argv[0]);

  add(argv[1], argv[2], argv[3]);

  return EXIT_SUCCESS;
}
