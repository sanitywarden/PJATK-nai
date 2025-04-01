#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "entry.hpp"

class Perceptron {
public:
    double _w1;
    double _w2;
    double _w3;
    double _w4;
    double _threshold;

    Perceptron(double w1, double w2, double w3, double w4, double threshold)
        : _w1(w1), _w2(w2), _w3(w3), _w4(w4), _threshold(threshold) {}

    double net(double x1, double x2, double x3, double x4) {
        return _w1 * x1 + _w2 * x2 + _w3 * x3 + _w4 * x4;
    }

    bool compute(double x1, double x2, double x3, double x4) {
        return net(x1, x2, x3, x4) >= _threshold;
    }

    std::string decision_from_file(std::string filename, Entry entry) {
        auto from_file_sorted = entries_from_file_sorted(filename, entry);
        return from_file_sorted[0].decision;
    }

    void learn(Entry entry) {
        auto from_file = decision_from_file("iris_training.txt", entry);
        auto decision = compute(
            entry.attributes[0], 
            entry.attributes[1], 
            entry.attributes[2], 
            entry.attributes[3]) ? "iris-setosa" : "other";
        
        std::cout << "Guess: " << decision << " | Real: " << from_file << "\n";

        auto d = from_file == "iris-setosa";
        auto y = decision == "iris-setosa";

        if(d == y)
            return;

        _w1 = _w1 + (d - y) * _threshold * entry.attributes[0];
        _w2 = _w2 + (d - y) * _threshold * entry.attributes[1];
        _w3 = _w3 + (d - y) * _threshold * entry.attributes[2];
        _w4 = _w4 + (d - y) * _threshold * entry.attributes[3];

        std::cout << "\tWeights changed " << _w1 << " " << _w2 << " " << _w3 << " " << _w4 << "\n";
    }

    void train() {
        auto training = entries_from_file("iris_training.txt");
        for(auto data : training)
            this->learn(data);
    }
};

int main() {
    double w1;
    double w2;
    double w3;
    double w4;
    double threshold;

    std::cout << "input weights (4) and threshold (1): \n"; 
    std::cin >> w1 >> w2 >> w3 >> w4;

    Perceptron p = Perceptron(w1, w2, w3, w4, threshold);
    p.train();

    double x1;
    double x2;
    double x3;
    double x4;
    std::cout << "input data to test (4): \n";
    std::cin >> x1;
    std::cin >> x2;
    std::cin >> x3;
    std::cin >> x4;
    std::cout << "result for your data net=" << p.net(x1, x2, x3, x4) << " is " << p.compute(x1, x2, x3, x4) << "\n";
}