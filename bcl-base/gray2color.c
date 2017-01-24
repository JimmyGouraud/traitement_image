/**
 * @file test1.c
 * @author Vinh-Thong Ta <ta@labri.fr>
 * @brief ???
 */

#include <stdlib.h>
#include <stdio.h>

#include <bcl.h>

void  
process(char* ims0_name, char* ims1_name, char* ims2_name, char* imd_name)
{
  pnm ims0 = pnm_load(ims0_name);
  int rows = pnm_get_height(ims0); 
  int cols = pnm_get_width(ims0);
  
  pnm ims1 = pnm_load(ims1_name);
  pnm ims2 = pnm_load(ims2_name);
  pnm imd = pnm_new(cols, rows, PnmRawPpm);

  unsigned short* channel = (unsigned short *)malloc(rows * cols * sizeof(unsigned short));
  
  pnm_get_channel(ims0, channel, 0);
  pnm_set_channel(imd, channel, 0);
    
  pnm_get_channel(ims1, channel, 1);
  pnm_set_channel(imd, channel, 1);

  pnm_get_channel(ims2, channel, 2);
  pnm_set_channel(imd, channel, 2);

  pnm_save(imd, PnmRawPpm, imd_name);

  free(channel);
  pnm_free(ims0);
  pnm_free(ims1);
  pnm_free(ims2);
  pnm_free(imd);
}

void 
usage (char *s){
  fprintf(stderr, "Usage: %s <ims0> <ims1> <ims2> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 4
int 
main(int argc, char *argv[]){
  if (argc != PARAM+1) usage(argv[0]);
  
  char *ims0 = argv[1];
  char *ims1 = argv[2];
  char *ims2 = argv[3];
  char *imd  = argv[4];
  
  process(ims0, ims1, ims2, imd);
  
  return EXIT_SUCCESS;
}
