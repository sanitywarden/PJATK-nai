#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <map>
#include <iostream>

struct Entry {
    std::vector<double> attributes;
    std::string decision;
};

Entry entry_from_line(std::string line) {
    std::replace_if(line.begin(), line.end(), [](char c){ return c == '\t'; }, '|');
    
    char previous = ' ';
    Entry entry;

    for(int i = 0; i < line.length() - 1; i++) {
        if(previous == '|' || i == 0) {
            if(i - 1 == line.find_last_of('|')) {
                entry.decision = line.substr(i, line.size() % i - 1);
                break;
            }

            int next_bar = line.find('|', i);
            auto length = abs(i - next_bar);
            auto value_str = line.substr(i, length);
            
            if(value_str.find(',') != std::string::npos)
                value_str[value_str.find(',')] = '.';
            
            auto value = std::stod(value_str.c_str());

            entry.attributes.push_back(value);
        }
        
        previous = line[i];
    }

    return entry;
}

std::vector<Entry> entries_from_file(std::string filename) {
    std::fstream file(filename.c_str(), std::ios::in);
    std::string line;
    std::vector<Entry> entries;

    while(std::getline(file, line) ) {
        auto entry = entry_from_line(line);
        entries.push_back(entry);
    }
    file.close();
    
    return entries;
}

void sort_entries(std::vector<Entry>& entries, Entry entry_to_test) {
    std::sort(entries.begin(), entries.end(), [entry_to_test](Entry a, Entry b) {
        double sum_a = 0;
        for(int i = 0; i < a.attributes.size(); i++)
            sum_a += std::pow(a.attributes[i] - entry_to_test.attributes[i], 2);
        sum_a = std::sqrt(sum_a);

        double sum_b = 0;
        for(int i = 0; i < b.attributes.size(); i++)
            sum_b += std::pow(b.attributes[i] - entry_to_test.attributes[i], 2);
        sum_b = std::sqrt(sum_b);

        return sum_a < sum_b;
    });
}

std::vector<Entry> entries_from_file_sorted(std::string filename, Entry entry) {
    auto entries = entries_from_file(filename);
    sort_entries(entries, entry);
    return entries;
}

bool is_correct(Entry to_test, std::vector <Entry>& decision_from_test, int k) {
    auto training = entries_from_file("iris_training.txt");
    sort_entries(training, to_test);

    std::map <std::string, int> counts_1;
    std::map <std::string, int> counts_2;

    for(int i = 0; i < k; i++) {
        std::cout << decision_from_test[i].decision << " - " << training[i].decision << "\n";
        counts_1[decision_from_test[i].decision]++;        
        counts_2[training[i].decision]++;        
    }

    std::pair <std::string, int> choice_1 = { "", 0 };
    std::pair <std::string, int> choice_2 = { "", 0 };

    for(int i = 0; i < k; i++) {
        if(counts_1[decision_from_test[i].decision] > choice_1.second) {
            choice_1.first = decision_from_test[i].decision;
            choice_1.second = counts_1[decision_from_test[i].decision];
        }

        if(counts_2[training[i].decision] > choice_2.second) {
            choice_2.first = training[i].decision;
            choice_2.second = counts_2[training[i].decision];
        }
    }

    return choice_1.first == choice_2.first;
} 