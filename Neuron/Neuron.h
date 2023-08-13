// Nueron.h
#ifndef NEURON_H
#define NEURON_H

#include "../Value/Value.h"
#include <vector>

class Neuron {
private:
    //std::vector<Value> w;
    //Value b;

public:
    std::vector<Value*> w;
    Value* b;

    //* -> means method has passed unit testing

    Neuron(int nin);
    Neuron(int nin, unsigned int seed);

    // Getter methods
    std::vector<Value*> getW(); //*
    Value* getB() const; //*

    // Zero all parameter gradients
    void zeroGrad(); //*

    // __call__ method
    Value* operator()(std::vector<Value>& x); //*
    Value* operator()(std::vector<Value*>& x); //*


    // Test print method
    void testPrint(); //*

    // Initialize seed
    void initializeSeed(); //*
    void initializeSeed(unsigned int seed); //*




};

#endif // NEURON_H