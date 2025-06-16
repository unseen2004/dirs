#include <vector>
#include <algorithm>
#include <iostream>

void bubbleSort(std::vector<int>& v){
    for(int i{}; i<v.size()-1; ++i){
        for(int j{}; j<v.size()-i-1; ++j){
            if(v[j] < v[j+1])
                std::swap(v[j], v[j+1]);
        }
    }
}

int main(){
    std::vector<int> v {1,23,213,12,4234,25,3536,5645645,-1};
    bubbleSort(v);
    for(int i: v){
        std::cout<<i<<"\n";
    }
    return 0;
}
