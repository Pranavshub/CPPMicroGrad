#include <gtest/gtest.h>
#include "../Value/Value.h"
#include "../Neuron/Neuron.h"



// Test fixture for Value class tests (optional, but can be helpful to set up common objects and resources)
class NeuronTest : public ::testing::Test {
protected:
    // Member variables with default constructors or default-initialized here
    Neuron n = Neuron(3, 120); // seed: 120

    // Default constructor for the test fixture
    NeuronTest() = default;
    
    // Optional: Set up common objects and resources before each test case
    void SetUp() override {
        // Optional: Set up common objects and resources for each test case
    }

    // Optional: Clean up common objects and resources after each test case
    void TearDown() override {
        n.zeroGrad();
    }
};


// Test cases for the Neuron class

// Test getter methods
TEST_F(NeuronTest, ConstructorAndGetters) {
    // Test getter methods

    // Test that all initialized weights contain a value in between -1.0 and 1.0
    for(Value* weight : n.getW()){
        ASSERT_TRUE(weight->getData() >= -1.0 && weight->getData() <= 1.0);
    }

    // Test that the bias value is in between -1.0 and 1.0
    Value* b = n.getB();
    ASSERT_TRUE(b->getData() >= -1.0 && b->getData() <= 1.0);
}


// Testing math of Tanh(W*X+b)
TEST_F(NeuronTest, TanhWXb) {
    // Test operator method
    
    // Setting up necessary testing variables
    std::vector<double> weightValues = {-0.44979528125831636, 0.29070789147667031, -0.072467951603451763};
    double biasValue = 0.86413697892061303; // Hard coding the values for the weights and biases, fuck off, i wanna sleep tonight
    std::vector<Value> x = {Value(1.0), Value(2.0), Value(3.0)};
    x[0].setLabel("x1"); // Actual inputs likely wont have labels
    x[1].setLabel("x2"); // This is to make debugging easier
    x[2].setLabel("x3");
    std::vector<Value*> w = n.getW(); // Do math with pointers instead
    Value* b = n.getB(); // Pointers FTW

    // Making sure that the randomly inialized weights and biases are being loaded correctly according to the seeds
    for(int i = 0; i < w.size(); ++i){
        ASSERT_EQ(w[i]->getData(), weightValues[i]);
    }
    ASSERT_EQ(b->getData(), biasValue);

    // Tanh((W * X) + b)
    // Below is what should be happening inside of the Neuron class operator method
    Value* act = new Value(0.0);
    act->setLabel("act"); // Need to allocate this variable onto the heap somehow

    // W*X
    for(int i = 0; i < w.size(); ++i){
        Value* product = new Value(*w[i] * x[i]); // Allocate new memory for each product on the heap
        product->setLabel(w[i]->getLabel() + "*" + x[i].getLabel()); // Create new Label for product
        *act += *product; // IS THE SOLUTION *
    }
    // (W*X) + b
    *act += *b; 

    // tanh((W*X) + b)
    act->tanh(); 
    ASSERT_EQ(act->getLabel(), "tanh(act+=w1*x1+=w2*x2+=w3*x3+=b)");
    ASSERT_EQ(act->getData(), 0.6517607143450252);// Need to add a seeding option when generating random weight values to make testing more consistent
    
    act->backward(); // Backpropagate through the Neuron

    // Tanh()
    // Testing that gradients are being calculated correctly
    // t = 0.643545
    double t = (std::exp(2 * act->getPrev()[0]->getData()) - 1) / (std::exp(2 * act->getPrev()[0]->getData()) + 1);

    ASSERT_EQ(act->getGrad(), 1.0);
    ASSERT_EQ(act->getPrev()[0]->getGrad(), ((1.0 - t * t) * act->getGrad()));

    for(int i = 0; i < w.size(); ++i){
        ASSERT_EQ(w[i]->getGrad(), (x[i].getData() * w[i]->getAncestor()->getGrad()));
        ASSERT_EQ(x[i].getGrad(), (w[i]->getData() * w[i]->getAncestor()->getGrad()));
    }

    //n.testPrint();
}


// Testing math of ReLU(W*X+b)
TEST_F(NeuronTest, ReLUWXb) {
    
    // Setting up necessary testing variables
    std::vector<double> weightValues = {-0.67370497420137054, -0.9595014024337295, -0.34007070369090453};
    double biasValue = 0.031137400786922065; // Hard coding the values for the weights and biases, fuck off, i wanna sleep tonight
    std::vector<Value> x = {Value(1.0), Value(2.0), Value(3.0)};
    x[0].setLabel("x1"); // Actual inputs likely wont have labels
    x[1].setLabel("x2"); // This is to make debugging easier
    x[2].setLabel("x3");
    std::vector<Value*> w = n.getW(); // Do math with pointers instead
    Value* b = n.getB(); // Pointers FTW

    // Making sure that the randomly inialized weights and biases are being loaded correctly according to the seeds
    for(int i = 0; i < w.size(); ++i){
        ASSERT_EQ(w[i]->getData(), weightValues[i]);
    }
    ASSERT_EQ(b->getData(), biasValue);

    // ReLU((W * X) + b)
    // Below is what should be happening inside of the Neuron class operator method
    Value* act = new Value(0.0);
    act->setLabel("act"); // Need to allocate this variable onto the heap somehow
    
    // W*X
    for(int i = 0; i < w.size(); ++i){
        Value* product = new Value(*w[i] * x[i]); // Allocate new memory for each product on the heap
        product->setLabel(w[i]->getLabel() + "*" + x[i].getLabel()); // Create new Label for product
        *act += *product; // IS THE SOLUTION *
    }

    
    // (W*X) + b
    *act += *b; 

    // tanh((W*X) + b)
    act->relu(); 
    ASSERT_EQ(act->getLabel(), "relu(act+=w1*x1+=w2*x2+=w3*x3+=b)");
    ASSERT_EQ(act->getData(), 0);// Need to add a seeding option when generating random weight values to make testing more consistent

    act->backward(); // Backpropagate through the Neuron

    // ReLU()
    // Testing that gradients are being calculated correctly

    ASSERT_EQ(act->getGrad(), 1.0);
    ASSERT_EQ(act->getPrev()[0]->getGrad(), 0);

    for(int i = 0; i < w.size(); ++i){
        ASSERT_EQ(w[i]->getGrad(), (x[i].getData() * w[i]->getAncestor()->getGrad()));
        ASSERT_EQ(x[i].getGrad(), (w[i]->getData() * w[i]->getAncestor()->getGrad()));
    }

    //n.testPrint();
}



// Testing the Operator method of the Neuron class
// Currently using Tanh as the activation function
TEST_F(NeuronTest, Operator) {
    // Setting up necessary testing variables
    std::vector<double> weightValues = {-0.70269347340925292, -0.16920758931395019, 0.12804640043901583};
    double biasValue = 0.43168306696772718; // Hard coding the values for the weights and biases, fuck off, i wanna sleep tonight
    std::vector<Value> x = {Value(1.0), Value(2.0), Value(3.0)};
    x[0].setLabel("x1"); // Actual inputs likely wont have labels
    x[1].setLabel("x2"); // This is to make debugging easier
    x[2].setLabel("x3");
    std::vector<Value*> w = n.getW(); // Do math with pointers instead
    Value* b = n.getB(); // Pointers FTW

    // Making sure that the randomly inialized weights and biases are being loaded correctly according to the seeds
    for(int i = 0; i < w.size(); ++i){
        ASSERT_EQ(w[i]->getData(), weightValues[i]);
    }
    ASSERT_EQ(b->getData(), biasValue);

    Value* act = n(x); // Call neuron using input vector

    //std::cout << act.getLabel() << " Address: " << act << std::endl;

    act->backward(); // backpropagation

    //Tanh()
    // Testing that gradients are being calculated correctly
    // t = 0.643545
    double t = (std::exp(2 * act->getPrev()[0]->getData()) - 1) / (std::exp(2 * act->getPrev()[0]->getData()) + 1);

    ASSERT_EQ(act->getGrad(), 1.0);
    ASSERT_EQ(act->getPrev()[0]->getGrad(), ((1.0 - t * t) * act->getGrad()));

    for(int i = 0; i < w.size(); ++i){
        ASSERT_EQ(w[i]->getGrad(), (x[i].getData() * w[i]->getAncestor()->getGrad()));
        ASSERT_EQ(x[i].getGrad(), (w[i]->getData() * w[i]->getAncestor()->getGrad()));
    }

    //n.testPrint();
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
