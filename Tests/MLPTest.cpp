#include <gtest/gtest.h>
#include "../Value/Value.h"
#include "../Neuron/Neuron.h"
#include "../Layer/Layer.h"
#include "../MLP/MLP.h"




// Test fixture for Value class tests (optional, but can be helpful to set up common objects and resources)
class MLPTest : public ::testing::Test {
protected:
    // Member variables with default constructors or default-initialized here
    MLP mlp = MLP(2, {3, 1}); // Defines a multi-layer perceptron of 2 inputs  1, 3 input hidden layers and 1 output
    // There is a hardcoded seed in the Layer constructor right now, I might want to add an alternative constructor for the layer that allows for a seed. 
    
    // Optional: Set up common objects and resources before each test case
    void SetUp() override {
        // Optional: Set up common objects and resources for each test case
    }

    // Optional: Clean up common objects and resources after each test case
    void TearDown() override {
        // Optional: Clean up common objects and resources for each test case
    }
};


// Test getter methods
TEST_F(MLPTest, ConstructorAndGetters) {
    std::vector<Layer*> layers = mlp.getLayers();

    //int count = 1;
    //for(Layer* layer : layers){
        //std::cout << "Layer " << count << ": " <<std::endl;
        //layer->testPrint(); // replace with some sort of assertion

        //++count;
    //}
}


// Test Operator Method
TEST_F(MLPTest, MLPOperator) {
    std::vector<Value*> x; // Will have to write alternate operator functions to pass this as an input
    x.push_back(new Value(1.0));
    x.push_back(new Value(2.0));
    x.push_back(new Value(3.0));
    x.push_back(new Value(4.0));
    x.push_back(new Value(5.0));

    std::vector<Value*> y_hat = mlp(x);

    //std::cout << "Prediction vector size: " << y_hat.size() << std::endl;
    for(Value* val : y_hat){
        std::cout << val->getLabel() << ": " << val->getData() << std::endl;
    }

    // NOW THE REAL TEST BACKPROPPING THROUGH THE OUTPUTS
    for(Value* val : y_hat){
        val->backward();
    } 


    // Looks like the gradients are being updated
    for(Layer* layer : mlp.getLayers()){
        layer->testPrint();
    }

    // Will have to devise a way here to test that the values being calculated for the gradients are accurate
    // I can do this by hand, but I would like to automate it somehow
}


// Test main function (optional)
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}