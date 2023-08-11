// Layer.cpp
#include "../Layer/Layer.h"
#include "../Neuron/Neuron.h"
#include "../Value/Value.h"

#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>


// nout -> # of neuron in layer
// nin -> # of weigths in each neuron
// Constructor for Layer
Layer::Layer(int nin, int nout) {
    for(int i = 0; i < nout; ++i){
        Neuron neuron(nin, 567); // Hard coded SEED here, will remove later: SEED = 567
        neurons.push_back(neuron);
    }
}
/*
// NEED TO FIGURE THIS SHIT OUT
// Operator method for Layer
std::vector<Value*> Layer::operator()(std::vector<Value> x){
    //std::vector<Value> activations;
    std::vector<Value*> activationPtrs;
    for (Neuron* neuron : getNeurons()) {
        Value* actPtr = (*neuron)(x); // Allocate new Value object on the heap
        //Value act = (*neuron)(x); // Call neuron using input vector
        //Value* actPtr = &act;
        //activations.push_back(act);

        activationPtrs.push_back(actPtr);
        
        //std::cout << activationPtrs.back()->getLabel() << " Address (IN LAYER OEPRATOR): " << activationPtrs.back() << std::endl;

    }


    //for(int i = 0; i < activationPtrs.size(); ++i){  
        //std::cout << activationPtrs[i]->getLabel() << " Address (IN LAYER OP): " << activationPtrs[i] << std::endl; //
    //}


    return activationPtrs;
}
*/

// NEED TO FIGURE THIS SHIT OUT
// Operator method for Layer
std::vector<Value*> Layer::operator()(std::vector<Value> x){
    std::vector<Value*> activationPtrs;
    for (Neuron* neuron : getNeurons()) {
        Value * act = (*neuron)(x);
        act->backward();
        
        activationPtrs.push_back(act);
    }
    return activationPtrs;
}

std::vector<Value**> Layer::getActivations(std::vector<Value> x){
    std::vector<Value**> activationPtrs;
    for (Neuron* neuron : getNeurons()) {
        Value * act = (*neuron)(x);
        //act->backward();

        Value** actPtr = new Value*(act);
        
        activationPtrs.push_back(actPtr);
        
        
        //std::cout << act << std::endl;
        //std::cout << "------------------------" << std::endl;
        //std::cout << actPtr << std::endl;
        //std::cout << "++++++++++++++++++++++++" << std::endl;
    }

    //std::cout << "(In Operator) Layer Output Address: " << activationPtrs[0] << std::endl;
    //(*activationPtrs[0])->backward();

    //std::cout << "(In Operator) Vector Address: " << &activationPtrs << std::endl;
    return activationPtrs;
}


/*
// Neuron operator method
Value Neuron::operator()(std::vector<Value>& x){
    std::vector<Value*> w = this->getPointers(); // Do math with pointers instead
    Value* bPtr = this->getBPointer();

    // W * X + b
    Value act(0.0);
    act.setLabel("act");

    for(int i = 0; i < w.size(); ++i){
        Value* product = new Value(*w[i] * x[i]); // Allocate new memory for each product on the heap
        product->setLabel(w[i]->getLabel() + "*" + x[i].getLabel()); // Create new Label for product
        act += *product; // IS THE SOLUTION *
    }
    act += *bPtr;
    act.tanh(); // Might switch to ReLU or Sigmoid later
    return act;

}

*/

std::vector<Neuron*> Layer::getNeurons() {
    std::vector<Neuron*> neuronPointers;

    for (Neuron& neuron : neurons) {
        //std::cout << "Neuron Address: " << &neuron << std::endl;
        Neuron* ptr = &neuron;
        neuronPointers.push_back(ptr);
    }

    return neuronPointers;  

}

void Layer::testPrint(){
    std::cout << "Layer Address: " << this << std::endl;
    int count = 1;
    for (Neuron& neuron : neurons) {
        std::cout << "Neuron " << count << " Address: " << &neuron << std::endl;
        neuron.testPrint();
        ++count;
    }
}


/*
int main(){
    //std::cout << "hello" << std::endl;

    Layer l1(5, 3); // 3 neurons with 5 weights each


    for (const Value& value : l1.parameters()) {
        std::cout << value.getLabel() << ": " << value.getData() << std::endl;
    }
}

*/
