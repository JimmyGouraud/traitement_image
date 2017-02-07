/**
 * @file filter.c
 * @author Jimmy Gouraud 
 * @brief zoom
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void filter(int factor, char* filter_name, char* ims_name, char* imd_name)
{/*
  pnm ims = pnm_load(ims_name);
  int rows = pnm_get_height(ims);
  int cols = pnm_get_width(ims);
  int imd_rows = rows * factor;
  int imd_cols = cols * factor;
  pnm imd = pnm_new(imd_cols, imd_rows, PnmRawPpm);

  int tmp_j;
  int left;
  for (int j = 0; j < imd_rows; ++j) {
    tmp_j = j / factor;
    left  = tmp_j - ;
    right = tmp_j + ; 
  }
  
  if (strcmp(filter_name, "box") == 0) {
    
  } else if (strcmp(filter_name, "tent") == 0) {
    printf("tent !");
  } else if (strcmp(filter_name, "bell") == 0) {
    printf("bell !");
  } else if (strcmp(filter_name, "mitch") == 0) {
    printf("mitch !");
  } else {
    fprintf(stderr, "Wrong <filter-name> : filter-name={box, tent, bell, mitch}.\n");
    exit(EXIT_FAILURE);
  }
*/
  (void) factor;
  (void) filter_name;
  (void) ims_name;
  (void) imd_name;
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

