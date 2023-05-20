#!/bin/sh
clear
libraries="-lm -lSDL2 -lSDL2_ttf -lSDL2_gfx -lSDL2_image"
flags="-std=c11 -Wall -Wextra -Wpedantic -Werror -g"
gcc main.c $flags $libraries -o main.out
