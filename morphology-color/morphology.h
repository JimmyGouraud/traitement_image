#ifndef __MORPHOLOGY_HH__
#define __MORPHOLOGY_HH__

/**
 *  @file  morphology.h
 *  @brief header for morphology.c that implements functions 
 *         for mathematical morphology
 *         image processing
 *	   A structuring element is viewed as a pnm binary image file : 0 for
 *	   the background and 255 for the shape.
 */
#include <stdlib.h>
#include <stdio.h>
#include <pnm.h>

/**
 * @brief  generate a structuring element of a given shape and halfsize
 * @param  shape: the structing element shape umber
 *         0: square
 *         1: diamond
 *	   2: disk
 * @param  halfsize: the structuring element halfsize
 * @return the structuring element of (2*halfsize+1)^2 size as an pnm object
 */
pnm 
se(int shape, int halfsize); 

/**
 * @brief  compute a morphological dilation or erosion on a grayscale image
 *         with a given structuring element. Dilation or erosion 
 *         processing depends on an order function  defined by the pointer pf
 * @param  shape: the structing element shape umber
 * @param  halfsize: the structuring element halfsize
 * @param  ims: the input image source to process
 * @param  imd: the destination image
 * @param  pf: a pointer on a ordering function
 */
void
process(int shape, 
	int halfsize, 
	pnm ims, 
	pnm imd, 
	void (*pf)(unsigned short, unsigned short*));

/**
 * @brief  ordering function, if val is geater than max then update max
 * @param  val: the input value
 * @param  max: the value to update
 */
void
greater(unsigned short val, unsigned short* max);

/**
 * @brief  ordering function, if val is lesser than min then update min
 * @param  val: the input value
 * @param  min: the value to update
 */
void
lesser(unsigned short val, unsigned short* min);

#endif
