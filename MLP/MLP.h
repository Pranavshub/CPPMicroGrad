// MLP.h
#ifndef MLP_H
#define MLP_H

#include "Layer.h"
#include <vector>


class MLP{
private:
    std::vector<Layer> layers;

public:
    MLP(int nin, std::vector<int> nouts);
    
    // __call__ method
    std::vector<Value> operator()(const std::vector<Value>& x) const;

    // Getter methods
    std::vector<Layer> getLayers();
    std::vector<Value> parameters() const;

    // Loss Function
    Value MSELoss(std::vector<Value> labels, std::vector<Value> predictions);
};

#endif // MLP_H