#include <gtest/gtest.h>
#include "../Value/Value.h"
#include "../Neuron/Neuron.h"
#include "../Layer/Layer.h"



// Test fixture for Value class tests (optional, but can be helpful to set up common objects and resources)
class LayerTest : public ::testing::Test {
protected:
    // Member variables with default constructors or default-initialized here
    Layer l = Layer(5, 3); // 3 neurons with 5 weights each
    // There is a hardcoded seed in the Layer constructor right now, I might want to add an alternative constructor for the layer that allows for a seed. 

    // Default constructor for the test fixture
    LayerTest() = default;
    
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
TEST_F(LayerTest, ConstructorAndGetters) {
    // Bruh i don't know how to test this
    std::vector<Neuron*> neurons = l.getNeurons();
    //l.testPrint(); // it work tho

}


// Test Neuron Operator call and Backprop
TEST_F(LayerTest, NeuronOperator) {
    // Setting up necessary testing variables
    std::vector<Neuron*> neurons = l.getNeurons();
    std::vector<Value> x = {Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)}; // Each neuron has 5 weights and so we have 5 inputs
    x[0].setLabel("x1"); // Actual inputs likely wont have labels
    x[1].setLabel("x2"); // They might, depends on how lazy im feeling when running actual inference
    x[2].setLabel("x3"); // This is to make debugging easier
    x[3].setLabel("x4");
    x[4].setLabel("x5"); 

    for(Neuron* neuron : neurons){
        std::vector<Value*> w = neuron->getW(); 
        Value* bPtr = neuron->getB(); 
       
        // bruh this works, fuggin how?
        Value* act = (*neuron)(x); // Call neuron using input vector

        //std::cout << act.getLabel() << " Gay Address: " << &act << std::endl; //


        //std::cout << act.getLabel() << " Address: " << &act << std::endl;
        act->backward(); // backpropagation

        //Tanh() activation function, cuz ReLU causing gradient death and sigmoid cringe (meaning I haven't written it yet)
        // Testing that gradients are being calculated correctly
        double t = (std::exp(2 * act->getPrev()[0]->getData()) - 1) / (std::exp(2 * act->getPrev()[0]->getData()) + 1);

        ASSERT_EQ(act->getGrad(), 1.0);
        ASSERT_EQ(act->getPrev()[0]->getGrad(), ((1.0 - t * t) * act->getGrad()));

        for(int i = 0; i < w.size(); ++i){
            ASSERT_EQ(w[i]->getGrad(), (x[i].getData() * w[i]->getAncestor()->getGrad()));
            ASSERT_EQ(x[i].getGrad(), (w[i]->getData() * w[i]->getAncestor()->getGrad()));
        }
        
    }
    
    //l.testPrint(); 
}


// Test Layer Operator call and Backprop
TEST_F(LayerTest, LayerOPPtr) {
    // Setting up necessary testing variables
    std::vector<Neuron*> neurons = l.getNeurons();
    //std::vector<Value> x = {Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)}; // Each neuron has 5 weights and so we have 5 inputs
    /*
    x[0].setLabel("x1");
    x[1].setLabel("x2");
    x[2].setLabel("x3");
    x[3].setLabel("x4");
    x[4].setLabel("x5");
    */

    std::vector<Value*> x; // Will have to write alternate operator functions to pass this as an input
    x.push_back(new Value(1.0));
    x.push_back(new Value(2.0));
    x.push_back(new Value(3.0));
    x.push_back(new Value(4.0));
    x.push_back(new Value(5.0));

    
    std::vector<Value*> testPtrs = l(x); // Testing returning a vector of pointers to pointers

    //testPtrs[0]->printAddresses(); // Useful function

    // FIXED FINALLY!
    int i = 0;
    for(Value* ptr : testPtrs){
        std::vector<Value*> w = neurons[i]->getW(); 
        Value* b = neurons[i]->getB(); 


        ptr->backward();

        //Tanh() activation function, cuz ReLU causing gradient death and sigmoid cringe (meaning I haven't written it yet)
        // Testing that gradients are being calculated correctly
        double t = (std::exp(2 * ptr->getPrev()[0]->getData()) - 1) / (std::exp(2 * ptr->getPrev()[0]->getData()) + 1);

        ASSERT_EQ(ptr->getGrad(), 1.0);
        ASSERT_EQ(ptr->getPrev()[0]->getGrad(), ((1.0 - t * t) * ptr->getGrad()));

        for(int i = 0; i < w.size(); ++i){
            ASSERT_EQ(w[i]->getGrad(), (x[i]->getData() * w[i]->getAncestor()->getGrad()));
            ASSERT_EQ(x[i]->getGrad(), (w[i]->getData() * w[i]->getAncestor()->getGrad()));
        }

        ++i;
    }

    //l.testPrint();
}


// Test main function (optional)
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
