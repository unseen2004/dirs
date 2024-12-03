#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <cctype>

std::string get_data(const std::string& file_name) {
    std::ifstream file(file_name);
    if (!file) {
        std::cerr << "Error: Unable to open file " << file_name << "\n";
        return "";
    }

    std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return data;
}

bool add_mul(std::string_view sv, int& sum, int& out_idx) {
    size_t pos = sv.find("mul(");
    if (pos == std::string_view::npos) {
        return false;
    }

    int i = static_cast<int>(pos) + 4;
    int n1, n2;
    std::string sn1{}, sn2{};

    while (i < sv.size()) {
        sn1 = "";
        sn2 = "";

        while (i < sv.size() && std::isdigit(sv[i])) {
            sn1 += sv[i];
            ++i;
        }
        if (sn1.size() > 3 || sn1.empty())
            break;
        if (i >= sv.size() - 3)
            return false;
        if (sv[i] != ',')
            break;

        ++i;

        while (i < sv.size() && std::isdigit(sv[i])) {
            sn2 += sv[i];
            ++i;
        }
        if (sn2.size() > 3 || sn2.empty())
            break;
        if (sv[i] != ')')
            break;

        n1 = std::stoi(sn1);
        n2 = std::stoi(sn2);
        sum = sum + (n1 * n2);
        out_idx = i + 1;
        return true;
    }

    out_idx = static_cast<int>(pos) + 4;
    return false;
}
int main(){
    int sum{};

    std::string data = get_data("input2.txt");
    std::string_view datav = data;
    int idx{};
    while(add_mul(datav, sum, idx)){
        
        datav = datav.substr(idx);
    }
    
    std::cout<<sum;

}




