#include <stdlib.h>
#include <stdio.h>

#include "morphology.h"

void subtract(int min, int max, char* ims_name, char* imd_name) {
  pnm ims = pnm_load(ims_name);

  int width = pnm_get_width(ims);
  int height = pnm_get_height(ims);
  pnm imd = pnm_new(width, height, PnmRawPpm);

  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      for (int k = 0; k < 3; ++k) {
        unsigned short value = pnm_get_component(ims, i, j, k);
        if (value < min || value > max) {
          pnm_set_component(imd, i, j, k, 0);
        } else {
          pnm_set_component(imd, i, j, k, 255);
        }
      }
    }
  }

  pnm_save(imd, PnmRawPpm, imd_name);
  
  pnm_free(ims);
  pnm_free(imd);
}

void usage(char* s)
{
  fprintf(stderr,"%s <min> <max> <ims> <imd>\n",s);
  exit(EXIT_FAILURE);
}

#define PARAM 4
int main(int argc, char* argv[])
{
  if(argc != PARAM+1)
  usage(argv[0]);

  subtract(atoi(argv[1]), atoi(argv[2]), argv[3], argv[4]);

  return EXIT_SUCCESS;
}
