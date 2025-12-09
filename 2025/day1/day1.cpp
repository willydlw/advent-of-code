// Day 1: Secret Entrance



#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>      // std::pair

#define NUM_DIAL_POSITIONS 100
#define DIAL_START_POSITION 50


bool readInput(const std::string& filename, std::vector<std::pair<char,int>>& rotations)
{
    std::string line;
    std::ifstream infile(filename, std::ios::in);
    
    if(!infile.is_open()){
        std::cerr << "[ERROR]: could not open input file: " << filename << "\n";
        return false;
    }

    while(std::getline(infile, line)){
        char direction;
        int  distance;
        std::stringstream os(line);

        if(!(os >> direction) || !(os >> distance)){
            std::cerr << "Error extracting direction and/or distance from line: " << line << "\n";
            return false;
        }

        rotations.push_back(std::pair<char, int>{direction, distance});
    }

    infile.close();
    return true; 
}


// Part 1 answers
//  test.txt    3
//  input.txt   1150

// Count number of times dial position is at zero after completing a rotation
void part1(const std::vector<std::pair<char, int>>& rotations)
{
    int dialPosition = DIAL_START_POSITION;
    int zeroCount = 0;

    for(auto [direction, distance] : rotations)
    {
        if(direction == 'L'){
            dialPosition = (dialPosition - distance) % NUM_DIAL_POSITIONS;
        }
        else if(direction == 'R'){
            dialPosition = (dialPosition + distance) % NUM_DIAL_POSITIONS;
        }
        else{
            std::cerr << "Error unknown dial direction: " << direction << "\n";
        }

        if(dialPosition == 0){
            zeroCount++;
        }

        //std::cerr << "direction: " << direction << ", distance: " << distance << ", dial position: " << dialPosition << "\n";
    }


    std::cerr << "Part 1, password is zero count: " << zeroCount << "\n";    
}

/* Part 2 answers 
    text.txt        6
    input.txt       6738

    Count the number of times any click causes the dial to point at 0, 
    regardless of whether it happens during a rotation or at the end of one.


    Method 0x43 4c 49 43 4b
             C   L  I C  K

*/
void part2(const std::vector<std::pair<char, int>>& rotations)
{
   
    int dialPosition = DIAL_START_POSITION;
    int zeroCount = 0;

  
    for(auto [direction, distance] : rotations)
    {
        if(distance >= NUM_DIAL_POSITIONS){
            zeroCount += distance / NUM_DIAL_POSITIONS;
            distance %= NUM_DIAL_POSITIONS;
        }

        int nextDialPosition;

        if(direction == 'L'){
            nextDialPosition = dialPosition - distance;
            if(nextDialPosition < 0){
                if(dialPosition > 0){
                    zeroCount++;
                }
                nextDialPosition = NUM_DIAL_POSITIONS + nextDialPosition;
            }

        }
        else if(direction == 'R'){
            nextDialPosition = dialPosition + distance;
            if(nextDialPosition > NUM_DIAL_POSITIONS){
               zeroCount++;
            }
            nextDialPosition %= NUM_DIAL_POSITIONS;
        }
        else{
            std::cerr << "Error unknown dial direction: " << direction << "\n";
        }

        dialPosition = nextDialPosition;
        if(dialPosition == 0){
            zeroCount++;
        }
    }

    std::cerr << "Part 2, password is zero count: " << zeroCount << "\n";

}

int main(int argc, char* argv[])
{
    std::string fileName("test.txt");

    if(argc == 2){
        fileName = argv[1];
    }

    std::vector<std::pair<char, int>> rotations;

    if(!readInput(fileName, rotations)){
        std::cerr << "[FATAL] terminating to readInput failure\n";
    }

    part1(rotations);
    part2(rotations);
    return 0;
}