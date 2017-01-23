/**
 * @file test1.c
 * @author Vinh-Thong Ta <ta@labri.fr>
 * @brief ???
 */

#include <stdlib.h>
#include <stdio.h>

#include <bcl.h>

void  
process(int num, char* ims_name, char* imd_name){
  pnm ims = pnm_load(ims_name);

  
  pnm imd = pnm_new(cols, rows, PnmRawPpm);

  unsigned short* pims = pnm_get_image(ims);
  unsigned short* pimd = pnm_get_image(imd);

  for(int i = 0; i < pnm_get_height(ims); i++){
    for(int j = 0; j < pnm_get_width(ims); j++){
      for(int k = 0; k < 3; i++){
	
      }
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
  
  int num = atoi(argv[1]);
  char *ims_name = argv[2];
  char *imd_name = argv[3];
  
  process(num, ims_name, imd_name);
  
  return EXIT_SUCCESS;
}
