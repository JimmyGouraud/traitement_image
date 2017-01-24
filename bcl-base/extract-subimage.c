/**
 * @file test1.c
 * @author Vinh-Thong Ta <ta@labri.fr>
 * @brief ???
 */

#include <stdlib.h>
#include <stdio.h>

#include <bcl.h>

void  
process(int i0, int j0, int rows, int cols, char* ims_name, char* imd_name){
  pnm ims = pnm_load(ims_name);
  pnm imd = pnm_new(cols, rows, PnmRawPpm);

  unsigned short* pims = pnm_get_image(ims);
  unsigned short* pimd = pnm_get_image(imd);
  unsigned short* red;
  unsigned short* green;
  unsigned short* blue;
  for(int i = 0; i < pnm_get_height(ims); i++){
    for(int j = 0; j < pnm_get_width(ims); j++){
      if (i >= i0 && i < i0+rows && j >= j0 && j < j0+cols)
      {
	red = pims + pnm_offset(ims, i, j);
	green = red + 1;
	blue = green + 1;
	*pimd++ = *red;
	*pimd++ = *green;
	*pimd++ = *blue;
      }
    }    
  }
  
  pnm_save(imd, PnmRawPpm, imd_name);

  pnm_free(ims);
  pnm_free(imd);
}

void 
usage (char *s){
  fprintf(stderr, "Usage: %s <rows> <cols> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 6
int 
main(int argc, char *argv[]){
  if (argc != PARAM+1) usage(argv[0]);
  
  int i = atoi(argv[1]);
  int j = atoi(argv[2]);
  
  int rows = atoi(argv[3]);
  int cols = atoi(argv[4]);
  
  char *ims_name = argv[5];
  char *imd_name = argv[6];
  
  process(i, j, rows, cols, ims_name, imd_name);
  
  return EXIT_SUCCESS;
}
