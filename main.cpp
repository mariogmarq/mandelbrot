#include "complex.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>
#include <pthread.h>
#define THREADS 12

std::vector<std::vector<bool>> BooleanMatrix;
std::vector<std::vector<Complex>> ComplexMatrix;

Complex FunctionMandelbrot(Complex c, Complex z) {
    return (z*z) + c;
}


Complex RecursiveMandelbrot(int iterations, Complex c) {
    //First iteration
    auto z = FunctionMandelbrot(c, 0);
    for(int i = 1; i < iterations; i++) {
        z = FunctionMandelbrot(c, z);
    }

    return z;
}

bool Diverges(Complex c) {
    return isnan(c.GetReal()) || isnan(c.GetImg());
}

void Print(std::vector<std::vector<bool>> &BooleanMatrix) {
    

    std::ofstream writer("picture.pbm");
    if(!writer){
        std::cerr << "Error printing";
        return;
    }

    writer << "P1" << std::endl << "2000 3000";
    for(auto i = 0; i < BooleanMatrix.size(); i++){
        writer << std::endl;
        for(auto j = 0; j < BooleanMatrix.at(i).size(); j++){
            writer << (BooleanMatrix.at(i).at(j)) << " ";
        }
    }

}

void *Compute(void* begin) {
    long x = (long)begin;
    for(auto i = x; i < x+250; i++){
        std::cout << i << "/3000" << std::endl;
        BooleanMatrix.at(i).resize(2000);
        ComplexMatrix.at(i).resize(2000);
        for(auto j = 0; j < ComplexMatrix.at(i).size(); j++){
            ComplexMatrix.at(i).at(j) = Complex(3*((1.0*i)/BooleanMatrix.size()) - 2, 2*((1.0*j)/BooleanMatrix.at(i).size()) - 1);
            BooleanMatrix.at(i).at(j) = Diverges(RecursiveMandelbrot(500, ComplexMatrix.at(i).at(j)));
        }
    }

     pthread_exit(0);
}


int main() {
    pthread_t threads[THREADS];
    


    BooleanMatrix.resize(3000);
    ComplexMatrix.resize(3000);

    std::cout << "Creating Matrix" << std::endl;
       for(auto i = 0; i < ComplexMatrix.size(); i++){
        BooleanMatrix.at(i).resize(2000);
        ComplexMatrix.at(i).resize(2000);
    }

    std::cout << "Computing" << std::endl;
    for(long i = 0; i < THREADS; i++){
        pthread_create(&threads[i], nullptr, Compute, (void*)(250*i));
    }

    for(int i = 0; i < THREADS; i++){
        pthread_join(threads[i], nullptr);
    }
    
    std::cout << "Writing" << std::endl;
    Print(BooleanMatrix);

    std::cout << "Done";

    pthread_exit(0);
}