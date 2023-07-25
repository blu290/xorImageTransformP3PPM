# xorImageTransformP3PPM
simple image manipulation program written in C

xor's each pixel with a colour mask defined by the user, with a user defined probability. works only on p3 ppm files.

example usage: xorImageTransformP3PPM inputfile.ppm "255 255 255" outputfile.ppm 0.5

(applies the mask 255,255,255 to a pixel randomly selected with a given pixel having a 50% chance of being picked)
