// Neuron.cpp
#include "Neuron.h"
#include "../Value/Value.h"
#include <iostream>
#include <unordered_set>
#include <vector>
#include <cmath>
#include <string>
#include <functional>
#include <cstdlib>

// Function to initialize random seed (only once)
void Neuron::initializeSeed() {
    static bool seedInitialized = false;
    if (!seedInitialized) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        seedInitialized = true;
    }
}

// Function to initialize a specific seed (only once)
void Neuron::initializeSeed(unsigned int seed) {
    static bool seedInitialized = false;
    if (!seedInitialized) {
        std::srand(seed);
        seedInitialized = true;
    }
}
/*
// Constructor for Neuron class truly random weights and bias
Neuron::Neuron(int nin) {
    initializeSeed();

    b = new Value(std::rand() / (RAND_MAX / 2.0) - 1.0);
    b->setLabel("b");

    // Initialize w as a list of Value objects with random data between -1 and 1
    w.reserve(nin);
    for (int i = 0; i < nin; ++i) {
        Value* weightValue = new Value(std::rand() / (RAND_MAX / 2.0) - 1.0);
        weightValue->setLabel("w" + std::to_string(i + 1));
        w.push_back(weightValue);
    }
}
*/

// Constructor for Neuron class with specific seed for random weights and bias
Neuron::Neuron(int nin, unsigned int seed) {
    initializeSeed(seed);

    b = new Value(std::rand() / (RAND_MAX / 2.0) - 1.0);
    b->setLabel("b");
    
    // Initialize w as a list of Value objects with random data between -1 and 1
    w.reserve(nin);

    for (int i = 0; i < nin; ++i) {
        Value* weightValue = new Value(std::rand() / (RAND_MAX / 2.0) - 1.0);
        weightValue->setLabel("w" + std::to_string(i + 1));
        w.push_back(weightValue);
    }
}


// Getter method for the weight vector
std::vector<Value*> Neuron::getW() {
    //for(Value weight : w){
    //    std::cout << "Weight Address: " << &weight << std::endl;
    //}
    return w;
}

// Getter method for the bias value
Value* Neuron::getB() const{
    return b;
}

// Method to zero the gradients of all parameters
void Neuron::zeroGrad() {
    for (Value* val : w) {
        val->setGrad(0.0);
    }
    b->setGrad(0.0);
}


// Neuron operator method
Value* Neuron::operator()(std::vector<Value>& x){
    std::vector<Value*> w = this->getW(); // Do math with pointers instead
    Value* b = this->getB();

    // W * X + b
    Value* act = new Value(0.0); // Allocated onto the heap
    act->setLabel("act"); 

    for(int i = 0; i < w.size(); ++i){
        Value* product = new Value(*w[i] * x[i]); // Allocate new memory for each product on the heap
        product->setLabel(w[i]->getLabel() + "*" + x[i].getLabel()); // Create new Label for product
        *act += *product; // IS THE SOLUTION *
    }
    *act += *b;
    act->tanh(); // Might switch to ReLU or Sigmoid later

    //std::cout << act->getLabel() << " Address (IN NEURON OEPRATOR): " << act << std::endl;
    return act;
}

// Neuron operator method
Value* Neuron::operator()(std::vector<Value*>& x){
    std::vector<Value*> w = this->getW(); // Do math with pointers instead
    Value* b = this->getB();

    // W * X + b
    Value* act = new Value(0.0); // Allocated onto the heap
    act->setLabel("act"); 

    for(int i = 0; i < w.size(); ++i){
        Value* product = new Value(*w[i] * (*x[i])); // Allocate new memory for each product on the heap
        product->setLabel(w[i]->getLabel() + "*" + (*x[i]).getLabel()); // Create new Label for product
        *act += *product; // IS THE SOLUTION *
    }
    *act += *b;
    act->tanh(); // Might switch to ReLU or Sigmoid later

    //std::cout << act->getLabel() << " Address (IN NEURON OEPRATOR): " << act << std::endl;
    return act;
}

// Method to print parameter data and gradients
void Neuron::testPrint() {
    for(Value* weight : w){
        if(weight->hasBackward()){
            std::cout << "Gay" << std::endl;
            std::cout << weight->getLabel() << " Has Backward" << std::endl;
        }
        std::cout << weight->getLabel() << " Grad: " << weight->getGrad() << std::endl;
        //std::cout << weight->getLabel() << " Data: " << weight->getData() << std::endl;
        //std::cout << weight->getLabel() << " Address: " << weight << std::endl;
    }
    
    std::cout << b->getLabel() << " Grad: " << b->getGrad() << std::endl;
    //std::cout << b->getLabel() << " Data: " << b->getData() << std::endl;
    //std::cout << b->getLabel() << " Address: " << b << std::endl;
}

/*





void Neuron::testPrint() {
    for(Value& val : w){
        if(val.hasBackward()){
            std::cout << "Gay" << std::endl;
            std::cout << val.getLabel() << " Has Backward" << std::endl;
        }
        std::cout << val.getLabel() << " Grad: " << val.getGrad() << std::endl;
        std::cout << val.getLabel() << " Data: " << val.getData() << std::endl;
        //std::cout << val.getLabel() << " Address: " << &val << std::endl;
    }
    
    std::cout << b.getLabel() << " Grad: " << b.getGrad() << std::endl;
    std::cout << b.getLabel() << " Data: " << b.getData() << std::endl;
    //std::cout << b.getLabel() << " Address: " << &b << std::endl;
}
*/

/*
int main() {
    Neuron n = Neuron(3);

    for(Value weight : n.w){
        std::cout << "Weight Address" << &weight << std::endl;
        //std::cout << "Weight Value" << weight.getData() << std::endl;
    }

    std::cout << "----------------------------------" << std::endl;
   
    for(Value* param : n.parameters()){
        std::cout << "Param Address" << param << std::endl;
    }
}
*/