#!/bin/bash

ims='../data/lena-gray.ppm'

# make-se <shape> <halfsize> <imd>
./make-se 0 20 0.ppm; pvisu 0.ppm;
./make-se 1 20 1.ppm; pvisu 1.ppm;
./make-se 2 20 2.ppm; pvisu 2.ppm;
./make-se 3 20 3.ppm; pvisu 3.ppm;
./make-se 4 20 4.ppm; pvisu 4.ppm;
./make-se 5 20 5.ppm; pvisu 5.ppm;
./make-se 6 20 6.ppm; pvisu 6.ppm;
./make-se 7 20 7.ppm; pvisu 7.ppm;
./make-se 8 20 8.ppm; pvisu 8.ppm;


# dilation / erosion <shape> <halfsize> <ims> <imd>
./dilation 2 10 $ims dilation_circle.ppm; pvisu dilation_circle.ppm;
./dilation 7 10 $ims dilation_cross.ppm; pvisu dilation_cross.ppm;
./erosion 2 10 $ims erosion_circle.ppm; pvisu erosion_circle.ppm;
./erosion 7 10 $ims erosion_cross.ppm; pvisu erosion_cross.ppm;


# opening / closing <shape> <halfsize> <ims> <imd>
./opening 2 10 $ims opening_square.ppm; pvisu opening_square.ppm;
./opening 7 10 $ims opening_cross.ppm; pvisu opening_cross.ppm;
./closing 2 10 $ims closing_square.ppm; pvisu closing_square.ppm;
./closing 7 10 $ims closing_cross.ppm; pvisu closing_cross.ppm;

# add / subtract <ims-1> <ims-2> <imd>
## gradient morphologique
./dilation 2 2 $ims a.ppm; pvisu a.ppm;
./erosion 2 2 $ims b.ppm; pvisu b.ppm;
./subtract a.ppm b.ppm c.ppm; pvisu c.ppm;

## Laplacien morphologique
./add a.ppm b.ppm d.ppm;
./subtract d.ppm $ims e.ppm; 
./subtract e.ppm $ims f.ppm; pvisu f.ppm;

# extract.sh
./extract.sh

# binarize
./binarize 125 255 extract.ppm binarize_extract.ppm; 

# ./labeling-color <ims> <imd>
./labeling-color binarize_extract.ppm extract-color.ppm; pvisu extract-color.ppm
