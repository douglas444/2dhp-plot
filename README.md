# 2dhp-plot

This program can be used to plot the structure of a protein in the 2D HP model.
It take as input the sequence of aminoacids, the direction of the edges between the aminoacids and the output file name.

Bellow we have the an example of a figure plotted for the sequence HHHHHHHHHHHHPHPHPPHHPPHHPPHPPHHPPHHPPHPPHHPPHHPPHPHPHHHHHHHHHHHH and directions RLLSLRRLLRSRRLRLRRLLRRLLRRLRRLLRRLLRRLRRLLRRLLRRLRLRRSRLSSSLLSS.

![alt text](https://github.com/douglas444/2dhp-plot/blob/main/example_figure.bmp?raw=true)

## Compilation

To compile, execute the following command from the root of the project:

```
gcc -Wall -O2  -c ./main.c -o ./main.o 
```

## How to run

After compiling it, you can run the aco-2dhp binary by executing the following command from the root of the project.
```
./2dhp-plot "HHHHHHHHHHHHPHPHPPHHPPHHPPHPPHHPPHHPPHPPHHPPHHPPHPHPHHHHHHHHHHHH" "RLLSLRRLLRSRRLRLRRLLRRLLRRLRRLLRRLLRRLRRLLRRLLRRLRLRRSRLSSSLLSS" ./example_figure.bmp
```
## The meaning of the letters in the sequence and in the directions

Sequence:

H -> Hydrophobic

P -> Polar

Directions:

R -> Right

L -> Left

S -> Straight
