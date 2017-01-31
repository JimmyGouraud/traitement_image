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
 * @brief create a filename with prefix and name (without the path)
 * @param char* prefix, the prefix to add to the image file name
 * @param char* path, the image file name with the path
 */
static char* create_filename(char* prefix, char* path)
{
  char* origin_name = strrchr(path,'/') + 1;
  int size = strlen(prefix) + strlen(origin_name) + 1;
  char* namefile = malloc(size * sizeof(char*));
  snprintf(namefile, size, "%s%s", prefix, origin_name);
  
  return namefile;
}

/**
 * @brief recentering the image
 * @param int rows, the rows image
 * @param int cols, the cols image
 * @param pnm ims, the source image
 */
static pnm recentering (int rows, int cols, pnm ims)
{
  pnm ims_centered = pnm_new(cols, rows, PnmRawPpm);
  unsigned short value_centered;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      for (int k = 0; k < 3; k++) {
	value_centered = pnm_get_component(ims, i, j, k);
	if ((i+j) % 2 != 0){
	  value_centered *= -1;
	}
	pnm_set_component(ims_centered, i, j, k, value_centered);
      }
    }
  }
  
  return ims_centered;
}



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

  char * namefile = create_filename("FB-", name);
  pnm_save(imd, PnmRawPpm, namefile);

  free(channel);
  free(fftw_forward);
  free(fftw_backward);
  free(namefile);
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
  free(as);
  free(ps);
  
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
  
  char * namefile = create_filename("FB-ASPS-", name);
  pnm_save(imd, PnmRawPpm, namefile);

  free(channel);
  free(fftw_forward);
  free(fftw_backward);
  free(namefile);
  pnm_free(imd);

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
  int rows = pnm_get_height(ims);
  int cols = pnm_get_width(ims);

  // Recentrage
  pnm ims_centered = recentering(cols, rows, ims);
  (void)ims_centered;
  unsigned short* channel = malloc(rows * cols * sizeof(unsigned short));
  pnm_get_channel(ims, channel, 0);

  fftw_complex* fftw_forward = forward(rows, cols, channel);
  
  float *as = malloc(rows * cols * sizeof(float));
  float *ps = malloc(rows * cols * sizeof(float));
  freq2spectra(rows, cols, fftw_forward, as, ps);

  pnm imd_as = pnm_new(cols, rows, PnmRawPpm);
  pnm imd_ps = pnm_new(cols, rows, PnmRawPpm);

  // Trouve l'amplitude max
  float max_amplitude = 0;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (max_amplitude < *as) {
	max_amplitude = *as;
      }
      as++;
    }
  }
  as -= rows * cols;

  
  float as_normalized;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      as_normalized = pow(*as/max_amplitude, 0.15) * 255;
      for (int k = 0; k < 3; k++) {
	pnm_set_component(imd_as, i, j, k, as_normalized);
	pnm_set_component(imd_ps, i, j, k, *ps);
      }
      as++;
      ps++;
    }
  }
  as -= cols * rows;
  ps -= cols * rows;

  char * namefile_as = create_filename("AS-", name);
  pnm_save(imd_as, PnmRawPpm, namefile_as);
  char * namefile_ps = create_filename("PS-", name);
  pnm_save(imd_ps, PnmRawPpm, namefile_ps);

  // Free memories
  pnm_free(imd_as);
  pnm_free(imd_ps);
  pnm_free(ims_centered);
  free(namefile_as);
  free(namefile_ps);
  free(channel);
  free(fftw_forward);
  free(as);
  free(ps);
  
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
