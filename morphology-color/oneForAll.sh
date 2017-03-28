#!/bin/bash

ims='../data/mm-color.ppm'

# Extension à la couleur
./dilation 2 3 $ims dilation_marg.ppm; pvisu dilation_marg.ppm 
./erosion 2 3 $ims erosion_marg.ppm; pvisu erosion_marg.ppm
