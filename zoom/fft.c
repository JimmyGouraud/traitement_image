#include <float.h>
#include <stdlib.h>
#include <math.h>

#include "fft.h"

fftw_complex
*forward(int rows, int cols, unsigned short* gray_img)
{
  // 1. Construction image complexe
  fftw_complex* in = fftw_malloc(rows * cols * sizeof(fftw_complex));
  float value_centered;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      	value_centered = 1;
	if ((i+j) % 2 != 0){
	  value_centered *= -1;
	}
	*in++ = (double) value_centered * (*gray_img++ + I * 0);
    }
  }
  in -= cols * rows;
  gray_img -= cols * rows;

  
  fftw_complex* out = fftw_malloc(rows * cols * sizeof(fftw_complex));
  fftw_plan plan = fftw_plan_dft_2d(rows, cols, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

  fftw_execute(plan);

  // Free memory
  fftw_destroy_plan(plan);
  free(in);
  
  return out;
}


unsigned short 
*backward(int factor, int rows, int cols, fftw_complex* freq_repr)
{
  fftw_complex* out = fftw_malloc(rows * cols * sizeof(fftw_complex));
  fftw_plan plan = fftw_plan_dft_2d(rows, cols, freq_repr, out, FFTW_BACKWARD, FFTW_ESTIMATE);
  
  fftw_execute(plan);

  unsigned short* gray_img = fftw_malloc(rows * cols * sizeof(unsigned short));
  float value_centered;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      value_centered = (factor * factor * creal(*out++)) / (rows * cols);
      if ((i+j) % 2 != 0) {
	value_centered *= -1;
      }

      if (value_centered < 0) {
	value_centered = 0;
      } else if (value_centered > 255) {
	value_centered = 255;
      }
      
      *gray_img++ = (unsigned int) value_centered;
    }
  }
  gray_img -= cols * rows;
  out -= cols * rows;

  // Free memory
  fftw_destroy_plan(plan);
  free(out);
  
  return gray_img;
}

void
freq2spectra(int rows, int cols, fftw_complex* freq_repr, float* as, float* ps) 
{
  for (int i = 0; i < rows; i++){
    for (int j = 0; j < cols; j++){ 
      *as++ = (float) cabs(*freq_repr); // calcule la valeur absolue
      *ps++ = (float) carg(*freq_repr); // calcule l'argument
      freq_repr++;
    }
  }
  as -= rows * cols;
  ps -= rows * cols;
  freq_repr -= rows * cols;
}


void 
spectra2freq(int rows, int cols, float* as, float* ps, fftw_complex* freq_repr)
{
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) { 
      *freq_repr++ = *as * cos(*ps) + I * *as * sin(*ps);
      as++;
      ps++;
    }
  }
  as -= rows * cols;
  ps -= rows * cols;
  freq_repr -= rows * cols;
}
