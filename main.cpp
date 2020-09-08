#include "complex.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>
#include <pthread.h>
#include <cstdlib>
#define THREADS 16

std::vector<std::vector<bool>> BooleanMatrix;
std::vector<std::vector<Complex>> ComplexMatrix;

struct ComputeData {
    int begin;
    int gap;
    pthread_t thread;
};

void errorBreak(const std::string &message){
    std::cerr << message;
    exit(-1);
}

Complex FunctionMandelbrot(Complex c, Complex z) {
    return (z*z) + c;
}

Complex RecursiveMandelbrot(int iterations, Complex c) {
    //First iteration
    auto z = FunctionMandelbrot(c, static_cast<Complex>(0));
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

    writer << "P1" << std::endl << BooleanMatrix.at(0).size() << " " << BooleanMatrix.size();
    for(auto i = 0; i < BooleanMatrix.size(); i++){
        writer << std::endl;
        for(auto j = 0; j < BooleanMatrix.at(i).size(); j++){
            writer << (BooleanMatrix.at(i).at(j)) << " ";
        }
    }

}

void *Compute(void* Data) {
    auto data = static_cast<ComputeData*>(Data);  
    for(auto i = data->begin; i < data->begin+data->gap && i < BooleanMatrix.size(); i++){
        for(auto j = 0; j < ComplexMatrix.at(0).size(); j++){
            ComplexMatrix.at(i).at(j) = Complex(3*((1.0*i)/BooleanMatrix.size()) - 2, 2*((1.0*j)/BooleanMatrix.at(i).size()) - 1);
            BooleanMatrix.at(i).at(j) = Diverges(RecursiveMandelbrot(500, ComplexMatrix.at(i).at(j)));
        }
    }

     pthread_exit(0);
}


int main(int nargs, char** args) {
    if(nargs < 3){
        errorBreak("Please, enter in the program call the size of the picture");
    }
    int height=atoi(args[1]), width=atoi(args[2]);


    ComputeData threads[THREADS];
    
    BooleanMatrix.resize(height);
    ComplexMatrix.resize(height);

    std::cout << "Creating Matrix" << std::endl;
       for(auto i = 0; i < ComplexMatrix.size(); i++){
        BooleanMatrix.at(i).resize(width);
        ComplexMatrix.at(i).resize(width);
    }

    std::cout << "Computing" << std::endl;

    //gap between iterations of multiple threads
    auto gap = int(ceil((height * 1.0)/THREADS)); 
    for(auto i = 0; i < THREADS; i++){
        threads[i].gap = gap;
        threads[i].begin = gap*i;
        std::cout << gap*i << "/" << height<< "\n";
        pthread_create(&threads[i].thread, nullptr, Compute, &threads[i]);
    }

    for(int i = 0; i < THREADS; i++){
        pthread_join(threads[i].thread, nullptr);
    }
    
    std::cout << "Writing" << std::endl;
    Print(BooleanMatrix);

    std::cout << "Done";

    pthread_exit(0);
}
