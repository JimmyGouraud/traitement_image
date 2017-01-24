/**
 * @file test1.c
 * @author Vinh-Thong Ta <ta@labri.fr>
 * @brief ???
 */

#include <stdlib.h>
#include <stdio.h>

#include <bcl.h>

void  
process(int num, char* ims_name, char* imd_name)
{
  pnm ims = pnm_load(ims_name);
  int rows = pnm_get_height(ims); 
  int cols = pnm_get_width(ims);
  pnm imd = pnm_new(cols, rows, PnmRawPpm);

  unsigned short* channel = (unsigned short *)malloc(rows * cols * sizeof(unsigned short));
  pnm_get_channel(ims, channel, num);
  pnm_set_channel(imd, channel, 0);
  pnm_set_channel(imd, channel, 1);
  pnm_set_channel(imd, channel, 2);

  pnm_save(imd, PnmRawPpm, imd_name);

  free(channel);
  pnm_free(ims);
  pnm_free(imd);
}

void 
usage (char *s){
  fprintf(stderr, "Usage: %s <num> <ims> <imd>\n", s);
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
