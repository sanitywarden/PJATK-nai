#include <vector>
#include <string>
#include <fstream>
#include <iostream>

class Perceptron {
public:
    double _w1;
    double _w2;
    double _s1;
    double _s2;
    double _threshold;

    Perceptron(double w1, double w2, double s1, double s2, double threshold)
        : _w1(w1), _w2(w2), _s1(s1), _s2(s2), _threshold(threshold) {}

    double net() {
        return _w1 * _s1 + _w2 * _s2;
    }

    bool compute() {
        auto net_numeric = net();
        auto net = net_numeric >= _threshold;
        return net;
    }
};

int main() {
    double w1;
    double w2;
    double s1;
    double s2;
    double threshold;
    std::cout << "input weights, signals and threshold (w1, w2, s1, s2, threshold): \n";
    std::cin >> w1;
    std::cin >> w2;
    std::cin >> s1;
    std::cin >> s2;
    std::cin >> threshold;
    Perceptron p = Perceptron(w1, w2, s1, s2, threshold);
    std::cout << "result for your data net=" << p.net() << " is " << p.compute() << "\n";
}