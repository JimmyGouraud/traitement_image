#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <bcl.h>

static pnm us_to_pnm(unsigned short *values_ims, int width, int height)
{
  pnm ims = pnm_new(width, height, PnmRawPpm);
  
  unsigned short value;
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      int index = i * width + j;
      value = values_ims[index] > 255 ? 255 : values_ims[index];
      
      for (int k = 0; k < 3; ++k) {
	pnm_set_component(ims, i, j, k, value);
      }
    }
  }

  return ims;
}


static unsigned short* pnm_to_us(pnm ims)
{
  int height = pnm_get_height(ims);
  int width = pnm_get_width(ims);
  
  unsigned short *values_ims = malloc(height * width * sizeof(float));
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      values_ims[i * width + j] = pnm_get_component(ims, i, j, 0);
    }
  }
  
  return values_ims;
}


static float gauss(float k, int sigma)
{
  return exp(-(k * k)/(2 * sigma * sigma));
}


static int dist(int i, int j, int i2, int j2)
{
  return abs(i - i2 + j - j2);
}


static int compute_hs(int precompute_sigma_s)
{
  double dist_gauss;
  int i = 0;
  do {
    dist_gauss = gauss(dist(0,0,i,i), precompute_sigma_s);
    i++;
  } while (dist_gauss > 0.1);
  
  return i;
}


static float* precompute_exp(int size, int sigma)
{  
  float* exp_values = malloc(size * sizeof(float));
  
  for (int i = 0; i < size; ++i) {
    exp_values[i] = gauss(i, sigma);
  }
  
  return exp_values;
}


static void process(int sigma_s, int sigma_g, char* ims_name, char* imd_name)
{
  pnm ims = pnm_load(ims_name);
  int width = pnm_get_width(ims);
  int height = pnm_get_height(ims);

  float* exp_values_sigma_s = precompute_exp(width * height, sigma_s);
  float* exp_values_sigma_g = precompute_exp(255, sigma_g);
  
  int hs = compute_hs(sigma_s);
  int size = hs * 2 + 1;
  unsigned short* ims_values = pnm_to_us(ims);
  unsigned short* imd_values = malloc(height * width * sizeof(unsigned short));
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      int index = i * width + j;
      float sum1 = 0, sum2 = 0;
      for (int i2 = 0; i2 < size; ++i2) {
	int tmp_i = i + i2 - hs;
	if (tmp_i < 0 || tmp_i >= height) { continue; }
	for (int j2 = 0; j2 < size; ++j2) {
	  int tmp_j = j + j2 - hs;
	  if (tmp_j < 0 || tmp_j >= width) { continue; }
	  int index2 = tmp_i * width + tmp_j;
	  float tmp = exp_values_sigma_s[dist(i, j, tmp_i, tmp_j)] * exp_values_sigma_g[abs(ims_values[index] - ims_values[index2])];
	  sum1 += tmp * ims_values[index2];
	  sum2 += tmp;
	}
      }
      
      float value = sum1/sum2;
      if (value < 0) {
	value = 0;
      } else if (value > 255) {
	value = 255;
      } else {
	value = (unsigned short) value;
      }

      imd_values[index] = value;
    }
  }

  pnm imd = us_to_pnm(imd_values, width, height);
  pnm_save(imd, PnmRawPpm, imd_name);

  // free memory
  pnm_free(ims);
  pnm_free(imd);
  free(ims_values);
  free(imd_values);
  free(exp_values_sigma_s);
  free(exp_values_sigma_g);
}


void usage (char *s)
{
  fprintf(stderr, "Usage: %s <sigma_s> <sigma_g> <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 4
int main(int argc, char *argv[])
{
  if (argc != PARAM+1) {
    usage(argv[0]);
  }
  
  process(atoi(argv[1]), atoi(argv[2]), argv[3], argv[4]);

  return EXIT_SUCCESS;
}
