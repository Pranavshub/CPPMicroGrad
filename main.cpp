// main.cpp

#include "MLP.h"
#include "Layer.h"
#include "Neuron.h"
#include "Value.h"

#include <iostream>
#include <vector>

int main(){
    
    //std::vector<int> nouts = {4, 4, 1};
    //MLP mlp(3, nouts);     // Defines a multi-layer perceptron of 3 inputs  2, 4 input hidden layers and 1 output

    /*
    int layerCount = 0;
    int neuronCount = 0;
    for (Layer& layer : mlp.getLayers()) {
        ++layerCount;
        std::cout << "Layer " << layerCount << ":" << std::endl;
        neuronCount = 0;
        for(Neuron& neuron : layer.getNeurons()){
            ++neuronCount;
            std::cout << "Neuron " << neuronCount << ":" << std::endl;
            for (Value& value : neuron.parameters()) {
                std::cout << value.getLabel() << ": " << value.getData() << std::endl;
            }
        }
    }
    */
    /*
    // Training data
    std::vector<std::vector<Value> > xs = {
        {Value(2.0), Value(3.0), Value(-1.0)},
        {Value(3.0), Value(-1.0), Value(0.5)},
        {Value(0.5), Value(1.0), Value(1.0)},
        {Value(1.0), Value(1.0), Value(-1.0)}
    };
    */

    //double element = xs[1][1].getData();
    //std::cout << element << std::endl;

    /*
    // Testing data
    std::vector<double> ys = {1.0, -1.0, -1.0, 1.0}; // desired object target

    // Forward pass inference on the network
    std::vector<Value> ypreds;
    for(std::vector<Value> x : xs){
        //std::cout << x[0].getData() << std::endl;

        std::vector<Value> pred = mlp(x);

        //std::cout << "Prediction size: " << pred.size() << std::endl;

        ypreds.insert(ypreds.end(), pred.begin(), pred.end());
    }
    */

    //std::cout << ys.size() << std::endl;

    //std::cout<< "Label size: " << ys.size() << std::endl;


    //std::cout<< "Total Prediction size: " << ypreds.size() << std::endl;

    /*
    for(Value& value : ypreds){
        std::cout << value.getData() << std::endl;
    }
    */


    /*
    //Testing single neuron input -> output

    Layer l1 = mlp.getLayers()[0];
    Neuron n1 = l1.getNeurons()[0];
    std::vector<Value> n1Params = n1.parameters();

    std::cout << "Neuron 1 parameters: " << std::endl;
    for(Value& value : n1Params){
        std::cout << value.getLabel() << ": " << value.getData() << std::endl;
    }

    std::cout << "Neuron 1 output: " << std::endl;
    std::vector<Value> x1 = xs[0];
    Value n1Output = n1(x1); // Each individual neuron outputs a single value
    std::cout << n1Output.getData() << std::endl;
    // Getting the output from a single neuron seems to work
    */

    // Testing single layer input -> output
    //std::cout << "Layer 1 parameters: " << std::endl;
    //std::vector<Value> l1Params = l1.parameters();
    //for(Value& value : l1Params){
    //    std::cout << value.getLabel() << ": " << value.getData() << std::endl;
    //}

    //Layer l1 = mlp.getLayers()[0];

    //std::cout << "Layer 1 output: " << std::endl;
    //std::vector<Value> x1 = xs[0];
    //std::vector<Value> l1Output = l1(x1); // Each individual neuron outputs a single value

    //for(Value& value : l1Output){
    //    std::cout << value.getData() << std::endl;
    //}
    //std::cout << "Output length: " << l1Output.size() << std::endl;

    //std::cout << "# of Layers: " << mlp.getLayers().size() << std::endl;

    // Testing MLP input -> output going layer by layer
    
    //Layer l2 = mlp.getLayers()[1];
    //Layer l3 = mlp.getLayers()[2];

    //std::vector<Value> l2Output = l2(l1Output);
    //std::vector<Value> l3Output = l3(l2Output);

    //std::cout << "Layer2 Output Length: " << l2Output.size() << std::endl;

    //std::cout << "Layer3 Output Length: " << l3Output.size() << std::endl;

    // End to end pass seems to work: |3 inputs -> 4 outputs| -> |4 inputs -> 4 outputs| -> |4 intputs -> 1 output|

    // Testing MLP input -> output in one pass
    //std::vector<Value> mlpOutput = mlp(x1);

    //std::cout << "MLP Output Length: " << mlpOutput.size() << std::endl; // Has size 3 but should be 1
    
    //std::cout << "Layer3 Output: " << l3Output[0].getData() << std::endl;
    //std::cout << "MLP Output: " << mlpOutput << std::endl; // Has size 3 but should be 1
    //for(const Value value : mlpOutput){
    //    std::cout << value.getData() << std::endl;
    //}

    // Something is wrong with the MLP operator, it should be a single value but it is 3 values
    // Nevermind I got the MLP operator working, the issue was very simple 

    // Testing Value Exponentiation Operator
    //Value a(2.0);
    //Value b(2.0);

    //a.setLabel("a");
    //b.setLabel("b");

    //Value c = a + b;

    //std::cout << c.getData() << std::endl;

    // Testing Exponent Backprop
    //c.backward();

    //c = -c;

    //std::cout << c.getData() << std::endl;

    //c.backward();

    //std::cout << c.getOp() << std::endl;

    // Testing MSE Loss Function
    //std::vector<Value> labels = {Value(1.0), Value(2.0), Value(3.0)};
    //std::vector<Value> predictions = {Value(1.0), Value(2.0), Value(3.0)};
    //Value loss = mlp.MSELoss(labels, predictions);

    Value a(2.0);
    Value b(3.0);
    a.setLabel("a");
    b.setLabel("b");

    Value c = a + b;
    c.setLabel("c");
    
    //std::cout << a.getData() << std::endl;

    //a.backward();
    c.backward();

    //std::cout << "a data: " << a.getData() << std::endl;

    //std::cout <<"a op: "<< a.getOp() << std::endl;
    //std::cout <<"a grad: "<< a.getGrad() << std::endl;

    
    for(Value* val : c.getPrev()){ // Something weird with the prev vector
        std::cout << val -> getLabel()<< " data: " << val -> getData() << std::endl;
        std::cout << val -> getLabel() << " grad: " << val -> getGrad() << std::endl;

    }
    

}