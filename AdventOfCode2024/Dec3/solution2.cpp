#include <iostream>
#include <fstream>
#include <string>
#include <regex>

std::string get_data(const std::string& file_name) {
    std::ifstream file(file_name);
    if (!file) {
        std::cerr << "Error: Unable to open file " << file_name << "\n";
        return "";
    }

    std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return data;
}

int main() {
    int sum = 0;
    bool mul_enabled = true; // At the beginning, mul instructions are enabled

    std::string data = get_data("input.txt");

    // Combined regex pattern for mul(), do(), and don't()
    std::regex pattern(R"(mul\((\d{1,3}),(\d{1,3})\)|do\(\)|don't\(\))");
    std::smatch matches;
    std::string::const_iterator searchStart = data.cbegin();

    while (std::regex_search(searchStart, data.cend(), matches, pattern)) {
        std::string match_str = matches[0];
        if (match_str == "do()") {
            mul_enabled = true;
        } else if (match_str == "don't()") {
            mul_enabled = false;
        } else {
            // It's a mul() instruction
            if (mul_enabled) {
                int n1 = std::stoi(matches[1]);
                int n2 = std::stoi(matches[2]);
                if (n1 < 1000 && n2 < 1000) {
                    sum += n1 * n2;
                }
            }
        }
        searchStart = matches.suffix().first;
    }

    std::cout << sum << std::endl;

    return 0;
}
