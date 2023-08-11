// Value.cpp
#include "Value.h"
#include <iostream>
#include <unordered_set>
#include <vector>
#include <cmath>
#include <string>
#include <functional>
#include <typeinfo>


Value::Value(double data) : data(data), grad(0) {}

// Getter method to retrieve the data value
double Value::getData() const {
    return data;
}

// Setter method to set the data value
void Value::setData(double newData) {
    data = newData;
}

// Setter method to set the data value
void Value::setData(double* newData) {
    data = *newData;
}

// Getter method for grad
double Value::getGrad() const{
    return grad;
}

// Setter method for grad
void Value::setGrad(double newGrad){
    grad = newGrad;
}

// Getter method for operation
std::string Value::getOp() const{
    return op;
}

// Setter method for operation
void Value::setOp(const std::string& newOp){
    op = newOp;
}

// Getter method for previous values
std::vector<Value*> Value::getPrev() const{
    return prev;
}

// Setter method for previous values
void Value::setPrev(std::vector<Value*>&& newPrev) {
    prev = newPrev;
}

// Setter method for label
void Value::setLabel(const std::string& newLabel){
    label = newLabel;
}

// Getter method for label
std::string Value::getLabel() const{
    return label;
}

// Setter method for backward function
void Value::setBackward(std::function<void()> new_backward) {
    _backward = new_backward;
}

// Getter method for backward function
std::function<void()> Value::getBackward() const{
    return _backward;
}

// Boolean check for backward function
bool Value::hasBackward() const{
    return _backward != nullptr;
}

// Getter method for ancestor
void Value::setAncestor(Value* newAncestor) {
    ancestor = newAncestor;
}

// Setter method for ancestor
Value* Value::getAncestor() {
    return ancestor;
}

// Method to make a new _backward lambda given an operator and two Values
std::function<void()> Value::makeBackward(std::string op, Value* a, Value* b) {
    std::function<void()> _backward;
    
    if (op == "+") { // Edited is CORRECT (I think)
        _backward = [a, b, this]() { // Capture a and b by value            
            Value* ancestor = this->getAncestor();// Invalid ancestor issue, add null check
            double ancestorGrad;
            std::string thisLabel = this->getLabel();
            // Ancestor Null Check
            if(ancestor == nullptr){
                ancestorGrad = 1.0;
            }
            else{
                ancestorGrad = ancestor->getGrad();
            }
            a->grad += 1.0 * (ancestorGrad);
            b->grad += 1.0 * (ancestorGrad); 
        };
    }else if (op == "*") {// Not edited, is WRONG
        _backward = [&]() {
            a->grad += b->data * (this->getAncestor()->getGrad());
            b->grad += a->data * (this->getAncestor()->getGrad());
        };
    } else if (op == "^") {// Not edited, is WRONG
        _backward = [&]() {
            double power = std::pow(a->data, b->data - 1);
            a->grad += b->data * power * (this->getAncestor()->getGrad());
            b->grad += std::log(a->data) * power * (this->getAncestor()->getGrad());
        };
    } else if (op == "-") {// Not edited, is WRONG
        _backward = [&]() {
            a->grad += 1.0 * (this->getAncestor()->getGrad());
            b->grad -= 1.0 * (this->getAncestor()->getGrad());
        };
    } else if (op == "tanh") {// Not edited, is WRONG
        _backward = [&]() {
            double t = (std::exp(2 * a->data) - 1) / (std::exp(2 * a->data) + 1);
            a->grad += (1 - t * t) * (this->getAncestor()->getGrad());
        };
    } else if (op == "relu") {// Not edited, is WRONG
        _backward = [&]() {
            double r = (a->data > 0) ? a->data : 0;
            a->grad += (a->data > 0) ? (this->getAncestor()->getGrad()) : 0; 
        };
    } else if (op == "+=") { // Edited is CORRECT (I think)
        // b = copy
        // a = this
        _backward = [b]() {
            for(Value* val : b -> getPrev()){
                //std::cout << val->label << std::endl;
                val->grad += 1.0 * (b -> getGrad()); 
            }
        };
    } else {
        // Handle the case when op is not one of the above (optional)
        //std::cout << "Warning: op is not one of the above." << std::endl;
    }
    return _backward;
}



// Overload the addition operator to add two Value objects
Value Value::operator+(Value& other) {
    double sum = data + other.data;
    Value out(sum);

    // Add this and the other Value to the prev field of the new Value
    out.prev.push_back(this);
    out.prev.push_back(const_cast<Value*>(&other)); // Add the pointer to the other object to the prev vector of the result
    
    // Update the op field of this Value to be "+"
    out.op = "+";
    
    std::function<void()> _backward = [&]() {
        grad += 1.0 * (this -> getAncestor() -> getGrad());
        other.grad += 1.0 * (this -> getAncestor() -> getGrad());
        };
    out._backward = _backward;

    this->setAncestor(&out);
    other.setAncestor(&out);

    return out;
}


// Overload the multiplication operator to multiply two Value objects
Value Value::operator*(Value& other) {
    double product = data * other.data;
    Value out(product);

    // Add this and the other Value to the prev field of the new Value
    out.prev.push_back(this);
    out.prev.push_back(const_cast<Value*>(&other));

    // Update the op field of this Value to be "*"
    out.op = "*";

    std::function<void()>_backward = [&]() {
        double calculatedGrad = other.data * (this -> getAncestor() -> getGrad());
        double weightData = data;
        double inputData = other.data;
        double ancestorGrad = this -> getAncestor() -> getGrad();

        //std::cout << this->getAncestor()->getLabel() <<" Address: " <<  this -> getAncestor() << std::endl;
        
        grad += other.data * (this -> getAncestor() -> getGrad());
        other.grad += data * (this -> getAncestor() -> getGrad());
    };
    out._backward = _backward;

    this->setAncestor(&out);
    other.setAncestor(&out);

    return out;
}

// Overload the exponentiation operator to raise a Value object to the power of an int or float
Value Value::operator^(double exponent) {

    double minExp = (exponent - double(1.0));

    double result = std::pow(data, exponent);
    Value out(result);

    // Add this Value to the prev field of the new Value
    out.prev.push_back(this);

    // Update the op field of this Value to be "^"
    out.op = "^";

    std::function<void()> _backward = [=]() {
        double power = std::pow(data, minExp);
        double partialResult =  exponent * power * (this -> getAncestor() -> getGrad());
        grad += partialResult;
    };
    out._backward = _backward;

    this->setAncestor(&out);

    return out;
}


// Overload the unary minus operator to invert a Value object
Value Value::operator-() {
    double invertedData = -data;
    Value out(invertedData);

    // Add this Value to the prev field of the new Value
    out.prev.push_back(this);

    // Update the op field of this Value to be "-"
    out.op = "-";

    std::function<void()> _backward = [=]() {
        grad += -(this -> getAncestor() -> getGrad());
    };
    out._backward = _backward;

    this->setAncestor(&out);

    return out;
}

// Overload the binary minus operator to perform subtraction
Value Value::operator-(Value& other) {
    double difference = data - other.data;
    Value out(difference);

    // Add this and the other Value to the prev field of the new Value
    out.prev.push_back(this);
    out.prev.push_back(const_cast<Value*>(&other));

    // Update the op field of this Value to be "-"
    out.op = "-";

    std::function<void()> _backward = [&]() {
        grad += 1.0 * (this -> getAncestor() -> getGrad());
        other.grad -= 1.0 * (this -> getAncestor() -> getGrad());
    }; 
    out._backward = _backward;

    this->setAncestor(&out);
    other.setAncestor(&out);

    return out;
}


// Overload for summing operator
void Value::operator+=(Value& other) {
    if(this->getOp() != "+="){
        //Value copy = *this;
        Value* copy = new Value(*this); // Allocated onto the heap
        copy->setLabel(this -> getLabel());

        // Shove our copy into the prev vector of this Value
        // Our copy adopts all of the fields of this Value
        copy->setData(this->getData());
        copy->setOp(this->getOp());
        copy->setPrev(this->getPrev());
        copy->setBackward(this->getBackward()); // Might have issue with this later

        copy->setAncestor(this); 

        other.setAncestor(this);

        this->setPrev({copy, &other}); // Replace the prev of this to just contain our copy
        this->setData(this->getData() + other.getData()); // Perform addition operation
        this->setOp("+=");
        this->setLabel(this->getLabel() + "+=" + other.getLabel());


        // Create _backward function for this Value
        std::function<void()> _backward = [&]() {
            for(Value* val : this -> getPrev()){
                //std::cout << val->label << std::endl;
                val->grad += 1.0 * (this -> getGrad()); // Becuase the ancestor to each Value in prev is this Value
            }
        };

        this->setBackward(_backward);
    } else{
        other.setAncestor(this);

        // Perform addition operation
        this->setData(this->getData() + other.getData());
        this->setLabel(this->getLabel() + "+=" + other.getLabel());
        this->setOp("+=");

        std::vector<Value*> newPrev = this->getPrev();
        newPrev.push_back(&other);

        this->setPrev(std::move(newPrev));
    }
}

/*
        Value* copy = new Value(*this);
        copy->setLabel(this -> getLabel());

        // Shove our copy into the prev vector of this Value
        // Our copy adopts all of the fields of this Value
        copy->setData(this->getData());
        copy->setOp(this->getOp());
        copy->setPrev(this->getPrev());
        copy->setBackward(this->getBackward()); 
        copy->setAncestor(this); 

        other.setAncestor(this);

        this->setPrev({copy, &other}); // Replace the prev of this to just contain our copy
        this->setData(this->getData() + other.getData()); // Perform addition operation
        this->setOp("+=");
        this->setLabel(this->getLabel() + "+=" + other.getLabel());


        // Create _backward function for this Value
        std::function<void()> _backward = [&]() {
            for(Value* val : this -> getPrev()){
                //std::cout << val->label << std::endl;
                val->grad += 1.0 * (this -> getGrad()); // Becuase the ancestor to each Value in prev is this Value
            }
        };

        this->setBackward(_backward);
*/

// tanh activation function
void Value::tanh() {
    double x = data;
    double t = (std::exp(2 * x) - 1) / (std::exp(2 * x) + 1);

    // Create exact copy of this Value and shove it into the prev vector
    Value* copy = new Value(*this); // Allocated onto the heap
    copy->setData(this->getData());
    copy->setOp(this->getOp());

    std::vector<Value*> newPrev = this->getPrev();
    //copy->setPrev(this->getPrev());
    copy->setPrev(std::move(newPrev));
   
    copy->setAncestor(this); 
    
    std::function<void()> newBackward = makeBackward(copy->getOp(), this, copy); // Would be wrong if op != "+="
    // But the only values on which an activation function will be called would be the result of a sum, (I think)
    copy->setBackward(newBackward); 
    //copy->setBackward(this->getBackward()); // There is an issue here, 'this' within the _backward is not in reference to copy


    // Then alter this Value as if it has undergone a tanh activation
    this->setPrev({copy});
    this->setData(t); // Performing tanh operation
    this->setOp("tanh");
    this->setLabel("tanh(" + this->getLabel() + ")");

    std::function<void()> _backward = [x, t, copy]() {
        copy->grad += (1 - t * t) * (copy -> getAncestor() -> getGrad());
    };

    this->setBackward(_backward);
}

// ReLU activation function
void Value::relu() {
    double x = data;
    double r = (x > 0) ? x : 0;

    // Create exact copy of this Value and shove it into the prev vector
    Value* copy = new Value(*this);
    copy->setData(this->getData());
    copy->setOp(this->getOp());

    std::vector<Value*> newPrev = this->getPrev();
    //copy->setPrev(this->getPrev());
    copy->setPrev(std::move(newPrev));
   
    copy->setAncestor(this); 
    
    std::function<void()> newBackward = makeBackward(copy->getOp(), this, copy); // Would be wrong if op != "+="
    // But the only values on which an activation function will be called would be the result of a sum, (I think)
    copy->setBackward(newBackward); 
    //copy->setBackward(this->getBackward()); // There is an issue here, 'this' within the _backward is not in reference to copy


    // Then alter this Value as if it has undergone a ReLU activation
    this->setPrev({copy});
    this->setData(r); // Performing tanh operation
    this->setOp("relu");
    this->setLabel("relu(" + this->getLabel() + ")");

    std::function<void()> _backward = [x, copy]() {
        copy->grad += (x > 0) ? (copy -> getAncestor() -> getGrad()) : 0;
    };

    this->setBackward(_backward);
}

// Backpropogation function, calculates the gradient of current Value object as well as all previous objects
void Value::backward() {
    std::vector<Value*> topo;
    std::unordered_set<Value*> visited;
    
    // Helper function for topological sort
    std::function<void(Value*)> buildTopo = [&](Value* v) {
        if (visited.find(v) == visited.end()) {
            visited.insert(v);
            for (Value* child : v->getPrev()) {
                buildTopo(child);
            }
            //std::cout << v->getLabel() << " Address: " << v << std::endl;
            topo.push_back(v);
        }
    };

    buildTopo(this);

    //grad = 1.0;

    this->setGrad(1.0);

    
    // Zero all gradients in the computational graph
    // Might want to take this out and make it a function on its own.
    // DEFINITELY WANT TO TAKE THIS OUT AND MAKE IT A FUNCTION ON ITS OWN
    // THIS IS WHAT IS MESSING WITH MY NEURON GRADIENT CALCULATION
    for (auto it = topo.rbegin(); it != topo.rend(); ++it) {
        std::string label = (*it) -> getLabel(); 
        double data = (*it) -> getData(); 
        std::string op = (*it) -> getOp();
        if ((*it)!= this) {
            (*it)->setGrad(0.0);
        } 
    }
    
    

    // Calculate new gradients
    for (auto it = topo.rbegin(); it != topo.rend(); ++it) {
        Value* currentValue = *it; // Track the Value object pointed by the iterator
        std::string label = (*it) -> getLabel(); 
        double data = (*it) -> getData(); 
        std::string op = (*it) -> getOp();
        std::function<void()> _backward = (*it) -> getBackward();

        // Call _backward for every member of topo
        if ((*it)->getBackward()) {
            (*it)->getBackward()(); // One W2*X2 the prev Value gradients arent being updated
        }
    }
}

void Value::printAddress() {
    std::cout << "Address of " << this->getLabel() << ": " << this << std::endl;
}

// Prints the addresses of ll Value objects in the computational graph
void Value::printAddresses(){
    std::vector<Value*> topo;
    std::unordered_set<Value*> visited;

    // The segmentation fault I am currently dealing with seems to occur because of this topological sort function
    // The same function is being used inside of the backward() method, and it seems to be the cause of the seg faults
    // when running bacwkward(), which is good because it means that the issue isn't being caused by the gradient calculations.
    // However, I am completely stumped as to what is causing the seg fault in this topo function.
    // The worst part is that the seg faults are inconsistent, sometimes the code executes without issue and the rest of the time
    // I get seg faults. There isn't a single identifiable line of code that is causing the seg fault, it seems to be random.
    // I've combed through the registers and it seems the seg fault is always occuring due to an attempt to read data being stored
    // in the 'x1' general purpose register. I'm trying to find out what variable is supposed to be stored in this register when the fault occurs. 
    // Helper function for topological sort 
    std::function<void(Value*)> printTopo = [&](Value* v) {
        if (visited.find(v) == visited.end()) {
            visited.insert(v);
            for (Value* child : v->getPrev()) {
                printTopo(child);
            }
            topo.push_back(v);
            std::cout << v->getLabel() << " Address: " << v << std::endl;
        }
    };

    printTopo(this);
   
}

/*

int main() {
    Value a(2.0);
    a.setLabel("a");
    //std::cout << "Data: " << a.getData() << std::endl;
    //std::cout << "Grad: " << a.getGrad() << std::endl;

    Value b(3.0);
    b.setLabel("b");
    //std::cout << "Data: " << b.getData() << std::endl;

    a.setGrad(0);
    b.setGrad(0);

    Value c = a ^ 3.0;
    c.setLabel("c");

    //std::cout << "Data: " << c.getData() << std::endl;

    //std::cout << "Previous Values:";
    //const std::vector<Value*>& prev = c.getPrev();
    //for (Value* value : prev) {
    //    std::cout << " " << value->getLabel();
    //}
    //std::cout << std::endl;

    //std::cout << "Backward pass" << std::endl;
    c.backward();
    std::cout << "C Data: "<< c.getData() << ", C Grad: " << c.getGrad() << std::endl;
    std::cout << "A Data: "<< a.getData() << ", A Grad: " << a.getGrad() << std::endl;//Something wrong with exponent gradient calculation
    std::cout << "B Data: "<< b.getData() << ", B Grad: " << b.getGrad() << std::endl;

    //if (b.getBackward() != nullptr) {
    // The pointer is not null, it points to a valid function
    //std::cout << "Function pointer is not null." << std::endl;
    //} else {
    // The pointer is null
    //std::cout << "Function pointer is null." << std::endl;
    //}       
}
*/