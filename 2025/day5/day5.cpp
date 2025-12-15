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
        input.txt - Part 1 fresh ingredient count: 808  INCORRECT, answer too low
*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

void printRanges(const std::map<long long, long long>& idRanges)
{
    for(const auto& r : idRanges){
        std::cerr << r.first << " - " << r.second << "\n";
    }
}

bool loadInput(std::string filename, std::map<long long, long long>& idRanges,
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

        idRanges[low] = high;
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

void part01(const std::map<long long, long long>& idRanges, const std::vector<long long>& ingredients)
{
    long long freshCount = 0;

    for(const auto& ingredient : ingredients){
        // Get an iterator to the first element with a key greater than ingredient
        auto it = idRanges.upper_bound(ingredient);

        //std::cerr << "Checking ingredient number: " << ingredient << "\n";

        if(it == idRanges.begin()){
            //std::cerr << "No key less than or equal to ingredient: " << ingredient 
            //    << ", not a fresh ingredient\n";
            continue;
        }

        // Decrement iterator to get to element with the greatest key <= ingredientValue
        --it;                   
        bool notFound = true;
        while(notFound){
            if(it->first <= ingredient && ingredient <= it->second){
                std::cerr << "Found ingredient: " << ingredient << " in range: " << it->first << " - " << it->second << "\n";
                freshCount++;
                notFound = false;
            }
            else{
                if(it == idRanges.begin()){
                    //std::cerr << "At start of map, ingredient: " << ingredient << " not found\n";
                    break;
                }
                else{
                    --it;
                }
            }
        }
    }

    std::cerr << "Part 1 fresh ingredient count: " << freshCount << "\n";
}

int main(int argc, char* argv[])
{
    std::string filename("test.txt");
    if(argc == 2){
        filename = argv[1];
    }

    std::map<long long, long long> idRanges;
    std::vector<long long> ingredients;

    if(!loadInput(filename, idRanges, ingredients)){
        std::cerr << "[FATAL] loadInput failed\n";
        return 1;
    }


    //std::cerr << "idRanges.size() " << idRanges.size() << ", ingredients.size() " << ingredients.size() << "\n";
    //printRanges(idRanges);
    part01(idRanges, ingredients);

    return 0;
}