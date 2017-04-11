#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h> 
#include <bcl.h>
#include "fft.h"


static float lowpass(int u, int v, int d0, int n, int w, int u0, int v0)
{
  (void) w;
  (void) u0;
  (void) v0;

  float duv = sqrt(u*u + v*v);
  return 1 / (1 + pow(duv / d0, 2*n));
}

static float highpass(int u, int v, int d0, int n, int w, int u0, int v0)
{
  (void) w;
  (void) u0;
  (void) v0;

  float duv = sqrt(u*u + v*v);
  return 1 / (1 + pow(d0 / duv, 2*n));
}

static float bandreject(int u, int v, int d0, int n, int w, int u0, int v0)
{
  (void) u0;
  (void) v0;
  
  float duv = sqrt(u*u + v*v);
  return 1 / (1 + pow((duv * w) / (duv*duv - d0*d0), 2*n));
}

static float bandpass(int u, int v, int d0, int n, int w, int u0, int v0)
{
  return 1 - bandreject(u, v, d0, n, w, u0, v0);
}

static float notch(int u, int v, int d0, int n, int w, int u0, int v0)
{
  (void)w;
  
  float d1uv = sqrt((u-u0)*(u-u0) + (v-v0)*(v-v0));
  float d2uv = sqrt((u+u0)*(u+u0) + (v+v0)*(v+v0));
  return 1 / (1 + pow(d0*d0 / (d1uv*d2uv), 2*n));
}

static pnm create_imd (int rows, int cols, unsigned short *fftw_backward)
{
  pnm imd = pnm_new(cols, rows, PnmRawPpm);
  
  for (int k = 0; k < 3; k++) {
    pnm_set_channel(imd, fftw_backward, k);
  }

  return imd;
}

static void 
process(char* ims_name, char* imd_name, 
	int d0, int n, int w, int u0, int v0,
	float (*apply)(int, int, int, int, int, int, int))  
{
  pnm ims = pnm_load(ims_name);

  int height = pnm_get_height(ims);
  int width = pnm_get_width(ims);
  
  unsigned short* channel = malloc(height * width * sizeof(unsigned short));
  pnm_get_channel(ims, channel, 0);

  fftw_complex* fftw_forward = forward(height, width, channel);
  
  float *as = malloc(height * width * sizeof(float));
  float *ps = malloc(height * width * sizeof(float));
  freq2spectra(height, width, fftw_forward, as, ps);
  
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      as[i * width + j] *= apply(j - width/2, i - height/2, d0, n, w, u0, v0);
    }
  }
  
  spectra2freq(height, width, as, ps, fftw_forward);
  
  unsigned short* fftw_backward = backward(height, width, fftw_forward);
  pnm imd = create_imd(height, width, fftw_backward);

  pnm_save(imd, PnmRawPpm, imd_name);

  // Free memory
  pnm_free(ims);
  pnm_free(imd);
  free(as);
  free(ps);
  fftw_free(fftw_forward);
  fftw_free(fftw_backward);
  free(channel);
}

void usage (char *s){
  fprintf(stderr, "Usage: %s <ims> <imd> <filter> ", s);
  fprintf(stderr, "<d0> <n> <w> <u0> <v0>\n");
  exit(EXIT_FAILURE);
}

#define param 8
int main(int argc, char *argv[]){
  if (argc != param+1) 
    usage(argv[0]);
  int idx=1;
  char* ims = argv[idx++];
  char* imd = argv[idx++];
  char* f_name = argv[idx++];
  int d0 = atoi(argv[idx++]);
  int n = atoi(argv[idx++]);
  int w = atoi(argv[idx++]);
  int u0 = atoi(argv[idx++]);
  int v0 = atoi(argv[idx++]);
  n*=2;
  if(strcmp(f_name, "lp")==0){
    process(ims, imd, d0, n, w, u0, v0, &lowpass);
  }else if(strcmp(f_name, "hp")==0){
    process(ims, imd, d0, n, w, u0, v0, &highpass);
  }else if(strcmp(f_name, "br")==0){
    process(ims, imd, d0, n, w, u0, v0, &bandreject);
  }else if(strcmp(f_name, "bp")==0){
    process(ims, imd, d0, n, w, u0, v0, &bandpass);
  }else if(strcmp(f_name, "no")==0){
    process(ims, imd, d0, n, w, u0, v0, &notch);
  }else
    usage(argv[0]);
  return EXIT_SUCCESS;
}
