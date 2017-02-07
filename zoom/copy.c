/**
 * @file copy.c
 * @author Jimmy Gouraud 
 * @brief zoom
 */

#include <stdlib.h>
#include <stdio.h>
#include "fft.h"

void copy(int factor, char* ims_name, char* imd_name)
{
  pnm ims = pnm_load(ims_name);
  int rows = pnm_get_height(ims);
  int cols = pnm_get_width(ims);
  pnm imd = pnm_new(cols*factor, rows*factor, PnmRawPpm);
  
  unsigned short value;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      for (int k = 0; k < 3; ++k) {
	value = pnm_get_component(ims, i, j, k);
	for (int i2 = 0; i2 < factor; ++i2) {
	  for (int j2 = 0; j2 < factor; ++j2) {
	    pnm_set_component(imd, i*factor + i2, j*factor + j2, k, value);
	  }
	}
      }
    }
  }
  
  pnm_save(imd, PnmRawPpm, imd_name);

  // Free memory
  pnm_free(ims);
  pnm_free(imd);
}


void usage (char *s){
  fprintf(stderr, "Usage: %s <factor> <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 3
int main (int argc, char *argv[]){
  if (argc != PARAM+1) usage(argv[0]);

  int factor = atoi(argv[1]);
  char* ims_name = argv[2];
  char* imd_name = argv[3];
  
  copy(factor, ims_name, imd_name);
  
  return EXIT_SUCCESS;
}

