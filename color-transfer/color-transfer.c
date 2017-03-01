/**
 * @file color-transfert
 * @brief transfert color from source image to target image.
 *        Method from Reinhard et al. : 
 *        Erik Reinhard, Michael Ashikhmin, Bruce Gooch and Peter Shirley, 
 *        'Color Transfer between Images', IEEE CGA special issue on 
 *        Applied Perception, Vol 21, No 5, pp 34-41, September - October 2001
 */

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <bcl.h>

#define D 3

static float ID[D][D] = {
  {1, 0, 0}, 
  {0, 1, 0},  
  {0, 0, 1}
};


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
  { 0.5773,       0,       0},
  {      0,  0.4082,       0},
  {      0,       0,       0}
};

static float LAB2LMS[D][D] = {
  { 0.5773,       0,       0},
  {      0,  0.4082,       0},
  {      0,       0,       0}
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


static void delete_skew_RGB_to_LMS(int rows, int cols, float* values_lms)
{
  float value;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      for (int k = 0; k < 3; ++k) {
	// Gérer le cas où (values_lms == 0)
	value = log10(*values_lms);
	*values_lms++ = value;
      }
    }
  }
  values_lms -= rows * cols * 3;
  
  (void) rows;
  (void) cols;
  (void) values_lms;
}

static void delete_skew_LMS_to_RGB(int rows, int cols, float* values_lms)
{
  float value;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      for (int k = 0; k < 3; ++k) {
	value = pow(10, *values_lms);
	*values_lms++ = value;
      }
    }
  }
  values_lms -= rows * cols * 3;
  
  (void) rows;
  (void) cols;
  (void) values_lms;
}

static pnm float_to_pnm(int rows, int cols, float* values_ims)
{
  pnm ims = pnm_new(cols, rows, PnmRawPpm);
  unsigned short value;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      for (int k = 0; k < 3; ++k) {
	value = (unsigned short) *values_ims++;
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

  float* values_ims = malloc(rows * cols * D* sizeof(float));
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      for (int k = 0; k < 3; ++k) {
	*values_ims++ = pnm_get_component(ims, i, j, k);
      }
    }
  }
  values_ims -= rows * cols * D;
  
  return values_ims;
}


static void A_to_B(int rows, int cols, float* ims, float* imd, float matrix[D][D])
{
  float* vectorA = malloc(D * sizeof(float));
  float* vectorB = malloc(D * sizeof(float));
  
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      for (int k = 0; k < 3; ++k) {
	*vectorA++ = *ims++;
      }
      vectorA -= 3;
      product_matrix_vector(matrix, vectorA, vectorB);
      for (int k = 0; k < 3; ++k) {
	*imd++ = *vectorB++;
      }
      vectorB -= 3;
    }
  }
  
  ims -= rows * cols * D;
  imd -= rows * cols * D;

  free(vectorA);
  free(vectorB);
}


static void process(char *name_ims, char *name_imt, char* name_imd)
{
  pnm ims = pnm_load(name_ims);
  int rows = pnm_get_height(ims);
  int cols = pnm_get_width(ims);
  float* values_ims = pnm_to_float(ims);
  
  float* values_lms = malloc(rows * cols * D * sizeof(float));
  A_to_B(rows, cols, values_ims, values_lms, RGB2LMS);
  pnm_save(float_to_pnm(rows, cols, values_lms), PnmRawPpm, "RGB2LMS.ppm");
  delete_skew_RGB_to_LMS(rows, cols, values_lms);
  pnm_save(float_to_pnm(rows, cols, values_lms), PnmRawPpm, "RGB2LMSwithoutSkew.ppm");
  
  
  float* values_lab = malloc(rows * cols * D * sizeof(float));
  A_to_B(rows, cols, values_lms, values_lab, LMS2LAB);
  pnm_save(float_to_pnm(rows, cols, values_lab), PnmRawPpm, "LMS2LAB.ppm");

  A_to_B(rows, cols, values_lab, values_lms, LAB2LMS);
  pnm_save(float_to_pnm(rows, cols, values_lms), PnmRawPpm, "LAB2LMS.ppm");
  
  delete_skew_LMS_to_RGB(rows, cols, values_lms);
  pnm_save(float_to_pnm(rows, cols, values_lms), PnmRawPpm, "LAB2LMSwithoutSkew.ppm");

  float* values_imd = malloc(rows * cols * D * sizeof(float));
  A_to_B(rows, cols, values_lms, values_imd, LMS2RGB);
  pnm_save(float_to_pnm(rows, cols, values_imd), PnmRawPpm, "LMS2RGB.ppm");

  pnm imd = float_to_pnm(rows, cols, values_imd);
  pnm_save(imd, PnmRawPpm, name_imd);


  (void) ID;
  (void) LMS2RGB;
  (void) RGB2LMS;
  (void) values_ims;
  (void) name_ims;
  (void) name_imt;
  (void) name_imd;
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
