#include <math.h>

#include "morphology.h"

//enum {SQUARE, DIAMOND, DISK, LINE_V, DIAG_R, LINE_H, DIAG_L, CROSS, PLUS};

pnm
se(int s, int hs){  
  (void) s;
  (void) hs;
  return NULL;
}

void 
lesser(unsigned short val, unsigned short* min){ 
  (void) val;
  (void) min;
}

void 
greater(unsigned short val, unsigned short* max){ 
  (void) val;
  (void) max;
}

void 
process(int s, 
	int hs, 
	pnm ims, 
	pnm imd, 
	void (*pf)(unsigned short, unsigned short*))
{  
  (void) s;
  (void) hs;
  (void) ims;
  (void) imd;
  (void) pf;
}
