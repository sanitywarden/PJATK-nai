#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>

struct Entry {
    std::vector<double> attributes;
    std::string label;
};

class Perceptron {
private:
    std::vector<double> weights;
    double learning_rate;

public:
    // properties = 4
    Perceptron(size_t property_count, double learning_rate = 0.1) 
        : weights(property_count, 0.0), learning_rate(learning_rate) {}

    // calculate net value
    double net(const std::vector<double>& inputs) {
        double sum = 0;
        for (size_t i = 0; i < inputs.size(); ++i)
            sum += inputs[i] * weights[i];
        return sum;
    }

    // classify inputs to be either a 'iris-setosa' or 'other'
    int compute(const std::vector<double>& inputs) {
        // threshold theta is 0
        return net(inputs) >= 0.0 ? 1 : 0;
    }
    
    // teach the perceptron
    void train(const std::vector<Entry>& training_data, int rounds = 10) {
        for (int round = 0; round < rounds; ++round) {
            for (const auto& entry : training_data) {
                // d - expected
                int expected = (entry.label == "Iris-setosa") ? 1 : 0;
                
                // y - predicted
                int predicted = compute(entry.attributes);

                // (d - y)
                int error = expected - predicted;  

                // from delta rule
                // w' = w + (d - y) * learning rate * weight
                // w' += (d - y) * learning rate * weight
                for (size_t i = 0; i < weights.size(); ++i)
                    weights[i] += learning_rate * error * entry.attributes[i];
            }
        }
    }

    double test(const std::vector<Entry>& test_data) {
        int correct = 0;
        for (const auto& entry : test_data) {
            int expected = (entry.label == "Iris-setosa") ? 1 : 0;
            int predicted = compute(entry.attributes);
            if(expected == predicted)
                ++correct;
        }
        double accuracy = 100.0 * correct / test_data.size();
        std::cout << "Correctly classified: " << correct << "/" << test_data.size() << "\n";
        std::cout << "Accuracy: " << accuracy << "%\n";
        return accuracy;
    }

    std::string classify(const std::vector<double>& input) {
        return compute(input) ? "Iris-setosa" : "Other";
    }
};

std::vector<Entry> read_file(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<Entry> data;
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        Entry entry;

        for (int i = 0; i < 4; ++i) {
            std::getline(iss, token, ',');
            entry.attributes.push_back(std::stod(token));
        }
        std::getline(iss, token);
        entry.label = token;
        data.push_back(entry);
    }
    return data;
}

int main() {
    std::vector<Entry> training_data = read_file("iris_training.txt");
    std::vector<Entry> test_data = read_file("iris_test.txt");

    Perceptron p(4, 0.1); // 4 features, learning rate = 0.1
    p.train(training_data, 10);
    p.test(test_data);

    while (true) {
        std::vector<double> input(4);
        std::cout << "Enter 4 attribute values (or 'q' to quit): ";
        std::string temp;
        std::cin >> temp;
        if (temp == "q") break;
        input[0] = std::stod(temp);
        for (int i = 1; i < 4; ++i)
            std::cin >> input[i];

        std::string result = p.classify(input);
        std::cout << "Classification result: " << result << "\n";
    }

    return 0;
}
