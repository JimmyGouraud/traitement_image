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


static pnm create_imd (int rows, int cols, unsigned short *fftw_backward)
{
  pnm imd = pnm_new(cols, rows, PnmRawPpm);
  
  for (int k = 0; k < 3; k++) {
    pnm_set_channel(imd, fftw_backward, k);
  }

  return imd;
}
  

static float find_max (int rows, int cols, float *as)
{
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
  
  return max_amplitude;
}

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

  pnm imd = pnm_new(rows, cols, PnmRawPpm);
  
  for (int k = 0; k < 3; k++) {
    unsigned short* channel = malloc(rows * cols * sizeof(unsigned short));
    pnm_get_channel(ims, channel, k);
    
    fftw_complex* fftw_forward = forward(rows, cols, channel);
    unsigned short* fftw_backward = backward(rows, cols, fftw_forward);
    
    pnm_set_channel(imd, fftw_backward, k);

    // Free memory
    free(channel);
    free(fftw_forward);
    free(fftw_backward);
  }
  

  char * namefile = create_filename("FB-", name);
  pnm_save(imd, PnmRawPpm, namefile);

  // Free memory
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

  pnm imd = pnm_new(rows, cols, PnmRawPpm);

  for (int k = 0; k < 3; k++) {
    unsigned short* channel = malloc(rows * cols * sizeof(unsigned short));
    pnm_get_channel(ims, channel, k);

    fftw_complex* fftw_forward = forward(rows, cols, channel);
  
    float *as = malloc(rows * cols * sizeof(float));
    float *ps = malloc(rows * cols * sizeof(float));
    freq2spectra(rows, cols, fftw_forward, as, ps);
    spectra2freq(rows, cols, as, ps, fftw_forward);
  
    unsigned short* fftw_backward = backward(rows, cols, fftw_forward);

    pnm_set_channel(imd, fftw_backward, k);
    
    // Free memory
    free(as);
    free(ps);
    free(channel);
    free(fftw_forward);
    free(fftw_backward);
  }
  //pnm imd = create_imd(rows, cols, fftw_backward);

  char * namefile = create_filename("FB-ASPS-", name);
  pnm_save(imd, PnmRawPpm, namefile);

  // Free memory
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

  pnm imd_as = pnm_new(cols, rows, PnmRawPpm);
  pnm imd_ps = pnm_new(cols, rows, PnmRawPpm);
  
  for (int k = 0; k < 3; k++) {
    unsigned short* channel = malloc(rows * cols * sizeof(unsigned short));
    pnm_get_channel(ims, channel, k);

    fftw_complex* fftw_forward = forward(rows, cols, channel);
  
    float *as = malloc(rows * cols * sizeof(float));
    float *ps = malloc(rows * cols * sizeof(float));
    freq2spectra(rows, cols, fftw_forward, as, ps);

    //pnm imd_as = pnm_new(cols, rows, PnmRawPpm);
    //pnm imd_ps = pnm_new(cols, rows, PnmRawPpm);

    // Trouve l'amplitude max
    float max_amplitude = find_max(rows, cols, as);
    float max_phase = find_max(rows, cols, ps);
    float as_normalized;
    float ps_normalized;
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
	as_normalized = pow(*as/max_amplitude, 0.15) * 255;
	if (*ps < 0) {
	  ps_normalized = 0;
	} else {
	  ps_normalized = *ps / max_phase * 255;
	}

	for (int k2 = 0; k2 < 3; k2++) {
	  pnm_set_component(imd_as, i, j, k2, (unsigned short) as_normalized);
	  pnm_set_component(imd_ps, i, j, k2, (unsigned short) ps_normalized);
	}
	as++;
	ps++;
      }
    }
    as -= cols * rows;
    ps -= cols * rows;


    int size1 = strlen("AS-1-") + 1;
    char* name_as = malloc(size1 * sizeof(char));
    snprintf(name_as, size1, "AS-%d-", k);
    char * namefile_as = create_filename(name_as, name);
    pnm_save(imd_as, PnmRawPpm, namefile_as);

    int size2 = strlen("PS-1-") + 1;
    char* name_ps = malloc(size2 * sizeof(char));
    snprintf(name_ps, size2, "AP-%d-", k);
    char * namefile_ps = create_filename(name_ps, name);
    pnm_save(imd_ps, PnmRawPpm, namefile_ps);

  }
  
  // Free memory
  pnm_free(imd_as);
  pnm_free(imd_ps);
  
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
  
  int rows = pnm_get_height(ims);
  int cols = pnm_get_width(ims);
  unsigned short* channel = malloc(rows * cols * sizeof(unsigned short));
  pnm_get_channel(ims, channel, 0);

  fftw_complex* fftw_forward = forward(rows, cols, channel);
  
  float *as = malloc(rows * cols * sizeof(float));
  float *ps = malloc(rows * cols * sizeof(float));
  freq2spectra(rows, cols, fftw_forward, as, ps);
  
  float max_amplitude = find_max(rows, cols, as);
  pnm imd_fas = pnm_new(cols, rows, PnmRawPpm);
  int mean_i = rows/2;
  int mean_j = cols/2;
  float as_normalized;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if ((i == mean_i && (j == mean_j + 8 || j == mean_j - 8)) ||
	  (j == mean_j && (i == mean_i + 8 || i == mean_i - 8))) {
	*as = 0.25 * max_amplitude;
      }
      as_normalized = pow(*as/max_amplitude, 0.15) * 255;
      for (int k = 0; k < 3; k++) {	
	pnm_set_component(imd_fas, i, j, k, (unsigned short) as_normalized);
      }
      
      as++;
    }
  }
  as -= rows * cols;
  
  spectra2freq(rows, cols, as, ps, fftw_forward);
  
  unsigned short* fftw_backward = backward(rows, cols, fftw_forward);
  pnm imd_freq = create_imd(rows, cols, fftw_backward);

  char * namefile_freq = create_filename("FREQ-", name);
  pnm_save(imd_freq, PnmRawPpm, namefile_freq);
  char * namefile_fas = create_filename("FAS-", name);
  pnm_save(imd_fas, PnmRawPpm, namefile_fas);

  // Free memory
  free(channel);
  free(fftw_forward);
  free(as);
  free(ps);
  free(fftw_backward);
  free(namefile_freq);
  free(namefile_fas);
  pnm_free(imd_freq);
  pnm_free(imd_fas);
  
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
