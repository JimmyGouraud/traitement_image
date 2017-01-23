#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h> 
#include <bcl.h>
#include "fft.h"


static float lowpass(int u, int v, int d0, int n, int w, int u0, int v0){
  (void)u;
  (void)v;
  (void)d0;
  (void)n;
  (void)w;
  (void)u0;
  (void)v0;  
  return 0.0;
}

static float highpass(int u, int v, int d0, int n, int w, int u0, int v0){
  (void)u;
  (void)v;
  (void)d0;
  (void)n;
  (void)w;
  (void)u0;
  (void)v0;  
  return 0.0;
}

static float bandreject(int u, int v, int d0, int n, int w, int u0, int v0){
  (void)u;
  (void)v;
  (void)d0;
  (void)n;
  (void)w;
  (void)u0;
  (void)v0;  
  return 0.0;
}

static float bandpass(int u, int v, int d0, int n, int w, int u0, int v0){
  (void)u;
  (void)v;
  (void)d0;
  (void)n;
  (void)w;
  (void)u0;
  (void)v0;  
  return 0.0;
}

static float notch(int u, int v, int d0, int n, int w, int u0, int v0){
  (void)u;
  (void)v;
  (void)d0;
  (void)n;
  (void)w;
  (void)u0;
  (void)v0;  
  return 0.0;
}

static void 
process(char* inp, char* out, 
	int d0, int nx2, int ww, int u0, int v0,
	float (*apply)(int, int, int, int, int, int, int))  
{
  (void)inp;
  (void)out;
  (void)d0;
  (void)nx2;
  (void)ww;
  (void)u0;
  (void)v0;
  (void)apply;
  
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
  char* ims     = argv[idx++];
  char* imd     = argv[idx++];
  char* f_name  = argv[idx++];
  int   d0      = atoi(argv[idx++]);
  int   n       = atoi(argv[idx++]);
  int   w       = atoi(argv[idx++]);
  int   u0      = atoi(argv[idx++]);
  int   v0      = atoi(argv[idx++]);
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
