/**
 * @file print-ppm.c
 * @brief print to standard output a color ppm file
 */

#include <stdlib.h>
#include <stdio.h>

static void
process(int r, int g, int b, int rows, int cols){
  printf("P3\n%d %d\n255\n", cols, rows);
  for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < cols; j++)
	{
	  printf("%d %d %d ", r, g, b);
	}
      printf("\n");
    }
}

void 
usage (char *s)
{
  fprintf(stderr,
	  "Usage: %s <r={0,255}> <g={0,255}> <b={0,255}> <rows> <cols>\n"
	  ,s);
  exit(EXIT_FAILURE);
}

#define PARAMS 5
int 
main(int argc, char *argv[])
{
  if (argc != PARAMS+1){
    usage(argv[0]);
  }
  process(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
  return EXIT_SUCCESS;
}


