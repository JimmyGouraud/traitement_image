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

static int compare(const void* a, const void* b)
{
  return *((unsigned short*)a) - *((unsigned short*)b);
}

static unsigned short median(unsigned short *neighborhoods, int nb_heighbors)
{
  qsort(neighborhoods, nb_heighbors, sizeof(unsigned short), compare);
  return neighborhoods[nb_heighbors/2];
}

static void process(int hs, char* ims_name, char* imd_name)
{
  pnm ims = pnm_load(ims_name);
  int width = pnm_get_width(ims);
  int height = pnm_get_height(ims);

  unsigned short *values_ims = pnm_to_us(ims);
  unsigned short *values_imd = malloc(height * width * sizeof(unsigned short));
  
  int size = hs*2+1;
  unsigned short *neighborhoods = malloc(size * size * sizeof(unsigned short));
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      int nb_heighbors = 0;
      for (int i2 = 0; i2 < size; ++i2) {
	int tmp_i = i + i2 - hs;
	if (tmp_i < 0 || tmp_i >= height) { continue; }
	for (int j2 = 0; j2 < size; ++j2) {
	  int tmp_j = j + j2 - hs;
	  if (tmp_j < 0 || tmp_j >= width) { continue; }
	  neighborhoods[nb_heighbors] = values_ims[tmp_i * width + tmp_j];
	  nb_heighbors++;
	}
      }

      values_imd[i * width + j] = median(neighborhoods, nb_heighbors);
    }
  }

  pnm imd = us_to_pnm(values_imd, width, height);
  pnm_save(imd, PnmRawPpm, imd_name);
  
  pnm_free(ims);
  pnm_free(imd);
  free(values_ims);
  free(values_imd);
  free(neighborhoods);
}


void usage (char *s)
{
  fprintf(stderr, "Usage: %s <halfsize> <ims> <imd>\n", s);
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
