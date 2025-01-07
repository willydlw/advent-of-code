/*  Day 11 - Plutonian Pebbles

    part 1
        example.txt     55312 after 25 blinks
        input.txt       191690 after 25 blinks
    part 2
        input.txt       228651922369703 after 75 blinks
*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>

#include <map>

void populate_stones(std::string filename, std::map<long long, long long>& stones)
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
            stones[num]++;
        }
    }
}

// stones first is key, second is count
long long part01(std::map<long long, long long> stones, int numBlinks)
{
    for(int i = 0; i < numBlinks; i++){
        std::map<long long, long long> updatedStones;
        for(auto stone : stones){
            long long key = stone.first;
            long long count = stone.second;
            if(key == 0){
                // count number of zeros turning to ones
                updatedStones[1] += count;
            }
            else if(std::to_string(stone.first).length() % 2 == 0){
                std::string s = std::to_string(stone.first);
                size_t len = s.length();
                std::string right = s.substr(len/2, len/2);
                std::string left = s.substr(0, len/2);
                updatedStones[std::stoll(right)] += count;
                updatedStones[std::stoll(left)] += count;
            }
            else{
                updatedStones[key * 2024LL] += count;
            }
        }

        stones = updatedStones;
    }

    long long totalCount = 0;
    for(auto stone : stones){
        totalCount += stone.second;
    }

    return totalCount;
}

int main(int argc, char* argv[])
{
    std::string filename;
    int numBlinks;

    if(argc < 3){
        std::cerr << "Usage: %s filename numBlinks\n";
        return 1;
    }

    filename = argv[1];
    numBlinks = atoi(argv[2]);

    std::map<long long, long long> stones;
    populate_stones(filename, stones);

    long long total1 = part01(stones, numBlinks);
    std::cout << "total: " << total1 << " after " << numBlinks << " blinks\n";

    return 0;
}