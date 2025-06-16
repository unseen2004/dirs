#include <iostream>
#include <vector>


int partition(std::vector<int>& arr, int lo, int hi){

    const int pivot = arr[hi];

    int idx = lo - 1;

    for(int i{lo}; i<hi; ++i){
        if(arr[i] <= pivot){
            ++idx;
            const int tmp = arr[i];
            arr[i] = arr[idx];
            arr[idx] = tmp;
        }

    }


        ++idx;
        arr[hi] = arr[idx];
        arr[idx] = pivot;

        return idx;

}




void qs(std::vector<int>& arr, int lo, int hi){
    if(lo>=hi){
        return;
    }

    const int pivotIdx = partition(arr, lo, hi);

    qs(arr, lo, pivotIdx - 1);
    qs(arr, pivotIdx + 1, hi);

}
int main(){
    std::vector<int> arr = {5,4,3,2,3,4,5};
    qs(arr, 0, arr.size()-1);
    for(const int& i: arr){
        std::cout<<i<<" ";
    }
}

