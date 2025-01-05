/*  Day 11 - Plutonian Pebbles

    part 1
        example.txt
        input.txt
    part 2
        example.txt
        input.txt
*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>

#include <vector>

void populate_stones(std::string filename, std::vector<long long>& stones)
{
    std::ifstream infile(filename);
    if(!infile.is_open()){
        std::cerr << "Error opening file: " << filename << "\n";
        exit(EXIT_FAILURE);
    }

    std::string line;
    while(std::getline(infile, line)){
        std::stringstream ss(line);
        long long num;
        while(ss >> num){
            stones.push_back(num);
        }
    }
}

// passing copy of vector stones so it can be altered locally;
size_t part01(std::vector<long long> stones, int numBlinks)
{
    for(int i = 0; i < numBlinks; i++){

    }

    return stones.size();
}

int main(int argc, char* argv[])
{
    std::string filename("example.txt");
    int numBlinks = 1;

    if(argc == 3){
        filename = argv[1];
        numBlinks = atoi(argv[2]);
    }

    std::vector<long long> stones;

    size_t total1 = part01(stones, numBlinks);
    std::cout << "part 1: " << total1 << " after " << numBlinks << " blinks\n";

    return 0;
}