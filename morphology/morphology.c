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
      float dist = sqrt((i-middle) * (i-middle) + (j-middle) * (j - middle));
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
      int dist = abs(i - middle) + abs(j - middle);
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

  for (int j = 0; j < size; ++j) {
    for (int k = 0; k < 3; ++k) {
      pnm_set_component(imd, size - j - 1, j, k, 255);
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
  int size = hs * 2 + 1;

  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      for (int k = 0; k < 3; ++k) {
        unsigned short value = pnm_get_component(ims, i, j, k);
        for (int i2 = 0; i2 < size; ++i2) {
          int tmp_i = i + i2 - hs;
          if (tmp_i < 0 || tmp_i >= height) { continue; }
          for (int j2 = 0; j2 < size; ++j2) {
            int tmp_j = j + j2 - hs;
            if (tmp_j < 0 || tmp_j >= width) { continue; }
            if (pnm_get_component(elem, i2, j2, k) == 0) { continue; }
            (*pf)(pnm_get_component(ims, tmp_i, tmp_j, k), &value);
          }
        }
        pnm_set_component(imd, i, j, k, value);
      }
    }
  }
}
