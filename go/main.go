package main

import (
	"image"
	"image/color"
	"image/png"
	"math"
	"os"
	"runtime"
	"strconv"
	"sync"
)

var output *image.Gray

func main() {
	//Parse arguments
	args := os.Args
	if len(args) < 2 {
		panic("Pass image size as a parameter")
	}

	size, err := strconv.Atoi(args[1])
	if err != nil {
		panic(args[1] + " not a number")
	}

	//Create image
	rect := image.Rect(0, size, size, 0)
	output = image.NewGray(rect)
	f, err := os.Create("image.png")
	if err != nil {
		panic("Cannot create image")
	}

	//Create coroutines
	wg := sync.WaitGroup{}
	cores := runtime.NumCPU()
	if cores < 16 {
		cores = 16 //fix for my computer
	}

	for i := 0; i < size; i += size / cores {
		wg.Add(1)
		go func(step int) {
			next := step + size/cores
			for j := step; j < next; j++ {
				for z := 0; z < size; z++ {
					output.SetGray(j, z, color.Gray{mandelbrot(findComplex(float64(j), float64(z), float64(size)))})
				}
			}

			wg.Done()
		}(i)
	}

	wg.Wait()

	err = png.Encode(f, output)
	if err != nil {
		f.Close()
		panic("Couldnt write")
	}

	err = f.Close()
	if err != nil {
		panic("error writing")
	}

}

func modulus(c complex128) float64 {
	return math.Sqrt(real(c)*real(c) + imag(c)*imag(c))
}

func findComplex(x, y, size float64) complex128 {
	coordinates := [2]float64{(x + 0.5) / size, (y + 0.5) / size}
	return complex((coordinates[0]-0.5)*2-1, (coordinates[1]-0.5)*2)
}

func mandelbrot(c complex128) uint8 {
	value := complex(0, 0)

	for i := 0.0; i <= 255; i += 0.05 {
		value = value*value + c
		mod := modulus(value)

		if mod > 4 {
			return uint8(i)
		}
	}
	return 255
}
