#!/bin/bash

# Compiling source files
# Compile Value.cpp, Neuron.cpp, Layer.cpp, MLP.cpp into object files
clang++ -std=c++14 -arch arm64 -g -c Value.cpp -o Value.o
#clang++ -std=c++11 -g Value.o -o Value

clang++ -std=c++14 -arch arm64 -g -c Neuron.cpp -o Neuron.o
#clang++ -std=c++11 -g Value.o Neuron.o -o Neuron

clang++ -std=c++14 -arch arm64 -g -c Layer.cpp -o Layer.o
#clang++ -std=c++11 -g Value.o Neuron.o Layer.o -o Layer

clang++ -std=c++14 -arch arm64 -g -c MLP.cpp -o MLP.o
#clang++ -std=c++11 -g Value.o Neuron.o Layer.o MLP.o -o MLP

clang++ -std=c++14 -arch arm64 -g -c main.cpp -o main.o

# Compiling test files
# Compile ValueTest.cpp into object file
clang++ -std=c++14 -arch arm64 -g -Igoogletest/googletest/include -c Tests/ValueTest.cpp -o ValueTest.o

# Compile NeuronTest.cpp into an object file
clang++ -std=c++14 -arch arm64 -g -Igoogletest/googletest/include -c Tests/NeuronTest.cpp -o NeuronTest.o



# Link the object files into the final application
clang++ -std=c++14 -g Value.o Neuron.o Layer.o MLP.o main.o -o run
