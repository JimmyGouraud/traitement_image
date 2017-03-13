/**
 * @file colorization
 * @brief Colorization of a grayscale images by transferring color 
 *        between a source, color image and a destination, greyscale image.
 */
#include <time.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <bcl.h>

#define D 3


static float RGB2LMS[D][D] = {
  {0.3811, 0.5783, 0.0402}, 
  {0.1967, 0.7244, 0.0782},  
  {0.0241, 0.1288, 0.8444}
};

static float LMS2RGB[D][D] = {
  { 4.4679, -3.5873,  0.1193},
  {-1.2186,  2.3809, -0.1624},
  { 0.0497, -0.2439,  1.2045}
};

static float LMS2LAB[D][D] = {
  { 0.5774,  0.5774,  0.5774},
  { 0.4082,  0.4082, -0.8165},
  { 0.7071, -0.7071,       0}
};

static float LAB2LMS[D][D] = {
  { 0.5774,  0.4082,  0.7071},
  { 0.5774,  0.4082, -0.7071},
  { 0.5774, -0.8165,       0}
};

static void product_matrix_vector(float matrix[D][D], float vector1[D], float vector2[D])
{
  for (int i = 0; i < D; ++i) {
    vector2[i] = 0;
    for (int j = 0; j < D; ++j) {
      vector2[i] += matrix[i][j] * vector1[j];
    }
  }
}

static void delete_skew_RGB_to_LMS(int size, float* values_lms)
{
  float value;
  for (int i = 0; i < size; ++i) {
    for (int k = 0; k < 3; ++k) {
      if (*values_lms <= 0) {
	value = 0;
      } else {
	value = log10(*values_lms);
      }
      *values_lms++ = value;
    }
  }
  values_lms -= size * 3;
}

static void delete_skew_LMS_to_RGB(int size, float* values_lms)
{
  float value;
  for (int i = 0; i < size; ++i) {
    for (int k = 0; k < 3; ++k) {
      value = pow(10, *values_lms);
      if (value > 255) {
	value = 255;
      } else if (value < 0) {
	value = 0;
      }
      *values_lms++ = value;
    }
  }
  values_lms -= size * 3;
}

static pnm float_to_pnm(int rows, int cols, float* values_ims)
{
  pnm ims = pnm_new(cols, rows, PnmRawPpm);
  unsigned short value;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      for (int k = 0; k < 3; ++k) {
	if (*values_ims < 0) {
	  value = 0;
	} else if (*values_ims > 255) {
	  value = 255;
	} else {
	  value = (unsigned short) round(*values_ims);
	}
	values_ims++;
	pnm_set_component(ims, i, j, k, value);
      }
    }
  }
  values_ims -= rows * cols * 3;

  return ims;
}

static float* pnm_to_float(pnm ims)
{
  int rows = pnm_get_height(ims);
  int cols = pnm_get_width(ims);
  
  float* values_ims = malloc(rows * cols * 3 * sizeof(float));
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      for (int k = 0; k < 3; ++k) {
	*values_ims++ = (float) pnm_get_component(ims, i, j, k);
      }
    }
  }
  values_ims -= rows * cols * 3;
  
  return values_ims;
}


static void A_to_B(int size, float* values_ims, float* values_imd, float matrix[D][D])
{
  float* vectorA = malloc(D * sizeof(float));
  float* vectorB = malloc(D * sizeof(float));
  
  for (int i = 0; i < size; ++i) {
    for (int k = 0; k < 3; ++k) {
      *vectorA++ = *values_ims++;
    }
    vectorA -= 3;
    product_matrix_vector(matrix, vectorA, vectorB);
    for (int k = 0; k < 3; ++k) {
      *values_imd++ = *vectorB++;
    }
    vectorB -= 3;
  }
  values_ims -= size * 3;
  values_imd -= size * 3;

  free(vectorA);
  free(vectorB);
}

static float* convertRGBtoLAB(pnm ims)
{
  int rows = pnm_get_height(ims);
  int cols = pnm_get_width(ims);
  int size = rows * cols;
  
  float* values_RGB = pnm_to_float(ims);
  
  float* values_LMS = malloc(size * 3 * sizeof(float));
  A_to_B(size, values_RGB, values_LMS, RGB2LMS);
  delete_skew_RGB_to_LMS(size, values_LMS);

  float* values_LAB = malloc(size * 3 * sizeof(float));
  A_to_B(size, values_LMS, values_LAB, LMS2LAB);

  free(values_RGB);
  free(values_LMS);
  
  return values_LAB;
}

static pnm convertLABtoRGB(int rows, int cols, float* values_LAB)
{
  int size = rows * cols;
  float* values_LMS = malloc(size * 3 * sizeof(float));
  A_to_B(size, values_LAB, values_LMS, LAB2LMS);
  delete_skew_LMS_to_RGB(size, values_LMS);

  float* values_RGB = malloc(size * 3 * sizeof(float));
  A_to_B(size, values_LMS, values_RGB, LMS2RGB);

  pnm imd = float_to_pnm(rows, cols, values_RGB);
  
  free(values_LMS);
  free(values_RGB);

  return imd;
}

static float* compute_mean(int size, float *values)
{
  float* values_mean = malloc(3 * sizeof(float));
  for (int i = 0; i < 3; ++i) {
    *values_mean++ = 0;
  }
  values_mean -= 3;
  
  for (int i = 0; i < size; ++i) {
    for (int k = 0; k < 3; ++k) {
      *values_mean++ += *values++;
    }
    values_mean -= 3;
  }
  values -= size * 3;
  
  for (int i = 0; i < 3; ++i) {
    *values_mean++ /= size;
  }
  values_mean -= 3;

  return values_mean;
}

// Standard Deviation
static float* compute_sd(int size, float* values)
{
  float* values_sd = malloc(3 * sizeof(float));
  for (int i = 0; i < 3; ++i) {
    *values_sd++ = 0;
  }
  values_sd -= 3;

  for (int i = 0; i < size; ++i) {
    for (int k = 0; k < 3; ++k) {
      *values_sd++ += *values * *values;
      values++;
    }
    values_sd -= 3;
  }
  values -= size * 3;

  float* values_mean = compute_mean(size, values);
  for (int i = 0; i < 3; ++i) {
    *values_sd = sqrt(*values_sd / size - (*values_mean * *values_mean));
    values_sd++;
    values_mean++;
  }
  values_mean -= 3;
  values_sd -= 3;

  free(values_mean);
  
  return values_sd;
}


static float* luminance_remapping(pnm ims, pnm imt)
{
  int size_ims = pnm_get_height(ims) * pnm_get_width(ims);
  
  int rows_imt = pnm_get_height(imt);
  int cols_imt = pnm_get_width(imt);
  int size_imt = rows_imt * cols_imt;

  float* values_LAB_ims = convertRGBtoLAB(ims);
  float* values_LAB_imt = convertRGBtoLAB(imt);

  float* values_mean_ims = compute_mean(size_ims, values_LAB_ims);
  float* values_mean_imt = compute_mean(size_imt, values_LAB_imt);
  float* sd_imt = compute_sd(size_imt, values_LAB_imt);  
  float* sd_ims = compute_sd(size_ims, values_LAB_ims);
  
  float* values_luminance_remapping = malloc(size_imt * sizeof(float));
  for (int i = 0; i < size_imt; ++i) {
    *values_luminance_remapping++ = *sd_ims * (*values_LAB_imt - *values_mean_imt) / *sd_imt + *values_mean_ims;
    values_LAB_imt += 3;
  }
  values_LAB_imt -= size_imt * 3;
  values_luminance_remapping -= size_imt;
  
  free(values_LAB_ims);
  free(values_LAB_imt);
  free(values_mean_imt);
  free(values_mean_ims);
  free(sd_imt);  
  free(sd_ims);
  
  return values_luminance_remapping;
}

static float* compute_jittered_sampling(int rows, int cols, float* values_ims, int* nbSamples, int neighborhoodSize) {
  srand(time(NULL));
  
  int size = rows * cols;
  int nbPixelsBySquare = size / *nbSamples;
  int nbPixelsBySide = sqrt(nbPixelsBySquare);
  int nbSquaresByRows = rows / nbPixelsBySide;
  int nbSquaresByCols = cols / nbPixelsBySide;
  *nbSamples = nbSquaresByRows * nbSquaresByCols;
  
  float* jittered_sampling = malloc(3 * *nbSamples * sizeof(float));
  for (int i = 0; i < rows; i += nbPixelsBySide) {
    if (i >= nbSquaresByRows * nbPixelsBySide) { continue; }
    for (int j = 0; j < cols; j += nbPixelsBySide) {
      if (j >= nbSquaresByCols * nbPixelsBySide) { continue; }
      int random_i = rand()%nbPixelsBySide + i;
      int random_j = rand()%nbPixelsBySide + j;
      
      int cpt = 0;
      double mean = 0;
      double sd = 0;
      for (int i2 = random_i - neighborhoodSize; i2 <= random_i + neighborhoodSize; ++i2) {
	if (i2 < 0 || i2 >= rows) { continue; }
	for (int j2 = random_j - neighborhoodSize; j2 <= random_j + neighborhoodSize; ++j2) {
	  if (j2 < 0 || j2 >= cols) { continue; }
	  int offset = (i2 * cols + j2) * 3;
	  values_ims += offset;
	  mean += *values_ims;
	  sd += *values_ims * *values_ims;
	  values_ims -= offset;
	  cpt++;
	} 
      }
      mean /= cpt;
      int offset = (random_i * cols + random_j) * 3;
      values_ims += offset;
      *jittered_sampling++ = sqrt(sd / cpt - (mean * mean));
      *jittered_sampling++ = *values_ims++;
      *jittered_sampling++ = *values_ims++;
      values_ims -= offset + 3;
    }
  }
  jittered_sampling -= *nbSamples * 3;

  return jittered_sampling;
}


static void process(char *name_ims, char *name_imt, char* name_imd)
{
  pnm ims = pnm_load(name_ims);
  int rows_ims = pnm_get_height(ims);
  int cols_ims = pnm_get_width(ims);
  
  pnm imt = pnm_load(name_imt);
  int rows_imt = pnm_get_height(imt);
  int cols_imt = pnm_get_width(imt);
  
  // Step 1 - RGB to LAB
  float* values_ims = convertRGBtoLAB(ims);
  float* values_imt = convertRGBtoLAB(imt);
  printf("step 1 - done!\n");
  
  // Step 2 - luminance remapping
  float* lum_remapping = luminance_remapping(ims, imt);
  printf("step 2 - done!\n");
  
  // Step 3 - random jittered grid
  int nbSamples = 600;
  float* jittered_sampling = compute_jittered_sampling(rows_ims, cols_ims, values_ims, &nbSamples, 2);
  printf("step 3 - done!\n");

  // Step 4 - Colorization
  for (int i = 0; i < rows_imt; ++i) {
    for (int j = 0; j < cols_imt; ++j) {
      float best_luminance = 500;
      float best_alpha;
      float best_beta;
      for (int k = 0; k < nbSamples; ++k) {
	float value_luminance = 0.5 * (*lum_remapping + *jittered_sampling);
	if(abs(value_luminance - *values_imt) < abs(best_luminance - *values_imt)) {
	  best_luminance = value_luminance;
	  jittered_sampling++;
	  best_alpha = *jittered_sampling++;
	  best_beta = *jittered_sampling++;
	} else {
	  jittered_sampling += 3;
	}
      }
      jittered_sampling -= nbSamples * 3;
      *values_imt++ = best_luminance;
      *values_imt++ = best_alpha;
      *values_imt++ = best_beta;
      printf("i = %d, j = %d, *lum_remapping = %f\n", i, j, *lum_remapping);
      lum_remapping++;
    }
  }
  lum_remapping -= rows_imt * cols_imt;
  values_imt -= rows_imt * cols_imt * 3;
  
  printf("step 4 - done!\n");

  // Step 5 - LAB to RGB
  pnm imd = convertLABtoRGB(rows_imt, cols_imt, values_imt);
  pnm_save(imd, PnmRawPpm, name_imd);

  printf("step 5 - done!\n");

  pnm_free(ims);
  pnm_free(imt);
  pnm_free(imd);
}

void usage (char *s){
  fprintf(stderr, "Usage: %s <ims> <imt> <imd> \n", s);
  exit(EXIT_FAILURE);
}

#define param 3
int main(int argc, char *argv[]){
  if (argc != param+1) 
    usage(argv[0]);
  process(argv[1], argv[2], argv[3]);
  return EXIT_SUCCESS;
}