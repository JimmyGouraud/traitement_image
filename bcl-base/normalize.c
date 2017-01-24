/**
 * @file test1.c
 * @author Vinh-Thong Ta <ta@labri.fr>
 * @brief ???
 */

#include <stdlib.h>
#include <stdio.h>

#include <bcl.h>

void  
process(int min, int max, char* ims_name, char* imd_name)
{
  pnm ims = pnm_load(ims_name);
  int rows = pnm_get_height(ims); 
  int cols = pnm_get_width(ims);
  pnm imd = pnm_new(cols, rows, PnmRawPpm);

  unsigned short MIN = pnm_get_component(ims, 0, 0, 0);
  unsigned short MAX = MIN;
  unsigned short value_ims;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      for(int k = 0; k < 3; k++) {
	value_ims = pnm_get_component(ims, i, j, k);
	if (value_ims < MIN) {
	  MIN = value_ims;
	}
	if (value_ims > MAX) {
	  MAX = value_ims;
	}
      }
    }
  }
  unsigned short value_imd;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      for (int k = 0; k < 3; k++){
	value_ims = pnm_get_component(ims, i, j, k);
	value_imd = (max - min) / (float)(MAX - MIN) * value_ims + (min * MAX - max * MIN) / (float)(MAX - MIN);
	pnm_set_component(imd, i, j, k, value_imd); 
      }
    }
  }

  pnm_save(imd, PnmRawPpm, imd_name);
  
  pnm_free(imd);
  pnm_free(ims);
}

void 
usage (char *s){
  fprintf(stderr, "Usage: %s <min> <max> <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 4
int 
main(int argc, char *argv[]){
  if (argc != PARAM+1) usage(argv[0]);
  
  int min = atoi(argv[1]);
  int max = atoi(argv[2]);
  char *ims_name = argv[3];
  char *imd_name = argv[4];
  
  process(min, max, ims_name, imd_name);
  
  return EXIT_SUCCESS;
}
