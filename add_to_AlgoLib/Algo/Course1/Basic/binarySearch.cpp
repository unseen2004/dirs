#include <vector>
template<typename T>
bool binarySearch(const std::vector<T>& v, T target){
    int l=0,r=v.size()-1, m;
    while(l=<r){
        m = (l+(r-l)/2);
        if(v[m] == target)  return true;
        else if(v[m] > target){
            r = m-1;
        }
        else{
            l = m + 1;
        }
    }
    return false;
}
