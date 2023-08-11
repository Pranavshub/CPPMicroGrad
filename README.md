# Neural Network Framework

Welcome to the Neural Network Framework! This project is designed to provide a flexible and modular framework for constructing, training, and running inference on Multi-Layer Perceptrons (MLPs) or neural networks. Whether you're just getting started with neural networks or looking to experiment with various architectures, this framework is here to support your endeavors.

## Current state of project

So far the Value and Neuron classes are working as intended. I am currently experiencing issues with the Layer class and the MLP class. I am working on fixing these issues and will update the project as soon as I can. Gradient calculation works properly within the Value and Neuron classes. The Layer class is able to calculate the forward pass but is unable to calculate the backward pass. The MLP class is able to calculate the forward pass but is unable to calculate the backward pass. Gradient optimization is not yet implemented. I will write the code for it once I get gradient calculation working properly in both the Layer and MLP classes. 

## Overview

The Neural Network Framework consists of several key classes that work in tandem to create and operate neural networks:

- **Value Class**: This class represents a basic unit that stores a double value and its associated gradient. It plays a crucial role in storing and managing the values used in neural network computations.

- **Neuron Class**: Neurons are the building blocks of neural networks. Each neuron has weights and a bias, both represented as instances of the Value class. Neurons handle computations within themselves, including both forward and backward passes.

- **Layer Class**: A layer is a collection of neurons, where each neuron processes inputs and contributes to the layer's output. The Layer class manages these neurons and their connections, facilitating the aggregation of neuron outputs within a layer.

- **MLP Class**: The Multi-Layer Perceptron (MLP) class serves as the central structure of the neural network. It orchestrates the stacking of multiple layers to form a complete neural network. The MLP class manages the flow of data from input to output, ensuring sequential computations through the layers.

## Functionality

The Neural Network Framework provides the following functionalities:

- Construction of custom neural network architectures through the Layer and MLP classes.
- Training of neural networks using gradient-based optimization techniques.
- Forward and backward pass computations for calculating gradients and updating weights.
- Inference on trained networks for making predictions on new data.
- Memory management to avoid memory leaks and optimize memory usage.

## Getting Started

To begin using the Neural Network Framework, follow these steps:

1. Clone this repository to your local machine.
2. Navigate to the project directory and explore the classes mentioned above.
3. Run build.sh to compile the project.
4. Run ./run in your terminal to run the example program.
5. For unit testing run ./test to compile the test files
6. Look within the Tests folder and run the test file that corresponds with whatever class you wich to test. 
7. Utilize the provided classes to create your custom neural network architectures.

## Example Usage

Here's a brief example of how you might create a simple neural network using the framework:

```cpp

int main() {
    MLP mlp = MLP(3, {4, 4, 1}); // Defines a multi-layer perceptron of 3 inputs  2, 4 input hidden layers and 1 output
    // Look at MLP.h for more details on the constructor and its methods
}
