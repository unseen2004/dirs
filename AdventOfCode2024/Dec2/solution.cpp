#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <cstdlib>

void get_data(std::vector<std::vector<int>>& out_vec, const std::string& file_name){
    std::ifstream file(file_name);
    std::string str;

    while(std::getline(file, str)){
        std::vector<int> v{};
        std::string number{};
        for(const char c : str){
            if(c == ' '){
                v.push_back(stoi(number));
                number = "";
            }else{
                number += c;
            }
        }
        v.push_back(stoi(number));

        out_vec.push_back(v);
    }
}

bool is_unsafe(const std::vector<int>& v){
     int flag {-1}; // 0 - decreasing, 1 - increasing, -1 - not set
    
    for(int i{}; i<v.size()-1; ++i){
            if(flag == -1 && v[i] != v[i+1] && abs(v[i] - v[i+1]) <= 3){
                if(v[i] < v[i+1]){
                    flag = 1;
                }else{
                    flag = 0;
                }
            }
            else if(v[i] == v[i+1] || v[i] < v[i+1] && flag == 0 || v[i] > v[i+1] && flag == 1 || abs(v[i] - v[i+1]) > 3){
                return true;
            }       
    }

    return false;
}

int how_many_safe(const std::vector<std::vector<int>>& input){
    int how_many_unsafe {};
    for(auto v : input){
        if(is_unsafe(v))
            ++how_many_unsafe;
    }

    return input.size() - how_many_unsafe;
}




int how_many_safe2(const std::vector<std::vector<int>>& input){
    int safe_count = 0;

    for(const auto& v : input) {
        if(!is_unsafe(v)) {
            ++safe_count;
            continue;
        }

        bool dampened_safe = false;

        for(size_t i = 0; i < v.size(); ++i) {
            std::vector<int> modified = v;
            modified.erase(modified.begin() + i); 

            if(!is_unsafe(modified)) {
                dampened_safe = true;
                break; 
            }
        }

        if(dampened_safe) {
            ++safe_count;
        }
    }

    return safe_count;
}



int main(){
    std::vector<std::vector<int>> input{};
    get_data(input, "input.txt");

    std::cout<<how_many_safe(input);
    std::cout<<"\n"<<how_many_safe2(input);
 
}
