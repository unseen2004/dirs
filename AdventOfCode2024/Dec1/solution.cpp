#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cmath>

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
    std::sort(text2.begin(), text2.end());
    std::sort(text.begin(), text.end());
        
    int dif {};
    for(int i{}; i<text.size(); ++i){
       dif+= abs(stoi(text[i]) - stoi(text2[i]));
        std::cout<<dif<<"\n";
    }
    
  std::cout<<dif;

}
