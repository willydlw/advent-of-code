#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <unordered_map>


/* Answers
        Part 1:
            example.txt     11
            input.txt       1388114

        Part 2:
            example.txt     31
            input.txt       23529853
*/


void populate_lists(std::string fileName, std::vector<int>& left, std::vector<int>& right)
{
    std::ifstream infile(fileName);
    if(!infile.is_open()){
        std::cerr << "Error opening file: " << fileName << '\n';
        return;
    }

    int val1, val2;
    while(infile >> val1 >> val2){
        left.push_back(val1);
        right.push_back(val2);
    }

    infile.close();
}


// assumes left and right are same size
void print_lists(std::vector<int>& left, std::vector<int>& right)
{
    for(size_t i = 0; i < left.size(); i++){
        std::cout << std::setw(10) << left[i] << std::setw(10) << right[i] << '\n';
    }
    std::cout << "\n";
}

// find distance between left and right
// assumes left, right are same size
// assumes left, right are sorted in ascending order
long part01(std::vector<int>& left, std::vector<int>& right)
{
    int total = 0;
    for(size_t i = 0; i < left.size(); i++){
        total += static_cast<long> (abs(left[i] - right[i]));
    }
    return total;
}

// find similarity score
long part02(std::vector<int>&left, std::vector<int>&right)
{
    // frequency count of right
    std::unordered_map<int, int> frequency;
    for(auto r : right){
        frequency[r]++;
    }

    long total = 0;
    for(auto l : left){
        int score = l * frequency[l];
        total += score;
    }

    return total;
}

int main(int argc, char* argv[])
{
    std::string fileName{"example.txt"};

    if(argc == 2){
        fileName = argv[1];
    }

    std::vector<int> left;
    std::vector<int> right;

    populate_lists(fileName, left, right);
    //print_lists(left, right);

    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());

    long distance = part01(left, right);
    long score = part02(left, right);

    std::cout << "part 1: " << distance << '\n';
    std::cout << "part 2: " << score << '\n';
    return 0;
}