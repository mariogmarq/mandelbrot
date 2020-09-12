/**
 * Mandelbrot set PBM image generator
 * @file main.cpp
 * @author mariogmarq
 */

#include "complex.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>
#include <pthread.h>
#include <cstdlib>
#define THREADS 16

/**
 * struct with the necesary data to compute
 */
struct ComputeData
{
    int begin;
    int gap;
    pthread_t thread;
    std::vector<std::vector<bool>> *matrix = nullptr;
};

/**
 * @brief exit the program with message as output
 * @param message the text to be shown
 */
void errorBreak(const std::string &message)
{
    std::cerr << message;
    exit(-1);
}

/**
 * @brief Computes one iteration of the mandelbrot succession (z^2 + c)
 * @param c The number to be checked, if it diverges it doesn't belong to the mandelbrot set
 * @param z The result of the previous iteration of the succession. It startes in 0
 */
Complex FunctionMandelbrot(Complex c, Complex z)
{
    return (z * z) + c;
}

/**
 * @brief Applies FunctionMandelbrot() recursirvely over iterations iterations
 * @param iterations The iterations of the succession
 * @param c The complex number to be checked
 */
Complex RecursiveMandelbrot(int iterations, Complex c)
{
    //First iteration, since z = 0 for the first
    auto z = FunctionMandelbrot(c, static_cast<Complex>(0));

    for (int i = 1; i < iterations; i++)
    {
        z = FunctionMandelbrot(c, z);
    }

    return z;
}

/**
 * @brief Check if @param c has diverged as a result of a succession
 * @return Wether c has diverged or not
 */
bool Diverges(Complex c)
{
    return isnan(c.GetReal()) || isnan(c.GetImg()) || c.Module() > 100.0;
}

/**
 * @brief Writes BooleanMatrix as a PBM image called "picture.pbm"
 */
void Print(std::vector<std::vector<bool>> &BooleanMatrix)
{

    std::ofstream writer("picture.pbm");
    if (!writer)
    {
        std::cerr << "Error printing";
        return;
    }

    //Magic number of a PBM image
    writer << "P1" << std::endl
           << BooleanMatrix.at(0).size() << " " << BooleanMatrix.size();
    for (auto i = 0; i < BooleanMatrix.size(); i++)
    {
        writer << std::endl;
        for (auto j = 0; j < BooleanMatrix.at(i).size(); j++)
        {
            writer << (BooleanMatrix.at(i).at(j)) << " ";
        }
    }
}

/**
 * @brief pthread ready function that calculates BooleanMatrix
 */
void *Compute(void *Data)
{
    auto data = static_cast<ComputeData *>(Data);
    for (auto i = data->begin; i < data->matrix->size(); i += data->gap)
    {
        for (auto j = 0; j < data->matrix->at(0).size(); j++)
        {
            data->matrix->at(i).at(j) = Diverges(RecursiveMandelbrot(500,
                                                                     Complex(3 * ((1.0 * i) / data->matrix->size()) - 2, 2 * ((1.0 * j) / data->matrix->at(i).size()) - 1)));
        }
    }

    pthread_exit(0);
}

int main(int nargs, char **args)
{
    //Check that there is a height and a width passed from command line
    if (nargs < 3)
    {
        errorBreak("Please, enter in the program call the size of the picture\n");
    }
    int height = atoi(args[1]), width = atoi(args[2]);

    ComputeData threads[THREADS];
    std::vector<std::vector<bool>> BooleanMatrix;

    //Resize the matrix
    std::cout << "Creating Matrix" << std::endl;

    BooleanMatrix.resize(height);

    for (auto i = 0; i < BooleanMatrix.size(); i++)
    {
        BooleanMatrix.at(i).resize(width);
    }

    //Runs Compute(void* Data)
    std::cout << "Computing" << std::endl;

    for (auto i = 0; i < THREADS; i++)
    {
        threads[i].matrix = &BooleanMatrix;
        threads[i].gap = THREADS;
        threads[i].begin = i;
        pthread_create(&threads[i].thread, nullptr, Compute, &threads[i]);
    }

    //Join all the Threads so the main function wait for the other threads to end
    for (int i = 0; i < THREADS; i++)
    {
        pthread_join(threads[i].thread, nullptr);
    }

    //Write the file
    std::cout << "Writing" << std::endl;
    Print(BooleanMatrix);

    std::cout << "Done" << std::endl;

    pthread_exit(0);
}
