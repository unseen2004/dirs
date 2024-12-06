#include <iostream>
#include <vector>
#include <string>
#include <fstream>

void get_data(std::vector<std::string>& out_v, const std::string& file_name, int* out_start_loc) {
    std::ifstream file(file_name);
    std::string str;
    int line_index = 0;
    bool flag{false};
    while (std::getline(file, str)) {
        out_v.push_back(str);
        if(!flag){
             int i{};
            for(const char& c: str){
                if(c=='^'){
                    *out_start_loc = line_index;
                    *(out_start_loc+1) = i;
                    flag = true;
                    break;
                }
                ++i;
            }
        }
        ++line_index;
     }
}

void main_loop1(){
     std::vector<std::string> input;
    int pos[2] = {0, 0};
    get_data(input, "input.txt", pos);
    int diff{1};
    input[pos[0]][pos[1]] = 'X';
    std::cout<<"sd";
    char arrow = '^';
    while(true){
        std::cout<<arrow<<" "<<pos[0]<<" "<<pos[1]<<"\n";
        if(arrow == '>'){
            if(pos[1] == input[0].size()-1){
                break;
            }else if(input[pos[0]][pos[1]+1] == '#'){
                arrow = '∨';
            }else{
                if(input[pos[0]][pos[1]+1] != 'X'){
                    input[pos[0]][pos[1]+1] = 'X';
                    ++diff;
                }
                ++pos[1];
            }

        }else if(arrow == '<'){
             if(pos[1] == 0){
                break;
            }else if(input[pos[0]][pos[1]-1] == '#'){
                arrow = '^';
            }else{
                if(input[pos[0]][pos[1]-1] != 'X'){
                    input[pos[0]][pos[1]-1] = 'X';
                    ++diff;
                }
                --pos[1];
            }

        }else if(arrow == '^'){

            if(pos[0] == 0){
                break;
            }else if(input[pos[0]-1][pos[1]] == '#'){
                arrow = '>';
            }else{
                if(input[pos[0]-1][pos[1]] != 'X'){
                    input[pos[0]-1][pos[1]] = 'X';
                    ++diff;
                }
                --pos[0];
                std::cout<<pos[0];
            }
        }else{
            if(pos[0] == input.size()-1){
                break;
            }else if(input[pos[0]+1][pos[1]] == '#'){
                arrow = '<';
            }else{
                if(input[pos[0]+1][pos[1]] != 'X'){
                    input[pos[0]+1][pos[1]] = 'X';
                    ++diff;
                }
                ++pos[0];
            }

        }
    }

    std::cout << diff << "\n";
    
}

#include <set>
#include <tuple>

// ...

bool is_in_loop(char arrow, int pos_0, int pos_1, int to0, int to1, const std::vector<std::string>& input) {
    // Make a copy of the input to modify
    auto map = input;
    // Place the new obstruction
    map[pos_0 + to0][pos_1 + to1] = '#';

    // Starting position and arrow
    int pos[2] = {pos_0, pos_1};
    char current_arrow = arrow;

    // Set to store visited states
    std::set<std::tuple<int, int, char>> visited;
    // Add the initial state
    visited.insert({pos[0], pos[1], current_arrow});

    while (true) {
        // Movement logic
        if (current_arrow == '>') {
            if (pos[1] == map[0].size() - 1) {
                // Reached the right edge
                break;
            } else if (map[pos[0]][pos[1] + 1] == '#') {
                current_arrow = 'v'; // Turn down
            } else {
                pos[1]++;
            }
        } else if (current_arrow == '<') {
            if (pos[1] == 0) {
                // Reached the left edge
                break;
            } else if (map[pos[0]][pos[1] - 1] == '#') {
                current_arrow = '^'; // Turn up
            } else {
                pos[1]--;
            }
        } else if (current_arrow == '^') {
            if (pos[0] == 0) {
                // Reached the top edge
                break;
            } else if (map[pos[0] - 1][pos[1]] == '#') {
                current_arrow = '>'; // Turn right
            } else {
                pos[0]--;
            }
        } else if (current_arrow == 'v') {
            if (pos[0] == map.size() - 1) {
                // Reached the bottom edge
                break;
            } else if (map[pos[0] + 1][pos[1]] == '#') {
                current_arrow = '<'; // Turn left
            } else {
                pos[0]++;
            }
        }

        // Check if the new state has been visited
        auto state = std::make_tuple(pos[0], pos[1], current_arrow);
        if (visited.count(state)) {
            // The guard is in a loop
            return true;
        }

        // Add the new state to the set
        visited.insert(state);
    }

    // The guard exits the map without entering a loop
    return false;
}

void main_loop2(){
    std::vector<std::string> input;
    int pos[2] = {0, 0};
    get_data(input, "input.txt", pos);
    input[pos[0]][pos[1]] = 'X';
    char arrow = '^';
    int out2 = 0;

    // Loop over all possible positions on the map
    for (int i = 0; i < input.size(); ++i) {
        std::cout<<input.size()-1-i<<"\n";
        for (int j = 0; j < input[0].size(); ++j) {
            // Skip if it's the guard's starting position or an existing obstruction
            if ((i == pos[0] && j == pos[1]) || input[i][j] == '#')
                continue;

            // Check if placing an obstruction here causes a loop
            if (is_in_loop(arrow, pos[0], pos[1], i - pos[0], j - pos[1], input)) {
                ++out2;
            }
        }
    }

    std::cout << out2 << std::endl;
}


int main() {
    
    //main_loop1(); // ex 1
    main_loop2();   // ex 2

}

