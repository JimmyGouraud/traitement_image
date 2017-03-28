/**
 * @file  labeling.c
 * @brief count connected components in pnm binary image
*/


#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <pnm.h>
#include <memory.h>
 
int _find(int p, int* roots)
{
  while (roots[p] != p) {
    p = roots[p];
  }
  return p;
}

int _union(int r0, int r1, int* roots)
{
  if (r0 == r1) { return r0; }
  if (r0 == -1) { return r1; }
  if (r1 == -1) { return r0; }
  if (r0 < r1) {
    roots[r1] = r0;
    return r0;
  } else {
    roots[r0] = r1;
    return r1;  
  }
}

int _add(int p, int r, int* roots)
{
  if(r == -1) {
    roots[p] = p;
  } else { 
    roots[p] = r;
  }
  return roots[p];
}
 
void process(char* ims_name, char* imd_name)
{
  pnm ims = pnm_load(ims_name);
  int width = pnm_get_width(ims);
  int height = pnm_get_height(ims);  

  unsigned short *ps = pnm_get_channel(ims, NULL, PnmRed);;
  int *roots = memory_alloc(width * height * sizeof(int));
  
  int p = 0;
  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      int r = -1;
      if ( (j > 0) && (*(ps-1) == (*ps)) ) {
	r = _union(_find(p-1, roots), r, roots);
      }

      if ( (i > 0 && j > 0) && (*(ps-1-width) == (*ps)) ) { 
	r = _union(_find(p-1-width, roots), r, roots);
      }

      if (i > 0 && (*(ps-width) == (*ps)) ) {
	r = _union(_find(p-width, roots), r, roots);
      }
      
      if( (j < (width-1) && i > 0) && (*(ps+1-width) == (*ps)) ) {
	r = _union(_find(p+1-width, roots), r, roots);
      }
      
      r = _add(p, r, roots);
      p++; 
      ps++;
    }
  }
  ps -= width * height;

  for(int p = 0; p < width*height; p++) { 
    roots[p] = _find(p, roots);
  }
  
  int l = 0;
  for(int p = 0; p < width*height; p++) {
    if(roots[p] == p) {
      roots[p] = l++;
    } else {
      roots[p] = roots[roots[p]];
    }
  }

  // Add color to components
  int *roots_color = malloc(3 * l * sizeof(int));
  for (int i = 0; i < l; ++i) {
    for (int k = 0; k < 3; k++) {
      roots_color[i * 3 + k] = rand()%256;
    }
  }
  
  pnm imd = pnm_new(width, height, PnmRawPpm);
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      if (pnm_get_component(ims, i, j, 0) == 0 &&
	  pnm_get_component(ims, i, j, 1) == 0 &&
	  pnm_get_component(ims, i, j, 2) == 0) {
	for (int k = 0; k < 3; ++k) {
	  pnm_set_component(imd, i, j, k, 0);
	}
      } else {
	for (int k = 0; k < 3; ++k) {
	  unsigned short value = roots_color[roots[i*width + j] * 3 + k];
	  pnm_set_component(imd, i, j, k, value);
	}
      }
    }
  }
  
  fprintf(stderr, "labeling: %d components found\n", l);
  memory_free(roots);
  
  pnm_save(imd, PnmRawPpm, imd_name);  

  free(ps);
  free(roots_color);
  pnm_free(ims);
  pnm_free(imd);
}

void usage(char* s)
{
  fprintf(stderr,"%s <ims> <imd>\n",s);
  exit(EXIT_FAILURE);
}

#define PARAM 2
int main(int argc, char* argv[])
{
  if(argc != PARAM+1) 
    usage(argv[0]);
  
  process(argv[1], argv[2]);

  return EXIT_SUCCESS;
}
