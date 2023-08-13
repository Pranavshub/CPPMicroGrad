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



// Fixed operator method for Layer
std::vector<Value*> Layer::operator()(std::vector<Value*> x){
    std::vector<Value*> activationPtrs;
    for (Neuron* neuron : getNeurons()) {
        Value* act = (*neuron)(x);        
        activationPtrs.push_back(act);
    }
    //std::cout << "A)GIPJSOFHWIP:GHEP:OGHE){}" << std::endl;
    return activationPtrs;
}


/*
std::vector<Value**> Layer::getActivations(std::vector<Value> x){
    std::vector<Value**> activationPtrs;
    for (Neuron* neuron : getNeurons()) {
        Value* act = (*neuron)(x);
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

// Alternative activation function, for testing purposes
// This works, rewrite this as the main operator function for the layer
// The fix seems to be passing the input as a set of pointers, which point to Value objects
// that have been allocated onto the heap. 
std::vector<Value*> Layer::getActivations(std::vector<Value*> x){
    std::vector<Value*> activationPtrs;
    for (Neuron* neuron : getNeurons()) {
        Value* act = (*neuron)(x);        
        activationPtrs.push_back(act);
    }
    //std::cout << "A)GIPJSOFHWIP:GHEP:OGHE){}" << std::endl;
    return activationPtrs;
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
