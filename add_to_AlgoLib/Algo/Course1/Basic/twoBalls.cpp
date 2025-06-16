#include <vector>
#include <cmath>

int whenBreaks(const std::vector<bool> v){
    int jump = sqrt(v.size());
    int i{jump};
    for(; i<v.size(); i += jump){
        if(v[i]) break;
    }
    i -= jump;
    
    for(int j{}; j<jump && i < v.size(); ++j,++i){
        if(v[i]){
            return i;
        }
    }
    return -1;
}
