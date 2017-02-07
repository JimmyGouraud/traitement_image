/**
 * @file padding.c
 * @author Jimmy Gouraud 
 * @brief zoom
 */

#include <stdlib.h>
#include <stdio.h>
#include "fft.h"

static pnm create_imd (int rows, int cols, unsigned short *fftw_backward)
{
  pnm imd = pnm_new(cols, rows, PnmRawPpm);
  
  for (int k = 0; k < 3; k++) {
    pnm_set_channel(imd, fftw_backward, k);
  }

  return imd;
}

void padding(int factor, char* ims_name, char* imd_name)
{
  pnm ims = pnm_load(ims_name);
  int rows = pnm_get_height(ims);
  int cols = pnm_get_width(ims);
  
  unsigned short* channel = malloc(rows * cols * sizeof(unsigned short));
  pnm_get_channel(ims, channel, 0);
  
  fftw_complex* fftw_forward = forward(rows, cols, channel);

  int imd_rows = factor * rows;
  int imd_cols = factor * cols;
  fftw_complex* fftw_imd = fftw_malloc(imd_cols * imd_rows * sizeof(fftw_complex));
  for (int i = 0; i < imd_rows; ++i) {
    for (int j = 0; j < imd_cols; ++j) {
      if (i <  (imd_cols + cols)/2 &&
 	  i >= (imd_cols - cols)/2 &&
	  j <  (imd_rows + rows)/2 &&
	  j >= (imd_rows - rows)/2) {
	*fftw_imd++ = *fftw_forward++;
      } else {
	*fftw_imd++ = 0 + I * 0;
      }
    }
  }
  fftw_imd -= (imd_cols * imd_rows);
  fftw_forward -= (rows * cols);

  unsigned short* fftw_backward = backward(factor, imd_rows, imd_cols, fftw_imd);

  pnm imd = create_imd(imd_rows, imd_cols, fftw_backward);
  pnm_save(imd, PnmRawPpm, imd_name);

  // Free memory
  free(channel);
  free(fftw_imd);
  free(fftw_forward);
  free(fftw_backward);
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
  
  padding(factor, ims_name, imd_name);
  
  return EXIT_SUCCESS;
}

