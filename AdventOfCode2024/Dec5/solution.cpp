
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <algorithm>

void get_data(std::map<int, std::vector<int>>& out_map, std::vector<std::vector<int>>& v_lines_out, const std::string& file_name) {
    std::ifstream file(file_name);
    std::string str;
    bool parsing_map = true;
    int line_index = 0;

    while (std::getline(file, str)) {
        if (str.empty()) {
            parsing_map = false;
            continue;
        }

        if (parsing_map) {
            int n1 = std::stoi(str.substr(0, 2));
            int n2 = std::stoi(str.substr(3, 2));
            out_map[n1].push_back(n2);
        } else {
            std::string tmp;
            v_lines_out.emplace_back();
            for (char c : str) {
                if (c == ',') {
                    v_lines_out[line_index].push_back(std::stoi(tmp));
                    tmp.clear();
                } else {
                    tmp += c;
                }
            }
            if (!tmp.empty()) {
                v_lines_out[line_index].push_back(std::stoi(tmp));
            }
            ++line_index;
        }
    }
}

int main() {
    std::map<int, std::vector<int>> input1;
    std::vector<std::vector<int>> input2;
    get_data(input1, input2, "input.txt");
    int sum{};
    for(int j{}; j < input2.size(); ++j){
        int flag {true};
        for(int i{}; i<input2[j].size()-1; ++i){
            int spr = input2[j][i];
            for(int z{i+1}; z< input2[j].size(); ++z){
                if(input1.count(input2[j][z]) > 0){
                    if (std::find(input1[input2[j][z]].begin(), input1[input2[j][z]].end(), spr) != input1[input2[j][z]].end()){
                        flag = false;
                        break;
                    }
                }
            }
            if(!flag){
                break;
            }
        }
        if(flag){
            sum+= input2[j][input2[j].size()/2];
        }
    }
    std::cout<<sum<<"\n";
}

