#!/bin/bash

# Compiling source files
# Compile Value.cpp, Neuron.cpp, Layer.cpp, MLP.cpp into object files
clang++ -std=c++14 -arch arm64 -g -c Value/Value.cpp -o Value/Value.o
clang++ -std=c++14 -arch arm64 -g -c Neuron/Neuron.cpp -o Neuron/Neuron.o
clang++ -std=c++14 -arch arm64 -g -c Layer/Layer.cpp -o Layer/Layer.o



# Compiling test files
# Compile ValueTest.cpp into object file
clang++ -std=c++14 -arch arm64 -g -Igoogletest/googletest/include -c Tests/ValueTest.cpp -o Tests/ValueTest.o
#clang++ -std=c++11 -g Value/Value.o -o Value


# Compile NeuronTest.cpp into an object file
clang++ -std=c++14 -arch arm64 -g -Igoogletest/googletest/include -c Tests/NeuronTest.cpp -o Tests/NeuronTest.o
#clang++ -std=c++11 -g Value/Value.o Neuron/Neuron.o -o Neuron

# Compile LayerTest.cpp into an object file
clang++ -std=c++14 -arch arm64 -g -Igoogletest/googletest/include -c Tests/LayerTest.cpp -o Tests/LayerTest.o
#clang++ -std=c++11 -g Value/Value.o Neuron/Neuron.o Layer/Layer.o -o Layer


# Compiling final executable unit test
clang++ -std=c++14 -g Value/Value.o Tests/ValueTest.o googletest/build/lib/libgtest.a -o ValueTest -lpthread
clang++ -std=c++14 -g Value/Value.o Neuron/Neuron.o Tests/NeuronTest.o googletest/build/lib/libgtest.a -o NeuronTest -lpthread
clang++ -std=c++14 -g Value/Value.o Neuron/Neuron.o Layer/Layer.o Tests/LayerTest.o googletest/build/lib/libgtest.a -o LayerTest -lpthread
