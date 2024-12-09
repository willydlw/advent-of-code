#include <fstream>
#include <sstream>
#include <iostream>

#include <vector>

#include <cstdlib>
#include <iomanip>
#include <cassert>


/*  Part 1 answers:
        example.txt     2
        input.txt       534

    Part 2 answers:
        example.text    4
        input.txt       577
*/

static const int LOWER_DIFFERENCE_LIMIT = 1;
static const int UPPER_DIFFERNCE_LIMIT = 3;

enum LEVEL_STATE{EQUAL, INCREASING, DECREASING};

void print_reports(std::vector<std::vector<int>>& reports)
{
    for(auto r : reports){
        for(auto c : r){
            std::cout << std::setw(5) << c << " ";
        }
        std::cout << '\n';
    }

    std::cout << "\n";
}

bool populate_reports(std::string& fileName, std::vector<std::vector<int>>& reports)
{
    std::string line;
    std::ifstream infile(fileName);

    if(!infile.is_open())
    {
        std::cerr << "Error opening file: " << fileName << '\n';
        return false;
    }

    while(std::getline(infile, line)){
        std::vector<int> row;
        std::istringstream iss(line);
        int value;
        while(iss >> value){
            row.push_back(value);
        }
        reports.push_back(row);
    }

    infile.close();
    return true;
}


LEVEL_STATE find_level_state(int diff)
{
    if(diff > 0){
        return INCREASING;
    }
    else if(diff < 0){
        return DECREASING;
    }
    else{
        return EQUAL;
    }
}

bool diff_within_limits(int diff)
{
    return(diff >= LOWER_DIFFERENCE_LIMIT  
            && diff <= UPPER_DIFFERNCE_LIMIT);
}

bool is_level_safe(std::vector<int>& level)
{
    LEVEL_STATE currentState, prevState;

    int numCols = (int) level.size();

    for(int i = 1; i < numCols; i++){

        int diff = level[i] - level[i-1];
        currentState = find_level_state(diff);

        if(diff_within_limits(abs(diff))){
            if(i != 1){
                if(currentState != prevState){
                    return false;
                }
            }
        }
        else {
            return false;
        }
      
        prevState = currentState;
    }

    return true;
}


bool is_safe_after_removal(std::vector<int>& level)
{
    for(size_t i = 0; i < level.size(); i++){
        std::vector<int> test = level;
        test.erase(test.begin() + i);
        if(is_level_safe(test)){
                return true;
        }
    }

    return false;
}


int part01_safe_reports(std::vector<std::vector<int>>& reports)
{
    int safeCount = 0;
    for(auto r : reports){
        if(is_level_safe(r)){
            safeCount++;
        }
    }

    return safeCount;
}


int part02_safe_reports(std::vector<std::vector<int>>& reports)
{
    int safeCount = 0;
    for(auto r : reports){
        if(is_level_safe(r)){
            safeCount++;
        }
        else if(is_safe_after_removal(r)){
            safeCount++;
        }
    }

    return safeCount;
}


int main(int argc, char* argv[])
{
    std::string fileName{"example.txt"};
    if(argc == 2){
        fileName = argv[1];
    }
    
    std::vector<std::vector<int>> reports;
    if(!populate_reports(fileName, reports)){
        std::cerr << "Failed to populate reports matrix\n";
        return 1;
    }

    //print_reports(reports);

    // part 01
    int safeCount1 = part01_safe_reports(reports);
    std::cout << "part 1 safe count: " << safeCount1 << "\n";

    // part 02
    int safeCount2 = part02_safe_reports(reports);
    std::cout << "part 2 safe count: " << safeCount2 << "\n";

    return 0;
}