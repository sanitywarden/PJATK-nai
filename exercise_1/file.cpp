#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <cmath>
#include <climits>
#include <iomanip>

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

            auto next_bar = line.find('|', i);
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

int main(int argc, char *argv[]) {
    std::cout << "input K\n";

    int k = 1;
    std::cin >> k;

    std::cout << "input file (format: <name>.txt)\n";
    std::string filename;
    std::cin >> filename;

    std::cout << "input parameters (use '.' or ',' for floating point). input character 'x' to end input\n";
    Entry entry_to_test;

    while(true) {
        std::string input;
        std::cin >> input;
        if(input.find('x') != std::string::npos)
            break;

        if(input.find(',') != std::string::npos)
            input[input.find(',')] = '.';

        entry_to_test.attributes.push_back(std::stod(input.c_str()));
    }

    std::fstream file(filename.c_str(), std::ios::in);
    std::string line;
    std::vector<Entry> entries;

    while(std::getline(file, line) ) {
        auto entry = entry_from_line(line);
        entries.push_back(entry);
    }
    file.close();

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

    std::cout << "results for: file=" << filename << " K=" << k << " data=";;
    for(auto input : entry_to_test.attributes) std::cout << input << " ";
    std::cout << "\n";

    for(int i = 0; i < k; i++) {
        auto decision = entries[i];
        std::cout << "decision K=" << i + 1 << " -> " << decision.decision << "\n"; 
    }
}