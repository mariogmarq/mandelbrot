# Mandelbrot set
Program to create a PBM image of the [Mandelbrot set](https://en.wikipedia.org/wiki/Mandelbrot_set) via multithreading(the number of threads is defined in main.cpp, feel free to change it).

To compile
```
g++ main.cpp -pthread -o exec
```
And to run
```
./exec height width
```
Please note that optimus height width ratio is 3:2


Use gimp to open the picture.