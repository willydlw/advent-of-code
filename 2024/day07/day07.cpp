// Day 7: Bridge Repair

/*  Part 1
        example.txt     3749
        input.txt       1611660863222
    
    Part 2
        example.txt     11387
        input.txt       
*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>

void print_equations(std::vector<std::vector<long>>& equations)
{
    for(auto row : equations){
        for(auto val : row){
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
}

void populate_equations(std::string& fileName, std::vector<std::vector<long>>& equations)
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
        std::vector<long>  row;
        long val;
        while(ss >> val){
            row.push_back(val);
        }
        equations.push_back(row);
    }
    inFile.close();
}

bool check_calculation(long result, const std::vector<long>& operands, long sum1, long sum2, size_t startIndex, size_t numElements)
{
    if(sum1 == result || sum2 == result){
        return true;
    }

    if(startIndex >= numElements){
        return false;
    }

    if(sum1 > result && sum2 > result){
        return false;
    }

    
    sum1 = sum1 + operands[startIndex];
    sum2 = sum2 * operands[startIndex];

    ++startIndex;

    return  (check_calculation(result, operands, sum1, sum1, startIndex, numElements)) || 
            (check_calculation(result, operands, sum1, sum2, startIndex, numElements)) ||
            (check_calculation(result, operands, sum2, sum1, startIndex, numElements)) ||
            (check_calculation(result, operands, sum2, sum1, startIndex, numElements));

}

bool check_calculation2(long result, const std::vector<long>& operands, long sum1, long sum2, long sum3,
            size_t startIndex, size_t numElements)
{
    if(sum1 == result || sum2 == result || sum3 == result){
        return true;
    }

    if(startIndex >= numElements){
        return false;
    }

    if(sum1 > result && sum2 > result && sum3 > result){
        return false;
    }

    
    sum1 = sum1 + operands[startIndex];
    sum2 = sum2 * operands[startIndex];
    
    // concatenation
    // find number of digits
    long num = operands[startIndex];
    int numDigits = 0;
    do{
        num /= 10L;
        numDigits++;
    } while(num != 0);

    // multiply sum3 by 10^numDigits
    int i = 0;
    while(i < numDigits){
        sum3 *= 10;
        i++;
    }

    sum3 += operands[startIndex];
    
    ++startIndex;

    return  (check_calculation2(result, operands, sum1, sum1, sum1, startIndex, numElements)) || 
            (check_calculation2(result, operands, sum1, sum1, sum2, startIndex, numElements)) ||
            (check_calculation2(result, operands, sum1, sum1, sum3, startIndex, numElements)) ||

            (check_calculation2(result, operands, sum1, sum2, sum1, startIndex, numElements)) ||
            (check_calculation2(result, operands, sum1, sum2, sum2, startIndex, numElements)) || 
            (check_calculation2(result, operands, sum1, sum2, sum3, startIndex, numElements)) ||

            (check_calculation2(result, operands, sum1, sum3, sum1, startIndex, numElements)) ||
            (check_calculation2(result, operands, sum1, sum3, sum2, startIndex, numElements)) ||
            (check_calculation2(result, operands, sum1, sum3, sum3, startIndex, numElements)) ||

            (check_calculation2(result, operands, sum2, sum1, sum1, startIndex, numElements)) || 
            (check_calculation2(result, operands, sum2, sum1, sum2, startIndex, numElements)) ||
            (check_calculation2(result, operands, sum2, sum1, sum3, startIndex, numElements)) ||

            (check_calculation2(result, operands, sum2, sum2, sum1, startIndex, numElements)) ||
            (check_calculation2(result, operands, sum2, sum2, sum2, startIndex, numElements)) || 
            (check_calculation2(result, operands, sum2, sum2, sum3, startIndex, numElements)) ||

            (check_calculation2(result, operands, sum2, sum3, sum1, startIndex, numElements)) ||
            (check_calculation2(result, operands, sum2, sum3, sum2, startIndex, numElements)) ||
            (check_calculation2(result, operands, sum2, sum3, sum3, startIndex, numElements)) ||

            (check_calculation2(result, operands, sum3, sum1, sum1, startIndex, numElements)) || 
            (check_calculation2(result, operands, sum3, sum1, sum2, startIndex, numElements)) ||
            (check_calculation2(result, operands, sum3, sum1, sum3, startIndex, numElements)) ||

            (check_calculation2(result, operands, sum3, sum2, sum1, startIndex, numElements)) ||
            (check_calculation2(result, operands, sum3, sum2, sum2, startIndex, numElements)) || 
            (check_calculation2(result, operands, sum3, sum2, sum3, startIndex, numElements)) ||

            (check_calculation2(result, operands, sum3, sum3, sum1, startIndex, numElements)) ||
            (check_calculation2(result, operands, sum3, sum3, sum2, startIndex, numElements)) ||
            (check_calculation2(result, operands, sum3, sum3, sum3, startIndex, numElements));

}

// Each row must contain at least one number
long part01(const std::vector<std::vector<long>>& equations)
{
    long total = 0;
    for(size_t r = 0; r < equations.size(); r++){
        long result = equations[r][0];
        std::vector<long> operands(equations[r].size()-1);
        std::copy(equations[r].begin()+1, equations[r].end(), operands.begin());

        // sum1 is initially 0 because it will be added (addition identity, x + 0 = x)
        // sum2 is initially 1 because it will be multiplied (multiplication identity, x * 1 = x)
        // start at index 0 of the operands array
        if(check_calculation(result, operands, 0, 1, 0, equations[r].size())){
            total += result;
        }
    }

    return total;
}

long part02(const std::vector<std::vector<long>>& equations)
{
    long total = 0;
    for(size_t r = 0; r < equations.size(); r++){
        std::cout << "processing equation " << r << "\n";
        long result = equations[r][0];
        std::vector<long> operands(equations[r].size()-1);
        std::copy(equations[r].begin()+1, equations[r].end(), operands.begin());

        // sum1 is initially 0 because it will be added (addition identity, x + 0 = x)
        // sum2 is initially 1 because it will be multiplied (multiplication identity, x * 1 = x)
        // sum3 is initially 0 becuase it will be used for concatenation
        // start at index 0 of the operands array
        if(check_calculation2(result, operands, 0, 1, 0, 0, equations[r].size())){
            total += result;
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

    std::vector<std::vector<long>> equations;

    populate_equations(fileName, equations);
    //print_equations(equations);

    //long total1 = part01(equations);
    //std::cout << "part 1: " << total1 << "\n";

    long total2 = part02(equations);
    std::cout << "\npart2: " << total2 << "\n";


    return 0;
}