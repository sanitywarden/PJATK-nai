#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <random>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Iris {
    vector<double> features;
    string label;
};

class NaiveBayesClassifier {
private:
    map<string, vector<double>> means;
    map<string, vector<double>> variances;
    map<string, int> class_counts;
    int total_samples = 0;

    double gaussianProb(double x, double mean, double var) {
        return (1.0 / sqrt(2 * 3.141 * var)) * exp(-pow(x - mean, 2) / (2 * var));
    }

public:
    void train(const vector<Iris>& dataset) {
        map<string, vector<vector<double>>> class_feature_values;

        for (const auto& sample : dataset) {
            class_counts[sample.label]++;
            total_samples++;

            if (class_feature_values[sample.label].empty()) {
                class_feature_values[sample.label] = vector<vector<double>>(sample.features.size());
            }

            for (size_t i = 0; i < sample.features.size(); ++i) {
                class_feature_values[sample.label][i].push_back(sample.features[i]);
            }
        }

        for (const auto& [label, features_by_column] : class_feature_values) {
            vector<double> class_means, class_vars;
            for (const auto& values : features_by_column) {
                double mean = 0, var = 0;
                for (double v : values) mean += v;
                mean /= values.size();

                for (double v : values) var += pow(v - mean, 2);
                var /= values.size();

                class_means.push_back(mean);
                class_vars.push_back(var);
            }
            means[label] = class_means;
            variances[label] = class_vars;
        }
    }

    string predict(const vector<double>& input) {
        double best_prob = -1.0;
        string best_class;

        for (const auto& [label, mean_vector] : means) {
            double log_prob = log((double)class_counts[label] / total_samples);
            for (size_t i = 0; i < input.size(); ++i) {
                double prob = gaussianProb(input[i], means[label][i], variances[label][i]);
                log_prob += log(prob + 1e-9);
            }

            if (log_prob > best_prob || best_prob == -1.0) {
                best_prob = log_prob;
                best_class = label;
            }
        }
        return best_class;
    }
};

vector<Iris> load_iris(const string& filename) {
    vector<Iris> dataset;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string value;
        vector<double> features;
        string label;
        int count = 0;
        while (getline(ss, value, ',')) {
            if (count < 4) {
                features.push_back(stod(value));
            } else {
                label = value;
            }
            count++;
        }
        dataset.push_back({features, label});
    }
    return dataset;
}

void shuffleAndSplit(const vector<Iris>& dataset, vector<Iris>& train, vector<Iris>& test, double train_ratio = 0.8) {
    vector<Iris> shuffled = dataset;
    random_device rd;
    mt19937 g(rd());
    shuffle(shuffled.begin(), shuffled.end(), g);

    size_t train_size = shuffled.size() * train_ratio;
    train.assign(shuffled.begin(), shuffled.begin() + train_size);
    test.assign(shuffled.begin() + train_size, shuffled.end());
}

void evaluate(NaiveBayesClassifier& clf, const vector<Iris>& test_set) {
    map<string, map<string, int>> confusion;
    int correct = 0;

    for (const auto& sample : test_set) {
        string predicted = clf.predict(sample.features);
        if (predicted == sample.label) correct++;
        confusion[sample.label][predicted]++;
    }

    double accuracy = (double)correct / test_set.size();
    cout << fixed << setprecision(2);
    cout << "\nAccuracy: " << accuracy * 100 << "%" << endl;

    // Confusion matrix
    cout << "\nConfusion Matrix:\n";
    vector<string> classes = {"Iris-setosa", "Iris-versicolor", "Iris-virginica"};
    cout << setw(16) << "Predicted";
    for (const string& c : classes) cout << setw(16) << c;
    cout << endl;

    for (const string& actual : classes) {
        cout << setw(16) << actual;
        for (const string& predicted : classes) {
            cout << setw(16) << confusion[actual][predicted];
        }
        cout << endl;
    }
}

int main() {
    vector<Iris> dataset = load_iris("iris_training.txt");

    vector<Iris> train_set, test_set;
    shuffleAndSplit(dataset, train_set, test_set);

    NaiveBayesClassifier classifier;
    classifier.train(train_set);

    evaluate(classifier, test_set);

    while(true) {
        vector<double> user_input(4);
        cout << "Input data: ";
        cin >> user_input[0] >> user_input[1] >> user_input[2] >> user_input[3];
        
        string prediction = classifier.predict(user_input);
        cout << "\nThe predicted Iris class is: " << prediction << endl;
    }

    return 0;
}
