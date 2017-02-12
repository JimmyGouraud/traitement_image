/**
 * @file filter.c
 * @author Jimmy Gouraud 
 * @brief zoom
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "fft.h"

double box(double x);
double tent(double x);
double bell(double x);
double mitchellNetravali(double x);

  
double WF(char* filter_name)
{
  if (strcmp(filter_name, "box") == 0) {
    return 0.5;
  }
  if (strcmp(filter_name, "tent") == 0) {
    return 1.0;
  }
  if (strcmp(filter_name, "bell") == 0) {
    return 1.5;
  }
  if (strcmp(filter_name, "mitch") == 0) {
    return 2.0;
  }
  
  fprintf(stderr, "Wrong <filter-name> : filter-name={box, tent, bell, mitch}.\n");
  exit(EXIT_FAILURE);
}

double h(char* filter_name, double x)
{
  if (strcmp(filter_name, "box") == 0) {
    return box(x);
  }
  if (strcmp(filter_name, "tent") == 0) {
    return tent(x);
  }
  if (strcmp(filter_name, "bell") == 0) {
    return bell(x);
  }
  if (strcmp(filter_name, "mitch") == 0) {
    return mitchellNetravali(x);
  }
  
  fprintf(stderr, "Wrong <filter-name> : filter-name={box, tent, bell, mitch}.\n");
  exit(EXIT_FAILURE);
}


double box(double x)
{
  if (-0.5 <= x && x <= 0.5) {
    return 1.0;
  }
  return 0.0;
}

double tent(double x)
{
  if (-1 <= x && x <= 1){
    return 1 - fabs(x);
  }

  return 0;
}

double bell(double x)
{
  double absX = fabs(x);
  if (absX <= 0.5){
    return -(x*x) + 0.75;
  }

  if (absX <= 1.5){
    return 0.5 * (absX - 1.5) * (absX - 1.5);
  }

  return 0;
}

double mitchellNetravali(double x)
{
  double absX = fabs(x);
  if (-2 <= x && x <= 2) {
    if (-1 <= x && x <= 1) {
      return (7/6.0) * absX * absX * absX - 2 * x * x + 8/9.0;
    }
    return (-7/18.0) * absX * absX * absX + 2 * x * x - (10/3.0) * absX + 16/9.0;
  }
  
  return 0;
}


void rotation (int rows, int cols, pnm ims, pnm imd) {  
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      for (int k = 0; k < 3; ++k) {
	pnm_set_component(imd, j, i, k, pnm_get_component(ims, i, j, k));
      }
    }
  }
}


void interpolation(char* filter_name, int factor, int imd_rows, int imd_cols, int cols, pnm ims, pnm imd)
{
  for (int j_prime = 0; j_prime < imd_cols; ++j_prime){
    for (int i = 0; i < imd_rows; ++i) {
      double j = j_prime / (double)factor;
      double wf = WF(filter_name);
      double left = j - wf;
      double right = j + wf;
      double S = 0;
      for (int k = left; k < right; ++k) {
	if (k < 0 || k >= cols){
	  continue;
	}
	S += pnm_get_component(ims, i, k, 0) * h(filter_name, k - j);
      }
      for (int k = 0; k < 3; ++k) {
	pnm_set_component(imd, i, j_prime, k, S);
      }
    }
  }
}


void filter(int factor, char* filter_name, char* ims_name, char* imd_name)
{
  pnm ims = pnm_load(ims_name);
  int rows = pnm_get_height(ims);
  int cols = pnm_get_width(ims);
  int imd_rows = rows * factor;
  int imd_cols = cols * factor;

  pnm imd_dim1 = pnm_new(imd_cols, rows, PnmRawPpm);
  interpolation(filter_name, factor, rows, imd_cols, cols, ims, imd_dim1);

  pnm imd_rotation = pnm_new(cols, imd_rows, PnmRawPpm);
  rotation(rows, imd_cols, imd_dim1, imd_rotation);

  pnm imd_dim2 = pnm_new(imd_cols, imd_rows, PnmRawPpm);
  interpolation(filter_name, factor, imd_rows, imd_cols, cols, imd_rotation, imd_dim2);

  pnm imd = pnm_new(imd_cols, imd_rows, PnmRawPpm);
  rotation(imd_rows, imd_cols, imd_dim2, imd);

  pnm_save(imd, PnmRawPpm, imd_name);

}


void usage (char *s){
  fprintf(stderr, "Usage: %s <factor> <filter-name> <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 4
int main (int argc, char *argv[]){
  if (argc != PARAM+1) usage(argv[0]);

  int factor = atoi(argv[1]);
  char* filter_name = argv[2];
  char* ims_name = argv[3];
  char* imd_name = argv[4];
  
  filter(factor, filter_name, ims_name, imd_name);
  
  return EXIT_SUCCESS;
}

