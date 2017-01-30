/**
 * @file test-fft.c
 * @brief test the behaviors of functions in fft module
 *
 */
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "fft.h"

/**
 * @brief test the forward and backward functions
 * @param pnm ims, the source image
 * @param char* name, the image file name
 */
static void
test_for_backward(pnm ims, char* name)
{
  fprintf(stderr, "test_for_backward: ");
  
  int rows = pnm_get_height(ims);
  int cols = pnm_get_width(ims);

  unsigned short* channel = malloc(rows * cols * sizeof(unsigned short));
  pnm_get_channel(ims, channel, 0);

  fftw_complex* fftw_forward = forward(rows, cols, channel);
  unsigned short* fftw_backward = backward(rows, cols, fftw_forward);

  pnm imd = pnm_new(cols, rows, PnmRawPpm);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      for (int k = 0; k < 3; k++) {
	pnm_set_component(imd, i, j, k, *fftw_backward);
      }
      fftw_backward++;
    }
  }
  fftw_backward -= cols * rows;

  char * origin_name = strrchr(name,'/') + 1;
  int size = strlen(origin_name) + 4;
  char namefile[size];
  snprintf(namefile, size, "FB-%s", origin_name);

  pnm_save(imd, PnmRawPpm, namefile);

  free(channel);
  free(fftw_forward);
  free(fftw_backward);
  pnm_free(imd);
  
  fprintf(stderr, "OK\n");
}


/**
 * @brief test image reconstruction from of amplitude and phase spectrum
 * @param pnm ims: the source image
 * @param char *name: the image file name
 */
static void
test_reconstruction(pnm ims, char* name)
{
  fprintf(stderr, "test_reconstruction: ");
  
  int rows = pnm_get_height(ims);
  int cols = pnm_get_width(ims);

  unsigned short* channel = malloc(rows * cols * sizeof(unsigned short));
  pnm_get_channel(ims, channel, 0);

  fftw_complex* fftw_forward = forward(rows, cols, channel);
  
  float *as = malloc(rows * cols * sizeof(float));
  float *ps = malloc(rows * cols * sizeof(float));

  freq2spectra(rows, cols, fftw_forward, as, ps);
  spectra2freq(rows, cols, as, ps, fftw_forward);

  unsigned short* fftw_backward = backward(rows, cols, fftw_forward);

  pnm imd = pnm_new(cols, rows, PnmRawPpm);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      for (int k = 0; k < 3; k++) {
	pnm_set_component(imd, i, j, k, *fftw_backward);
      }
      fftw_backward++;
    }
  }
  fftw_backward -= cols * rows;

  char * origin_name = strrchr(name,'/') + 1;
  int size = strlen(origin_name) + 9;
  char namefile[size];
  snprintf(namefile, size, "FB-ASPS-%s", origin_name);

  pnm_save(imd, PnmRawPpm, namefile);

  free(channel);
  free(fftw_forward);
  free(fftw_backward);
  pnm_free(imd);
  free(as);
  free(ps);

  printf("%s", name);
  fprintf(stderr, "OK\n");
}


/**
 * @brief test construction of amplitude and phase images in ppm files
 * @param pnm ims, the source image
 * @param char* name, the image file name
 */
static void
test_display(pnm ims, char* name)
{
  fprintf(stderr, "test_display: ");
  (void)ims;
  (void)name;
  fprintf(stderr, "OK\n");
}

/**
 * @brief test the modification of amplitude and phase spectrum and 
 *        construct output images
 * @param pnm ims, the source image
 * @param char* name, the image file name
 */
static void
test_modification(pnm ims, char* name)
{
  fprintf(stderr, "test_modification: ");
  (void)ims;
  (void)name;
  fprintf(stderr, "OK\n");
}


static void 
run(pnm ims, char* name)
{
  test_for_backward(ims, name);
  test_reconstruction(ims, name);
  test_display(ims, name);
  test_modification(ims, name);
}


void 
usage (char *s)
{
  fprintf(stderr, "Usage: %s <ims> \n", s);
  exit(EXIT_FAILURE);
}

int 
main(int argc, char *argv[])
{
  if (argc != 2)
    usage(argv[0]);
  
  pnm ims = pnm_load(argv[1]);
  run(ims, argv[1]);
  
  pnm_free(ims);

  return EXIT_SUCCESS;
}
