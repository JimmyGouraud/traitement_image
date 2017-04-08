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


static float laplacien(float* values_ims, int width, int height, int i, int j)
{
  int index = i * width + j;

  float value = -4 * values_ims[index];
  value += (i == 0)          ? values_ims[index] : values_ims[index - width];
  value += (i == height - 1) ? values_ims[index] : values_ims[index + width];
  value += (j == 0)          ? values_ims[index] : values_ims[index - 1];
  value += (j == width - 1)  ? values_ims[index] : values_ims[index + 1];
    
  return value;
}

static void diffusion(float* values_imd, float* values_tmp, int width, int height)
{
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      values_imd[i * width + j] += 0.25 * laplacien(values_tmp, width, height, i, j);
    }
  }
}

static void copy_values(float* values_ims, float* values_imd, int width, int height)
{
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      int index = i * width + j;
      values_imd[index] = values_ims[index];
    }
  }
}

static void process(int n, char* ims_name, char* imd_name)
{
  pnm ims = pnm_load(ims_name);
  int width = pnm_get_width(ims);
  int height = pnm_get_height(ims);
  
  float* values_imd = pnm_to_float(ims);
  float* values_imd_tmp = malloc(height * width * sizeof(float));
  for (; n > 0; --n) {
    copy_values(values_imd, values_imd_tmp, width, height);
    diffusion(values_imd, values_imd_tmp, width, height);
  }
    
  pnm imd = float_to_pnm(values_imd, width, height);
  pnm_save(imd, PnmRawPpm, imd_name);
  
  pnm_free(ims);
  pnm_free(imd);
  free(values_imd);
  free(values_imd_tmp);
}


void usage (char *s)
{
  fprintf(stderr, "Usage: %s <n> <ims> <imd>\n", s);
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
