/**
 * @file test1.c
 * @author Vinh-Thong Ta <ta@labri.fr>
 * @brief ???
 */

#include <stdlib.h>
#include <stdio.h>

#include <bcl.h>

void  
process(char* ims_name, char* imd_name)
{
  pnm ims = pnm_load(ims_name);
  int rows = pnm_get_height(ims); 
  int cols = pnm_get_width(ims);
  
  pnm imd = pnm_new(cols, rows, PnmRawPpm);

  unsigned short* channel_r = (unsigned short *)malloc(rows * cols * sizeof(unsigned short));
  unsigned short* channel_g = (unsigned short *)malloc(rows * cols * sizeof(unsigned short));
  unsigned short* channel_b = (unsigned short *)malloc(rows * cols * sizeof(unsigned short));
  
  pnm_get_channel(ims, channel_r, 0);
  pnm_get_channel(ims, channel_g, 1);
  pnm_get_channel(ims, channel_b, 2);

  unsigned short* channel = (unsigned short *)malloc(rows * cols * sizeof(unsigned short));
  
  for (int i = 0; i < rows * cols; i++){
    channel[i] = (unsigned short)((channel_r[i] + channel_g[i] + channel_b[i]) / 3);
  }
  
  pnm_set_channel(imd, channel, 0);
  pnm_set_channel(imd, channel, 1);
  pnm_set_channel(imd, channel, 2);

  pnm_save(imd, PnmRawPpm, imd_name);
  
  pnm_free(ims);
  pnm_free(imd);

  free(channel);
  free(channel_r);
  free(channel_g);
  free(channel_b);
}

void 
usage (char *s){
  fprintf(stderr, "Usage: %s <ims> <imd>\n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 2
int 
main(int argc, char *argv[]){
  if (argc != PARAM+1) usage(argv[0]);
  
  char *ims = argv[1];
  char *imd = argv[2];
  
  process(ims, imd);
  
  return EXIT_SUCCESS;
}
