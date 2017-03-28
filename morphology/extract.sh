#!/bin/bash

pvisu $1

# Etape 1 : Ouverture
./opening 2 10 $1 opening.ppm
pvisu opening.ppm

#Etape 2 : soustraction
./subtract $1 opening.ppm extract.ppm
pvisu extract.ppm
