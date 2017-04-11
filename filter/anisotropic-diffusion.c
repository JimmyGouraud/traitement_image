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

static float c0(int lambda, float s)
{
  return 1;
}

static float c1(int lambda, float s)
{
  return 1/(1 + (s/lambda) * (s/lambda));
}

static float c2(int lambda, float s)
{
  return exp(-(s/lambda) * (s/lambda));
}

static float delta1(float* values_ims, int width, int height, int i, int j)
{
  int index = i * width + j;
  float value = (i == height - 1) ? values_ims[index] : values_ims[index + width];
  return value - values_ims[index];
}

static float delta2(float* values_ims, int width, int height, int i, int j)
{
  int index = i * width + j;
  float value = (j == width - 1)  ? values_ims[index] : values_ims[index + 1];
  return value - values_ims[index];
}

static float norm(float delta_1, float delta_2)
{
  return sqrt(delta_1 * delta_1 + delta_2 * delta_2);
}

static float div_gradient(float* values_tmp, int width, int height, int i, int j)
{
  int index = (i * width + j) * 2;
  float value = values_tmp[index];
  value -= (i == 0) ? values_tmp[index] : values_tmp[index - width * 2];
  value += values_tmp[index + 1];
  value -= (j == 0) ? values_tmp[index + 1] : values_tmp[index - 1 * 2 + 1];

  return value;
}

static void compute_gradient(float* values_imd, float* values_gradient, float (*c)(int, float), int lambda, int width, int height) {
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      float gradient[2] = {delta1(values_imd, width, height, i, j),
			   delta2(values_imd, width, height, i, j)};
      float norm_gradient = norm(gradient[0], gradient[1]);
      float value_c = c(lambda, norm_gradient);
      values_gradient[(i * width + j) * 2] = value_c * gradient[0];
      values_gradient[(i * width + j) * 2 + 1] = value_c * gradient[1];
    }
  }
}

static void diffusion(float* values_imd, float* values_gradient, int lambda, int width, int height)
{
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      values_imd[i * width + j] += 0.25 * div_gradient(values_gradient, width, height, i, j);
    }
  }
}

static void process(int n, int lambda, float (*c)(int, float), char* ims_name, char* imd_name)
{  
  pnm ims = pnm_load(ims_name);
  int width = pnm_get_width(ims);
  int height = pnm_get_height(ims);
  
  float* values_imd = pnm_to_float(ims);
  float* values_gradient = malloc(height * width * 2 * sizeof(float));
  for (; n > 0; --n) {
    compute_gradient(values_imd, values_gradient, c, lambda, width, height);
    diffusion(values_imd, values_gradient, lambda, width, height);
  }
    
  pnm imd = float_to_pnm(values_imd, width, height);
  pnm_save(imd, PnmRawPpm, imd_name);
  
  pnm_free(ims);
  pnm_free(imd);
  
  free(values_imd);
  free(values_gradient);
}


void usage (char *s)
{
  fprintf(stderr, "Usage: %s <n> <lambda> <function> <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 5
int main(int argc, char *argv[])
{
  if (argc != PARAM+1) {
    usage(argv[0]);
  }

  float (*c)(int, float);
  switch(atoi(argv[3])) {
  case 0:
    c = c0;
    break;
  case 1:
    c = c1;
    break;
  case 2:
    c = c2;
    break;
  default:
    fprintf(stderr, "<n> must be between 0 and 2.\n");
    exit(EXIT_FAILURE);
  }
    
  process(atoi(argv[1]), atoi(argv[2]), c, argv[4], argv[5]);

  return EXIT_SUCCESS;
}
