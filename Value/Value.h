// Value.h
#ifndef VALUE_H
#define VALUE_H

#include <vector>
#include <unordered_set>
#include <functional>
#include <string>


class Value {
private:
    double data;
    double grad;
    std::string op;
    std::vector<Value*> prev;
    std::string label;
    std::function<void()> _backward;
    Value* ancestor = nullptr;

public:
    Value(double data);

    //* -> means method has passed unit testing

    // Getter methods
    double getData() const; //*
    double getGrad() const; //*
    std::string getOp() const; //*
    std::vector<Value*> getPrev() const; //*
    std::string getLabel() const; //*
    std::function<void()> getBackward() const; //*
    bool hasBackward() const; //*
    Value* getAncestor(); //

    // Print address of object
    void printAddress(); //*
    void printAddresses(); //*


    // Make backward
    std::function<void()> makeBackward(std::string op, Value* a, Value* b); //

    // Setter methods
    void setData(double newData); //*
    void setData(double* newData); //*
    void setGrad(double newGrad); //*
    void setOp(const std::string& newOp); //*
    void setPrev(std::vector<Value*>&& newPrev); //
    void setLabel(const std::string& newLabel); //*
    void setBackward(std::function<void()> newBackward); //
    void setAncestor(Value* newAncestor); //

    // Operator overloads
    Value operator+(Value& other); //*
    //Value operator+(Value* other); //
    Value operator*(Value& other); //*
    Value operator^(double exponent); //*
    Value operator-(); //*
    Value operator-(Value& other); //*
    void operator+=(Value& other); //*



    // Other member functions
    void backward(); //*
    //Value tanh(); //*
    void tanh(); //*

    void relu(); //*


    // Copy constructor for deep copy
    Value(const Value& other) : data(other.data), grad(other.grad), op(other.op), label(other.label), ancestor(other.ancestor) {
        // Deep copy the prev vector
        for (Value* prevValue : other.prev) {
            prev.push_back(new Value(*prevValue));
        }
    }

    // Destructor to handle memory deallocation
    //~Value() {
        // Delete the objects in the prev vector
    //    for (Value* prevValue : prev) {
    //        delete prevValue;
    //    }
    //}
};

#endif
