#include <iostream>
#include <vector>
#include <cmath>

class Value {
private:
    double data;
    double grad;
    std::vector<Value*> prev;
    std::string op;
    std::string label;
    
    void backward();
    
public:
    Value(double data, std::vector<Value*> children = {}, std::string op = "", std::string label = "")
        : data(data), grad(0), op(op), label(label) {
            prev = children;
    }
    
    double getGrad() const {
        return grad;
    }
    
    void setGrad(double grad) {
        this->grad = grad;
    }
    
    std::vector<Value*> getPrev() const {
        return prev;
    }
    
    std::string getOp() const {
        return op;
    }
    
    std::string getLabel() const {
        return label;
    }
    
    Value operator+(const Value& other) const;
    Value operator*(const Value& other) const;
    Value operator**(double power) const;
    Value operator-() const;
    Value operator-(const Value& other) const;
    Value tanh() const;
    Value exp() const;
    void print() const;
    void backwardPass();
};

Value Value::operator+(const Value& other) const {
    Value out(data + other.data, {const_cast<Value*>(&(*this)), const_cast<Value*>(&other)}, "+");
    
    out.backward = [this, &other, out]() {
        this->grad += 1.0 * out.grad;
        other.grad += 1.0 * out.grad;
    };
    
    return out;
}

Value Value::operator*(const Value& other) const {
    Value out(data * other.data, {const_cast<Value*>(&(*this)), const_cast<Value*>(&other)}, "*");
    
    out.backward = [this, &other, out]() {
        this->grad += other.data * out.grad;
        other.grad += this->data * out.grad;
    };
    
    return out;
}

Value Value::operator**(double power) const {
    assert((power == static_cast<int>(power) && "only supporting int/float powers for now"));
    Value out(std::pow(data, power), {const_cast<Value*>(&(*this))}, "**" + std::to_string(power));
    
    out.backward = [this, power, out]() {
        this->grad += power * std::pow(this->data, power - 1) * out.grad;
    };
    
    return out;
}

Value Value::operator-() const {
    return Value(-data, {const_cast<Value*>(&(*this))}, "-");
}

Value Value::operator-(const Value& other) const {
    return *this + (-other);
}

Value Value::tanh() const {
    double t = (std::exp(2 * data) - 1) / (std::exp(2 * data) + 1);
    Value out(t, {const_cast<Value*>(&(*this))}, "tanh");
    
    out.backward = [this, t, out]() {
        this->grad += (1 - std::pow(t, 2)) * out.grad;
    };
    
    return out;
}

Value Value::exp() const {
    Value out(std::exp(data), {const_cast<Value*>(&(*this))}, "exp");
    
    out.backward = [this, out]() {
        this->grad += out.data * out.grad;
    };
    
    return out;
}

void Value::print() const {
    std::cout << "Value(data=" << data << ")" << std::endl;
}

void Value::backwardPass() {
    std::vector<Value*> topo;
    std::unordered_set<Value*> visited;
    
    auto buildTopo = [&](Value* v) {
        if (visited.find(v) == visited.end()) {
            visited.insert(v);
            for (Value* child : v->getPrev()) {
                buildTopo(child);
            }
            topo.push_back(v);
        }
    };
    
    buildTopo(this);
    grad = 1.0;
    
    for (auto it = topo.rbegin(); it != topo.rend(); ++it) {
        (*it)->backward();
    }
}

void Value::backward() {
    backward();
}

int main() {
    Value a(2.0);
    Value b(3.0);
    Value c(4.0);
    
    Value d = a * b + c;
    Value e = d ** 2;
    Value f = e.tanh();
    Value g = f - a;
    Value h = g.exp();
    
    h.backwardPass();
    
    std::cout << "a.grad: " << a.getGrad() << std::endl;
    std::cout << "b.grad: " << b.getGrad() << std::endl;
    std::cout << "c.grad: " << c.getGrad() << std::endl;
    
    return 0;
}
