#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <map>

std::vector<std::string> split(std::vector<std::string>& outVec1){
    std::vector<std::string> outVec2;
    for(std::string& s : outVec1){
        size_t spaceIdx = s.find(' ');
        std::string text2 = s.substr(spaceIdx + 1);
        s.erase(s.begin()+spaceIdx, s.end());
        outVec2.push_back(text2);
    }

    return outVec2;
}

int main(){
std::ifstream input_stream("data1");

if (!input_stream) std::cerr << "Can't open input file!";

std::vector<std::string> text;

std::string line;

while (getline(input_stream, line)) {
    text.push_back(line);
}

    std::vector<std::string> text2 = split(text);
    
    std::map<int, int> m{};

    for(const std::string& s : text2){
        int number = stoi(s);
        ++m[number];
    }

    int sum{};
    for(const std::string& s : text){
        int number = stoi(s);
        if(m[number] > 0){
            sum += number * m[number];
        }
    }

    std::cout<<sum;
} 
