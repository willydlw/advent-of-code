// Day 7: Bridge Repair

/*  Part 1
        example.txt     3749
        input.txt       1611660863222
    
    Part 2
        example.txt     11387
        input.txt       945341732469724
*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

void print_equations(std::vector<std::vector<long long>>& equations)
{
    for(auto row : equations){
        for(auto val : row){
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
}

void populate_equations(std::string& fileName, std::vector<std::vector<long long>>& equations)
{
    std::ifstream inFile(fileName);
    if(!inFile.is_open()){
        std::cerr << "Error opening file: " << fileName << "\n";
        exit(EXIT_FAILURE);
    }

    std::string line;
    while(std::getline(inFile, line)){
        size_t pos = line.find(':');
        if(pos != std::string::npos){
            line.erase(pos, 1);
        }
        std::stringstream ss(line);
        std::vector<long long>  row;
        long long val;
        while(ss >> val){
            row.push_back(val);
        }
        equations.push_back(row);
    }
    inFile.close();
}

void check_calculation(long result, const std::vector<long long>& operands, long long sum, size_t index, bool& matchesTarget)
{
    if(index == operands.size()){
        if(sum == result){
            matchesTarget = true;
        }

        return;
    }
    
    check_calculation(result, operands, sum + operands[index], index + 1, matchesTarget);
    check_calculation(result, operands, sum * operands[index], index + 1, matchesTarget);
}


void check_calculation2(long result, const std::vector<long long>& operands, long long sum, size_t index, bool& matchesTarget)
{
    if(index == operands.size()){
        if(sum == result){
            matchesTarget = true;
        }

        return;
    }
    
    check_calculation2(result, operands, sum + operands[index], index + 1, matchesTarget);
    check_calculation2(result, operands, sum * operands[index], index + 1, matchesTarget);
    // concatenate values
    check_calculation2(result, operands, stoll(std::to_string(sum) + std::to_string((operands[index]))), 
                index + 1, matchesTarget);
}


// Each row must contain at least one number
long part01(const std::vector<std::vector<long long>>& equations)
{
    long total = 0;
    for(size_t r = 0; r < equations.size(); r++){
        bool matchesResult = false;
        check_calculation(equations[r][0], equations[r], equations[r][1], 2, matchesResult);
        if(matchesResult){
            total += equations[r][0];
        }
    }

    return total;
}


long part02(const std::vector<std::vector<long long>>& equations)
{
    long long total = 0;
    for(size_t r = 0; r < equations.size(); r++){
        bool matchesResult = false;
        check_calculation2(equations[r][0], equations[r], equations[r][1], 2, matchesResult);
        if(matchesResult){
            total += equations[r][0];
        }
    }

    return total;
}



int main(int argc, char* argv[])
{
    std::string fileName = "example.txt";
    if(argc == 2){
        fileName = argv[1];
    }

    std::vector<std::vector<long long>> equations;

    populate_equations(fileName, equations);
    //print_equations(equations);

    long long total1 = part01(equations);
    std::cout << "part 1: " << total1 << "\n";

    long long total2 = part02(equations);
    std::cout << "part2: " << total2 << "\n";


    return 0;
}