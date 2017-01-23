/**
 * @file test1.c
 * @author Vinh-Thong Ta <ta@labri.fr>
 * @brief ???
 */

#include <stdlib.h>
#include <stdio.h>

#include <bcl.h>

void  
process(int rows, int cols, char* imd_name){
  
  pnm imd = pnm_new(cols, rows, PnmRawPpm);

  unsigned short *p = pnm_get_image(imd);
  for(int j=0; j<cols; j++){
    for(int i=0; i<rows; i++){
      for(int k=0; k<3; k++){
	*p++ = pnm_maxval;
      }
    }    
  }
  int j0 = cols/4;
  int j1 = 3*j0;
  int i0 = rows/4;
  int i1 = 3*i0;
  unsigned short* image = pnm_get_image(imd);
  for(int j=j0; j<j1; j++){
    for(int i=i0; i<i1; i++){
      p  = image + pnm_offset(imd, i, j);
      *p = pnm_maxval - *p; 
      p++;
    }
  }
  pnm_save(imd, PnmRawPpm, imd_name);
  pnm_free(imd);
}

void 
usage (char *s){
  fprintf(stderr, "Usage: %s <rows> <cols> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 3
int 
main(int argc, char *argv[]){
  if (argc != PARAM+1) usage(argv[0]);  
  int rows = atoi(argv[1]);
  int cols = atoi(argv[2]);
  char *imd_name = argv[3];
  process(rows, cols, imd_name);
  return EXIT_SUCCESS;
}
