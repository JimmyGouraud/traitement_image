#!/bin/bash

ims1='../data/barbara.ppm'
ims2='../data/barbara-impulse.ppm'
ims3='../data/barbara-gaussian-10.ppm'

# heat-equation <n> <ims> <imd>
./heat-equation 10 $ims1 heat-equation-10.ppm; pvisu heat-equation-10.ppm;
./heat-equation 50 $ims1 heat-equation-50.ppm; pvisu heat-equation-50.ppm;
./heat-equation 100 $ims1 heat-equation-100.ppm; pvisu heat-equation-100.ppm;

# anisotropic-diffusion <n> <lambda> <function> <ims> <imd>
./anisotropic-diffusion 100 10 0 $ims1 anisotropic-diffusion-100-10-0.ppm; pvisu anisotropic-diffusion-100-10-0.ppm;
./anisotropic-diffusion 100 10 1 $ims1 anisotropic-diffusion-100-10-1.ppm; pvisu anisotropic-diffusion-100-10-1.ppm;
./anisotropic-diffusion 100 10 2 $ims1 anisotropic-diffusion-100-10-2.ppm; pvisu anisotropic-diffusion-100-10-2.ppm;

# median <halfsize> <ims> <imd>
./median 1 $ims2 median-1.ppm; pvisu median-1.ppm;
./median 2 $ims2 median-2.ppm; pvisu median-2.ppm;
./median 3 $ims2 median-3.ppm; pvisu median-3.ppm;

# bilateral <sigma_s> <sigma_g> <ims> <imd>
./bilateral 3  30  $ims1 bilateral-3-30.ppm;   pvisu bilateral-3-30.ppm;
./bilateral 3  100 $ims1 bilateral-3-100.ppm;  pvisu bilateral-3-100.ppm;
./bilateral 10 30  $ims1 bilateral-10-30.ppm;  pvisu bilateral-10-30.ppm;
./bilateral 10 100 $ims1 bilateral-100-30.ppm; pvisu bilateral-10-100.ppm;

## nlmeans <sigma> <ims> <imd>
./nlmeans 5 $ims1 nlmeans-5.ppm; pvisu nlmeans-5.ppm;
./nlmeans 5 $ims1 nlmeans-15.ppm; pvisu nlmeans-15.ppm;
./nlmeans 5 $ims3 nlmeans-gaussian-10.ppm; pvisu nlmeans-gaussian-10.ppm;
