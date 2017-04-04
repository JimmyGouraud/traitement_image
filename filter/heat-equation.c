#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <bcl.h>

static pnm float_to_pnm(int rows, int cols, float* values_ims)
{
  pnm ims = pnm_new(cols, rows, PnmRawPpm);
  unsigned short value;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      int index = (i * cols + j) * 3;
      for (int k = 0; k < 3; ++k) {
	if (values_ims[index + k] < 0) {
	  value = 0;
	} else if (values_ims[index + k] > 255) {
	  value = 255;
	} else {
	  value = (unsigned short) round(values_ims[index + k]);
	}
	pnm_set_component(ims, i, j, k, value);
      }
    }
  }

  return ims;
}

static float* pnm_to_float(pnm ims)
{
  int rows = pnm_get_height(ims);
  int cols = pnm_get_width(ims);
  
  float* values_ims = malloc(rows * cols * 3 * sizeof(float));
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      int index = (i * cols + j) * 3;
      for (int k = 0; k < 3; ++k) {
	values_ims[index + k] = (float) pnm_get_component(ims, i, j, k);
      }
    }
  }
  
  return values_ims;
}

static float laplacien(float* values_ims, int width, int height, int i, int j)
{
  int index = (i * width + j) * 3;
  
  float value = -4 * values_ims[index];
  value += (i == 0) ? values_ims[index] : values_ims[index - width * 3];
  value += (i == height - 1) ? values_ims[index] : values_ims[index + width * 3];
  value += (j == 0) ? values_ims[index] :  values_ims[index - 1 * 3];
  value += (j == width - 1) ? values_ims[index] :  values_ims[index + 1 * 3];
    
  return value;
}

static float delta(float* values_ims, int width, int height, int i, int j)
{
  return 0.25 * laplacien(values_ims, width, height, i, j) + values_ims[(i * width + j) * 3];
}

static void process(int n, char* ims_name, char* imd_name)
{
  pnm ims = pnm_load(ims_name);
  int width = pnm_get_width(ims);
  int height = pnm_get_height(ims);
  
  float* values_imd = pnm_to_float(ims);
  for (; n > 0; --n) {
    float* values_tmp = pnm_to_float(ims);
    for (int i = 0; i < height; ++i) {
      for (int j = 0; j < width; ++j) {
	float value = delta(values_imd, width, height, i, j);
	for (int k = 0; k < 3; ++k) {
	  values_tmp[(i * width + j) * 3 + k] = value; 
	}
      }
    }
    free(values_imd);
    values_imd = values_tmp;
  }
    
  pnm imd = float_to_pnm(height, width, values_imd);
  pnm_save(imd, PnmRawPpm, imd_name);
  
  pnm_free(ims);
  pnm_free(imd);
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
