/*  --- Day 5: Cafeteria ---

    As the forklifts break through the wall, the Elves are delighted to discover 
    that there was a cafeteria on the other side after all.

    You can hear a commotion coming from the kitchen. "At this rate, we won't 
    have any time left to put the wreaths up in the dining hall!" Resolute in your 
    quest, you investigate.

    "If only we hadn't switched to the new inventory management system right 
    before Christmas!" another Elf exclaims. You ask what's going on.

    The Elves in the kitchen explain the situation: because of their complicated 
    new inventory management system, they can't figure out which of their ingredients 
    are fresh and which are spoiled. When you ask how it works, they give you a copy 
    of their database (your puzzle input).

    The database operates on ingredient IDs. It consists of a list of fresh ingredient 
    ID ranges, a blank line, and a list of available ingredient IDs.

    For example:

    3-5
    10-14
    16-20
    12-18

    1
    5
    8
    11
    17
    32

    The fresh ID ranges are inclusive: the range 3-5 means that ingredient IDs 3, 4, and 5 are 
    all fresh. The ranges can also overlap; an ingredient ID is fresh if it is in any range.

    The Elves are trying to determine which of the available ingredient IDs are fresh. In this 
    example, this is done as follows:

    Ingredient ID 1 is spoiled because it does not fall into any range.
    Ingredient ID 5 is fresh because it falls into range 3-5.
    Ingredient ID 8 is spoiled.
    Ingredient ID 11 is fresh because it falls into range 10-14.
    Ingredient ID 17 is fresh because it falls into range 16-20 as well as range 12-18.
    Ingredient ID 32 is spoiled.
    So, in this example, 3 of the available ingredient IDs are fresh.

    Process the database file from the new inventory management system. How many of the available 
    ingredient IDs are fresh?

*/

/*  Answers 

    Part 1
        test.txt  - Part 1 fresh ingredient count: 3
        input.txt - Part 1 fresh ingredient count: 840 

    Part 2 
        test.txt  - Part 2 count: 14
        input.txt - Part 2 count: 359913027576322
*/

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

void printRanges(std::vector<std::pair<long long, long long>>& idRanges)
{
    for(const auto& r : idRanges){
        std::cerr << r.first << " - " << r.second << "\n";
    }
}

bool loadInput(std::string filename, std::vector<std::pair<long long, long long>>& idRanges,
                std::vector<long long>& indgredients)
{
    std::ifstream infile(filename);
    if(!infile.is_open()){
        std::cerr << "[ERROR] opening file: " << filename << "\n";
        return false;
    }

    std::string line;
    while(std::getline(infile, line)){
        if(line.empty()){
            break;
        }

        std::stringstream ss(line);
        long long low, high;
        char c;
        if(!(ss>>low) || !(ss >> c) || !(ss >> high)){
            std::cerr << "[ERROR] extracting low, -, high from line: " << line << "\n";
            return false;
        }

        idRanges.emplace_back(low, high);
    }

    // read ingredients
    long long val;
    while(infile >> val){
        indgredients.emplace_back(val);
    }

    // above loop may have terminated due to EOF or failure to read 
    if(infile.bad() && !infile.eof()){
        std::cerr << "[WARNING] may not have read all ingredients.\n";
    }

    infile.close();
    return true;
}

void part01(const std::vector<std::pair<long long, long long>>& idRanges, const std::vector<long long>& ingredients)
{
    long long freshCount = 0;

    for(const auto& ingredient : ingredients){
        for(const auto& range : idRanges){
            if(range.first <= ingredient && ingredient <= range.second){
                freshCount++;
                break;
            }
        }
    }
    
    std::cerr << "Part 1 fresh ingredient count: " << freshCount << "\n";
}

// Total unique ids
void part02(std::vector<std::pair<long long, long long>>& idRanges)
{
    long long count = 0;
    std::sort(idRanges.begin(), idRanges.end());
    printRanges(idRanges);

    std::pair<long long, long long> prevRange{-1, 0};

    for(const auto& range : idRanges)
    {
        if(prevRange.first == -1){
            prevRange.first = range.first;
            prevRange.second = range.second;
        }
        else if(prevRange.second < range.first)
        {
            // high value of previous is less than low value of this range
            // no overlap
            count += prevRange.second - prevRange.first + 1;
            prevRange.first = range.first;
            prevRange.second = range.second;
        }
        else 
        {   // ranges overlap, merge them
            // prevRange.first <= range.first because values are sorted
            // prevRange.first = prevRange.first
            // find greater of high values 
            prevRange.second = std::max(prevRange.second, range.second);
        }
    }

    count += prevRange.second - prevRange.first + 1;

    std::cerr << "Part 2 count: " << count << "\n";
}

int main(int argc, char* argv[])
{
    std::string filename("test.txt");
    if(argc == 2){
        filename = argv[1];
    }

    std::vector<std::pair<long long, long long>> idRanges;
    std::vector<long long> ingredients;

    if(!loadInput(filename, idRanges, ingredients)){
        std::cerr << "[FATAL] loadInput failed\n";
        return 1;
    }


    std::cerr << "idRanges.size() " << idRanges.size() << ", ingredients.size() " << ingredients.size() << "\n";
    //printRanges(idRanges);
    //part01(idRanges, ingredients);
    part02(idRanges);

    return 0;
}