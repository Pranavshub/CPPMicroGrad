// Layer.h
#ifndef LAYER_H
#define LAYER_H

#include "../Neuron/Neuron.h"
#include <vector>

class Layer {
private:
    std::vector<Neuron> neurons;

public:
    Layer(int nin, int nout);

    // __call__ method
    std::vector<Value*> operator()(std::vector<Value> x);
    //void operator()(std::vector<Value> x);
    std::vector<Value**> getActivations(std::vector<Value> x);



    // Getter method for the values of all parameters of the Layer
    std::vector<Neuron*> getNeurons();
    std::vector<Value**> parameters();

    // Print parameter values
    void testPrint();



};

#endif // LAYER_H