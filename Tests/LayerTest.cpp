#include <gtest/gtest.h>
#include "../Value/Value.h"
#include "../Neuron/Neuron.h"
#include "../Layer/Layer.h"



// Test fixture for Value class tests (optional, but can be helpful to set up common objects and resources)
class LayerTest : public ::testing::Test {
protected:
    // Member variables with default constructors or default-initialized here
    Layer l = Layer(5, 3); // 3 neurons with 5 weights each

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

/*
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
TEST_F(LayerTest, LayerOperator) {
    // Setting up necessary testing variables
    std::vector<Neuron*> neurons = l.getNeurons();
    std::vector<Value> x = {Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)}; // Each neuron has 5 weights and so we have 5 inputs
    x[0].setLabel("x1"); 
    x[1].setLabel("x2"); 
    x[2].setLabel("x3"); 
    x[3].setLabel("x4");
    x[4].setLabel("x5");

    // Aright lets trying doing this again with pointers instead
    //std::vector<Value*> activationPtrs = l(x);
    std::vector<Value*> activationPtrs = l(x);
    
    for(int i = 0; i < activationPtrs.size(); ++i){
        std::vector<Value*> w = neurons[i]->getW(); 
        Value* b = neurons[i]->getB(); 

        Value* actPtr = activationPtrs[i];

        double t = (std::exp(2 * actPtr->getPrev()[0]->getData()) - 1) / (std::exp(2 * actPtr->getPrev()[0]->getData()) + 1);

        ASSERT_EQ(actPtr->getGrad(), 1.0);
        ASSERT_EQ(actPtr->getPrev()[0]->getGrad(), ((1.0 - t * t) * actPtr->getGrad()));

        for(int i = 0; i < w.size(); ++i){
            ASSERT_EQ(w[i]->getGrad(), (x[i].getData() * w[i]->getAncestor()->getGrad()));
            // Not testing inpupt gradient here cux a. doesnt matter and b. memory fuckery
        }
    } 
}
*/

// Test Layer Operator call and Backprop
TEST_F(LayerTest, LayerOPPtr) {
    // Setting up necessary testing variables
    std::vector<Neuron*> neurons = l.getNeurons();
    std::vector<Value> x = {Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)}; // Each neuron has 5 weights and so we have 5 inputs
    x[0].setLabel("x1");
    x[1].setLabel("x2");
    x[2].setLabel("x3");
    x[3].setLabel("x4");
    x[4].setLabel("x5");

    Value* act = (*neurons[0])(x); // Call neuron using input vector
    //std::cout << "Neuron Output Adrress: " << act << std::endl;

    //act->printAddresses(); // Useful functions
    //std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
    
    //act->backward();

    // Act of backpropping doesn't affect addresses of members of the computational graph
    //act->printAddresses(); // Useful function
    
    std::vector<Value**> testPtrs = l.getActivations(x); // Testing returning a vector of pointers to pointers
    //std::cout << "Layer Output Adress: " << testPtrs[0] << std::endl; 
    //std::cout << "Layer Output Vector Address: " << &testPtrs << std::endl; 

    //(*testPtrs[0])->backward();
    (*testPtrs[0])->printAddresses(); // Segmentation fault occuring here as well

    // Stepping between the line above and the line below sometimes results in a segmentation fault
    // The fault always occurs on this line of machine code
    //181798144: 22 0C 40 AC                ldnp   q2, q3, [x1]
    

    std::cout << (*testPtrs[0]) -> getLabel() << std::endl; // Seg fault occurs on accessing x1.  
    //std::cout << "+++++++++++++++++++++" << std::endl; 


    //int count = 0;
    //for(Value** ptr : testPtrs){
        //++count;
        //(*ptr)->setLabel(std::to_string(count) + " " + (*ptr)->getLabel());
        //std::cout << (*ptr)->getLabel() << std::endl; // gotta figure out how to use pointers to pointers
        //(*ptr)->setData(100.0);
        //std::cout << (*ptr)->getPrev()[0]->getPrev()[1]->getPrev()[0]->getLabel() << std::endl;
        //(*ptr)->getPrev()[0]->getPrev()[1]->getPrev()[0]->setData(100.0);
        //(*ptr)->backward();
        //(*ptr)->printAddress();
    //}
    //l.testPrint();
}




// Object addresses change every run
/*
Layer Address: 0x139e06da0
Neuron 1 Address: 0x139e070a0
w1 Address: 0x139e08210
w2 Address: 0x139e08290
w3 Address: 0x139e08310
w4 Address: 0x139e08390
w5 Address: 0x139e08410
b Address: 0x139e070b8
Neuron 2 Address: 0x139e07138
w1 Address: 0x139e07f90
w2 Address: 0x139e08010
w3 Address: 0x139e08090
w4 Address: 0x139e08110
w5 Address: 0x139e08190
b Address: 0x139e07150
Neuron 3 Address: 0x139e071d0
w1 Address: 0x139e07d10
w2 Address: 0x139e07d90
w3 Address: 0x139e07e10
w4 Address: 0x139e07e90
w5 Address: 0x139e07f10
b Address: 0x139e071e8
*/

// Consistent Weight and Bias Values
/*
Layer Address: 0x13f7043a0
Neuron 1 Address: 0x13f7046a0
w1 Data: -0.998075
w2 Data: -0.641704
w3 Data: 0.878909
w4 Data: -0.172068
w5 Data: 0.0607404
b Data: 0.86396
Neuron 2 Address: 0x13f704738
w1 Data: 0.572976
w2 Data: 0.00072035
w3 Data: 0.106916
w4 Data: 0.934452
w5 Data: -0.658489
b Data: 0.776428
Neuron 3 Address: 0x13f7047d0
w1 Data: -0.582525
w2 Data: -0.495261
w3 Data: 0.151562
w4 Data: -0.693752
w5 Data: 0.102748
b Data: 0.893754
*/



/*
Layer Address: 0x147e06da0
Neuron 1 Address: 0x147e070a0
w1 Data: -0.998075
w2 Data: -0.641704
w3 Data: 0.878909
w4 Data: -0.172068
w5 Data: 0.0607404
b Data: 0.86396
Neuron 2 Address: 0x147e07138
w1 Data: 0.572976
w2 Data: 0.00072035
w3 Data: 0.106916
w4 Data: 0.934452
w5 Data: -0.658489
b Data: 0.776428
Neuron 3 Address: 0x147e071d0
w1 Data: -0.582525
w2 Data: -0.495261
w3 Data: 0.151562
w4 Data: -0.693752
w5 Data: 0.102748
b Data: 0.893754
*/


// Test main function (optional)
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}