#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <bcl.h>

static pnm float_to_pnm(float* values_ims, int width, int height)
{
  pnm ims = pnm_new(width, height, PnmRawPpm);
  
  unsigned short value;
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      int index = i * width + j;
      if (values_ims[index] < 0) {
	value = 0;
      } else if (values_ims[index] > 255) {
	value = 255;
      } else {
	value = (unsigned short) round(values_ims[index]);
      }
      for (int k = 0; k < 3; ++k) {
	pnm_set_component(ims, i, j, k, value);
      }
    }
  }

  return ims;
}


static float* pnm_to_float(pnm ims)
{
  int height = pnm_get_height(ims);
  int width = pnm_get_width(ims);
  
  float* values_ims = malloc(height * width * sizeof(float));
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      values_ims[i * width + j] = (float) pnm_get_component(ims, i, j, 0);
    }
  }
  
  return values_ims;
}


static int dist(int i, int j, int i2, int j2)
{
  return abs(i - i2 + j - j2);
}

static void process(int sigma, char* ims_name, char* imd_name)
{
  pnm ims = pnm_load(ims_name);
  int width = pnm_get_width(ims);
  int height = pnm_get_height(ims);
  
  int hs = 5;
  int r = 3;
  int size_r = 7; // (2 * 3 + 1)
  int precompute_sigma = 2 * sigma * sigma;
  
  float* ims_values = pnm_to_float(ims);
  float* imd_values = malloc(width * height * sizeof(float));

  // On parcourt chaque pixel
  for (int pi = 0; pi < height; ++pi) {
    for (int pj = 0; pj < width; ++pj) {

      float value_c = 0;
      float value_sum = 0;
      // On parcourt chaque voisin
      for (int qi = pi - hs; qi <= pi + hs; ++qi) {
	if (qi < 0 || qi >= height) { continue; }
	for (int qj = pj - hs; qj <= pj + hs; ++qj) {
	  if (qj < 0 || qj >= width) { continue; }

	  int nb_pixel = 0;
	  float value_d = 0;
	  // On parcourt chaque pixel du patch
	  for (int u = -r; u <= r; ++u) {
	    if (pi + u < 0 || pi + u >= height ||
		qi + u < 0 || qi + u >= height) { continue; }
	    for (int v = -r; v <= r; ++v) {
	      if (pj + v < 0 || pj + v >= width ||
		  qj + v < 0 || qj + v >= width) { continue; }
	      
	      nb_pixel++;

	      int index_pi = (pi + u) * width + pj + v;
	      int index_qi = (qi + u) * width + qj + v;
	      value_d += (ims_values[index_pi] - ims_values[index_qi]) *
		(ims_values[index_pi] - ims_values[index_qi]);
	    }
	  }

	  value_d /= nb_pixel;
	  float w = exp(-value_d/precompute_sigma);
	  value_c += w;
	  value_sum += w * ims_values[qi * width + qj];
	}
      }
      
      imd_values[pi * width + pj] = value_sum / value_c;
    }
  }
  
  pnm imd = float_to_pnm(imd_values, width, height);
  pnm_save(imd, PnmRawPpm, imd_name);

  // Free memory
  pnm_free(ims);
  pnm_free(imd);
  free(ims_values);
  free(imd_values);
}


void usage (char *s)
{
  fprintf(stderr, "Usage: %s <sigma> <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 3
int main(int argc, char *argv[])
{
  if (argc != PARAM+1) {
    usage(argv[0]);
  }
  
  process(atoi(argv[1]), argv[2], argv[3]);

  return EXIT_SUCCESS;
}
