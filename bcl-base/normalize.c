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

  // TO DO

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
