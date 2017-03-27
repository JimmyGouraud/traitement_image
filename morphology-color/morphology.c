#include <math.h>

#include "morphology.h"

enum {SQUARE, DIAMOND, DISK, LINE_V, DIAG_R, LINE_H, DIAG_L, CROSS, PLUS};

void create_square(pnm imd) {
  int size = pnm_get_width(imd);

  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      for (int k = 0; k < 3; ++k) {
        pnm_set_component(imd, i, j, k, 255);
      }
    }
  }
}

void create_disk(pnm imd) {
  int size = pnm_get_width(imd);
  int middle = size/2;

  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      float dist = sqrt((i-middle) * (i-middle) + (j-middle) * (j-middle));
      if (dist <= middle) {
        for (int k = 0; k < 3; ++k) {
          pnm_set_component(imd, i, j, k, 255);
        }
      }
    }
  }
}

void create_diamond(pnm imd) {
  int size = pnm_get_width(imd);
  int middle = size/2;

  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      int dist = abs(i-middle) + abs(j-middle);
      if (dist <= middle) {
        for (int k = 0; k < 3; ++k) {
          pnm_set_component(imd, i, j, k, 255);
        }
      }
    }
  }
}

void create_line_v(pnm imd) {
  int size = pnm_get_width(imd);
  int middle = size/2;

  for (int i = 0; i < size; ++i) {
    for (int k = 0; k < 3; ++k) {
      pnm_set_component(imd, i, middle, k, 255);
    }
  }
}

void create_line_h(pnm imd) {
  int size = pnm_get_width(imd);
  int middle = size/2;

  for (int j = 0; j < size; ++j) {
    for (int k = 0; k < 3; ++k) {
      pnm_set_component(imd, middle, j, k, 255);
    }
  }
}


void create_diag_l(pnm imd) {
  int size = pnm_get_width(imd);

  for (int i = 0; i < size; ++i) {
    for (int k = 0; k < 3; ++k) {
      pnm_set_component(imd, i, i, k, 255);
    }
  }
}


void create_diag_r(pnm imd) {
  int size = pnm_get_width(imd);

  for (int i = 0; i < size; ++i) {
    for (int k = 0; k < 3; ++k) {
      pnm_set_component(imd, size - i - 1, i, k, 255);
    }
  }
}

void create_cross(pnm imd)
{
  create_diag_l(imd);
  create_diag_r(imd);
}

void create_plus(pnm imd)
{
  create_line_v(imd);
  create_line_h(imd);
}


pnm se(int s, int hs) {
  int size = hs * 2 + 1;
  pnm imd = pnm_new(size, size, PnmRawPpm);

  switch(s) {
  case SQUARE:
    create_square(imd);
    break;
  case DIAMOND:
    create_diamond(imd);
    break;
  case DISK:
    create_disk(imd);
    break;
  case LINE_V:
    create_line_v(imd);
    break;
  case DIAG_R:
    create_diag_r(imd);
    break;
  case LINE_H:
    create_line_h(imd);
    break;
  case DIAG_L:
    create_diag_l(imd);
    break;
  case CROSS:
    create_cross(imd);
    break;
  case PLUS:
    create_plus(imd);
    break;
  }

  return imd;
}

void lesser(unsigned short val, unsigned short* min) {
  if (*min > val) {
    *min = val;
  }
}

void greater(unsigned short val, unsigned short* max) {
  if (*max < val) {
    *max = val;
  }
}

void process(int s, int hs, pnm ims, pnm imd, void (*pf)(unsigned short, unsigned short*))
{
  int width = pnm_get_width(ims);
  int height = pnm_get_height(ims);

  pnm elem = se(s, hs);
  int size = pnm_get_width(elem);

  unsigned short* imd_value = pnm_get_image(imd);
  unsigned short* ims_value = pnm_get_image(ims);
  unsigned short* elem_value = pnm_get_image(elem);

  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      int indice = (i * width + j) * 3;
      unsigned short r = ims_value[indice];
      unsigned short g = ims_value[indice + 1];
      unsigned short b = ims_value[indice + 2];

      for (int i2 = 0; i2 < size; ++i2) {
	int tmp_i = i + i2 - hs;
	if (tmp_i < 0 || tmp_i >= height) { continue; }
	for (int j2 = 0; j2 < size; ++j2) {
	  int tmp_j = j + j2 - hs;
	  if (tmp_j < 0 || tmp_j >= width) { continue; }
	  if (elem_value[(i2 * size + j2) * 3] == 0) { continue; }

	  int indice2 = (tmp_i * width + tmp_j) * 3;
	  unsigned short r2 = ims_value[indice2];
	  unsigned short g2 = ims_value[indice2 + 1];
	  unsigned short b2 = ims_value[indice2 + 2];

	  if (r == r2) {
	    if (g == g2) {
	      unsigned short value = b;
	      (*pf)(b2, &value);
	      if (value != b) {
		b = b2;
	      }
	    } else {
	      unsigned short value = g;
	      (*pf)(g2, &value);
	      if (value != g) {
		g = g2;
		b = b2;
	      }
	    }
	  } else {
	    unsigned short value = r;
	    (*pf)(r2, &value);
	    if (value != r) {
	      r = r2;
	      g = g2;
	      b = b2;
	    }
	  }
	}
      }
    
      imd_value[indice]   = r;
      imd_value[indice+1] = g;
      imd_value[indice+2] = b;
    }
  }
  
  pnm_free(elem);
}
