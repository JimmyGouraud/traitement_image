#!/bin/bash

ims='../data/gear.ppm'

pvisu $ims

# Etape 1 : Ouverture
./opening 2 10 $ims opening.ppm
pvisu opening.ppm

#Etape 2 : soustraction
./subtract $ims opening.ppm extract.ppm
pvisu extract.ppm
