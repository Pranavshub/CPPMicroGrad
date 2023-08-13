// MLP.cpp
#include "../Value/Value.h"
#include "../Neuron/Neuron.h"
#include "../Layer/Layer.h"
#include "../MLP/MLP.h"


#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>


// Constructor for MLP
MLP::MLP(int nin, std::vector<int> nouts) {
    std::vector<int> sz = nouts;

    sz.insert(sz.begin(), nin);

    // Populate layers vector
    for(int i = 0; i < nouts.size(); ++i){
        Layer newLayer = Layer(sz[i], sz[i+1]); // Creates a new layer with the number of weights of the previous layer's number of neurons and the number of neurons as there will be weights in the next layer
        layers.push_back(newLayer); // Add each new layer onto the main layers vector
    }
}


// Operator method for MLP
std::vector<Value*> MLP::operator()(const std::vector<Value*>& x) {
    std::vector<Value*> activations = x;

    //std::cout << "# of Layers: " << getLayers().size() << std::endl;

    // Get the activations of each layer
    int count = 1;
    for (Layer* layer : getLayers()) {
        activations = (*layer)(activations);
        
        // WILL COMMENT THIS LATER, THIS IS MEANT FOR TESTING PURPOSES
        if(count != getLayers().size()){
            for(Value* val : activations){
                val-> setLabel("Layer#" + std::to_string(count) + "output");
            }
        }

        ++count;
    }


    return activations;
}

// Getter method for the layers field
std::vector<Layer*> MLP::getLayers() {
    std::vector<Layer*> layerPointers;

    for (Layer& layer : layers) {
        //std::cout << "Neuron Address: " << &neuron << std::endl;
        Layer* ptr = &layer;
        layerPointers.push_back(ptr);
    }

    return layerPointers;  

}


// Check and see if this works and then check if you have call .backward() on the results of this function.
// Mean Squared Error Loss Function
/*
Value MLP::MSELoss(std::vector<Value> labels, std::vector<Value> predictions){
    if(labels.size() != predictions.size()){
        std::cout << "Error: labels and predictions must be the same size" << std::endl;
        exit(1);
    }
    else{
        double lossSum = 0.0;
        for (int i = 0; i < labels.size(); ++i) {
            Value squaredDifference = (predictions[i] - labels[i])^2.0;
            lossSum += squaredDifference;
        }

        double meanSquaredError = lossSum / static_cast<double>(labels.size());
        return Value(meanSquaredError);
    }
    return Value(0.0);
}
*/

/*
int main() {
    MLP mlp = MLP(3, {4, 4, 1}); // Defines a multi-layer perceptron of 3 inputs  2, 4 input hidden layers and 1 output
    

    int layerCount = 0;
    int neuronCount = 0;
    for (Layer& layer : mlp.getLayers()) {
        ++layerCount;
        std::cout << "Layer " << layerCount << ":" << std::endl;
        neuronCount = 0;
        for(Neuron& neuron : layer.getNeurons()){
            ++neuronCount;
            std::cout << "Neuron " << neuronCount << ":" << std::endl;
            for (Value& value : neuron.parameters()) {
                std::cout << value.getLabel() << ": " << value.getData() << std::endl;
            }
        }
    }

}
*/
