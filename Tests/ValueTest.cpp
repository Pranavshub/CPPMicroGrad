#include <gtest/gtest.h>
#include "../Value/Value.h"

// IMPORTANT NOTE: any time .backward() is called on a variable in a test case, the gradients of all variables
// preceeding that variable will have biased gradients (i.e. the gradients will be non-zero). This is because
// the .backward() function is called on the entire computational graph, and the gradients are accumulated
// as they are calculated. To avoid this, you can call .backward() on the variable in the test case before
// any other .backward() calls are made on other variables. Or once done with a test case, you can call zero all
// the gradients in the computational graph. See the ValueTest class below for an example of how to do this.
// Test fixture for Value class tests (optional, but can be helpful to set up common objects and resources)
class ValueTest : public ::testing::Test {
protected:
    // Member variables with default constructors or default-initialized here
    Value a = Value(2.0);
    Value b = Value(3.0);

    // Default constructor for the test fixture
    ValueTest() = default;

    // Optional: You can also explicitly define the destructor
    ~ValueTest() override = default;

    // Optional: Set up common objects and resources before each test case
    void SetUp() override {
        // Optional: Set up common objects and resources for each test case
        a.setData(2.0);
        b.setData(3.0);

        a.setLabel("a");
        b.setLabel("b");
    }

    // Optional: Clean up common objects and resources after each test case
    void TearDown() override {
        // Optional: Clean up common objects and resources after each test case
        // Zero Gradients
        a.setGrad(0);
        b.setGrad(0);

        // Reset Op fields
        a.setOp("");
        b.setOp("");

        // Reset Prev vectors
        a.setPrev({});
        b.setPrev({});

        // Reset Backward functions
        a.setBackward([]() {});
        b.setBackward([]() {});
    }
};


// Test cases for the Value class

// Test constructor and getter methods
TEST_F(ValueTest, ConstructorAndGetters) {
    // Test getter methods
    ASSERT_EQ(a.getData(), 2.0);
    ASSERT_EQ(b.getData(), 3.0);

    ASSERT_EQ(a.getGrad(), 0.0); // Assuming the default grad value is 0.0
    ASSERT_EQ(b.getGrad(), 0.0);
    
    ASSERT_EQ(a.getOp(), ""); // Assuming the default op value is an empty string
    ASSERT_EQ(b.getOp(), "");

    ASSERT_EQ(a.getLabel(), "a");
    ASSERT_EQ(b.getLabel(), "b");

    ASSERT_EQ(a.getBackward(), nullptr);
    ASSERT_EQ(b.getBackward(), nullptr);
}

// Test setters
TEST_F(ValueTest, Setters) {
    // Test setter methods

    a.setData(5.0);
    b.setData(6.0);
    ASSERT_EQ(a.getData(), 5.0);
    ASSERT_EQ(b.getData(), 6.0);

    a.setGrad(5.0);
    b.setGrad(6.0);
    ASSERT_EQ(a.getGrad(), 5.0);
    ASSERT_EQ(b.getGrad(), 6.0);

    a.setOp("+");
    b.setOp("^");
    ASSERT_EQ(a.getOp(), "+");
    ASSERT_EQ(b.getOp(), "^");

    //Come back to this
    //std::vector<Value> values= {Value(2.0), Value(3.0)};
    //a.setPrev(&values);
    //b.setPrev("^");
    //ASSERT_EQ(a.getOp(), "+");
    //ASSERT_EQ(b.getOp(), "^");

    a.setLabel("A");
    b.setLabel("B");
    ASSERT_EQ(a.getLabel(), "A");
    ASSERT_EQ(b.getLabel(), "B");

    //Test setBackward later too
}


// Test arithmetic operations
TEST_F(ValueTest, ArithmeticOperations) {
    // Test the arithmetic operations (+, -, *, ^) for the Value class
    
    // Test '+' operator
    Value c = a+b;
    c.setLabel("c");
    ASSERT_EQ(c.getData(), 5.0);
    ASSERT_EQ(a.getAncestor() -> getLabel(), "c");
    ASSERT_EQ(b.getAncestor() -> getLabel(), "c");

    // Test gradient calculation for '+' operator
    c.backward();
    ASSERT_EQ(c.getGrad(), 1.0);
    ASSERT_EQ(a.getGrad(), 1.0);
    ASSERT_EQ(b.getGrad(), 1.0);
    a.setGrad(0.0);
    b.setGrad(0.0);

    // Test '*' operator
    Value d = a*b;
    d.setLabel("d");
    ASSERT_EQ(d.getData(), 6.0);
    ASSERT_EQ(a.getAncestor() -> getLabel(), "d");
    ASSERT_EQ(b.getAncestor() -> getLabel(), "d");

    // Test gradient calculation for '+' operator
    d.backward();
    ASSERT_EQ(d.getGrad(), 1.0);
    ASSERT_EQ(a.getGrad(), 3.0);
    ASSERT_EQ(b.getGrad(), 2.0);
    a.setGrad(0.0);
    b.setGrad(0.0);

    // Test '^' operator
    Value e = a^2.0;
    e.setLabel("e");
    ASSERT_EQ(e.getData(), 4.0);
    ASSERT_EQ(a.getAncestor() -> getLabel(), "e");

    // Test gradient calculation for '*' operator
    e.backward();
    ASSERT_EQ(e.getGrad(), 1.0);
    ASSERT_EQ(a.getGrad(), 4.0); // Not working check later
    a.setGrad(0.0);

    // Test '-' inverter operator
    Value f = -a;
    f.setLabel("f");
    ASSERT_EQ(f.getData(), -2.0);
    ASSERT_EQ(a.getAncestor() -> getLabel(), "f");

    // Test gradient calculation for '-' inverter operator
    f.backward();
    ASSERT_EQ(f.getGrad(), 1.0);
    ASSERT_EQ(a.getGrad(), -1.0);
    a.setGrad(0.0);

    // Test '-' operator
    Value g = b - a;
    g.setLabel("g");
    ASSERT_EQ(g.getData(), 1.0);
    ASSERT_EQ(a.getAncestor() -> getLabel(), "g");
    ASSERT_EQ(b.getAncestor() -> getLabel(), "g");

    // Test gradient calculation for '-' operator
    g.backward();
    ASSERT_EQ(g.getGrad(), 1.0);
    ASSERT_EQ(a.getGrad(), -1.0);
    ASSERT_EQ(b.getGrad(), 1.0);
    a.setGrad(0.0);
    b.setGrad(0.0);
    
    
    // Test '+=' summing operator
    Value h = Value(2.0);
    h.setLabel("h");
    std::vector<Value*> values = {&a, &b};
    //std::vector<Value*> values = {&a};
    for(Value* value : values){
        h += *value; //NOTE: Use dereferenced pointers when using '+=' operator
    }

    ASSERT_EQ(h.getData(), 7.0);
    ASSERT_EQ(a.getAncestor() -> getLabel(), "h+=a+=b");
    ASSERT_EQ(b.getAncestor() -> getLabel(), "h+=a+=b");

    // Test gradient calculation for '+=' summing operator
    h.backward();//_backward is getting called multiple times for each child of h   
    ASSERT_EQ(h.getGrad(), 1.0);
    ASSERT_EQ(a.getGrad(), 1.0);
    ASSERT_EQ(b.getGrad(), 1.0);
    a.setGrad(0.0);
    b.setGrad(0.0);
    
}


// Test chaining operations
TEST_F(ValueTest, ChainingOperations) {
    // Test the chaining operations (+=, *=, -=, ^=) for the Value class
    // Example: Create a computational graph of Value objects, apply the chaining operations, and then check if the result is correct
    // Testing Chained Operations
    Value c = Value(5.0);
    c.setLabel("c"); 
    Value i = a + b;
    Value j = i * c;
    i.setLabel("i");
    j.setLabel("j");
    ASSERT_EQ(j.getData(), 25.0);

    ASSERT_EQ(a.getAncestor() -> getLabel(), "i");
    ASSERT_EQ(b.getAncestor() -> getLabel(), "i");
    ASSERT_EQ(c.getAncestor() -> getLabel(), "j");
    ASSERT_EQ(i.getAncestor() -> getLabel(), "j");


    // Test gradient calculation for chained operations
    j.backward();
    ASSERT_EQ(i.getGrad(), 5.0); 
    ASSERT_EQ(c.getGrad(), 5.0); 
    ASSERT_EQ(a.getGrad(), 5.0);
    ASSERT_EQ(b.getGrad(), 5.0);

    // Test gradient calculation for chained operations
    i.backward();
    ASSERT_EQ(i.getGrad(), 1.0); 
    ASSERT_EQ(a.getGrad(), 1.0);
    ASSERT_EQ(b.getGrad(), 1.0);


}


// Test activation functions
TEST_F(ValueTest, ActivationFunctions) {
    // Test the activation functions (tanh and relu) for the Value class
    // Testing Tanh activation function
    a.tanh();

    ASSERT_EQ(a.getData(), 0.9640275800758169);
    ASSERT_EQ(a.getLabel(), "tanh(a)");

    a.backward();

    ASSERT_EQ(a.getGrad(), 1.0);
    ASSERT_EQ(a.getPrev()[0]->getGrad(), 0.07065082485316443);

    
    // Testing ReLU activation function
    b.relu();

    ASSERT_EQ(b.getData(), 3.0);
    ASSERT_EQ(b.getLabel(), "relu(b)");

    b.backward();

    ASSERT_EQ(b.getGrad(), 1.0);
    ASSERT_EQ(b.getPrev()[0]->getGrad(), 1.0);
    
    
}

// Test backward method
TEST_F(ValueTest, Backward) {
    // Test the backward method of the Value class
    // Example: Create a computational graph of Value objects, set gradients, and then call the backward method to check if gradients are correctly propagated
}

// Add more test cases as needed to cover other functionalities and edge cases of the Value class

// Test main function (optional)

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}