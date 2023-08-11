// MLP.cpp
#include "MLP.h"
#include "Layer.h"
#include "Neuron.h"
#include "Value.h"

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
std::vector<Value> MLP::operator()(const std::vector<Value>& x) const{
    std::vector<Value> temp = x;
    for (const Layer& layer : layers) {
        temp = layer(temp);
    }
    
    return temp;
}

// Getter method for all layers in the MLP
std::vector<Layer> MLP::getLayers(){
    return layers;
}

// Getter method for all parameter Values in the MLP
std::vector<Value> MLP::parameters() const{
    std::vector<Value> params;
    for(const Layer& layer : layers){
        for(const Value& value : layer.parameters()){
            params.push_back(value);
        }
    }
    return params;
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
