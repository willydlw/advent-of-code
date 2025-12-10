#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>

#include <cmath>


bool loadInput(const std::string& fileName, std::vector<std::pair<int, int>> &ranges)
{
    std::ifstream inFile(fileName);
    if(!inFile.is_open()){
        std::cerr << "[FATAL] failed to open file: " << fileName << "\n";
        return false;
    }

    std::string line;
    while(std::getline(inFile, line, ',')){
       //std::cerr << "line: " << line << "\n";
       std::stringstream ss(line);
       int low, high;
       char hyphen;

       if(!(ss >> low) || !(ss >> hyphen) || !(ss >> high)){
        std::cerr << "[ERROR] extracting low, hyphen, high from line: " << line << "\n";
        inFile.close();
        return false;
       }

       ranges.push_back({low, high});
    }

    inFile.close();
    return true;
}


bool checkTwiceRepeated(int val)
{
    std::string strVal = std::to_string(val);
    size_t len = strVal.length();

    if(len % 2){
        std::cerr << "[LOGIC ERROR], strval: " << strVal << ", strVal.length(): " << len << "\n";
        return false;
    }

    size_t midPoint = len / 2;
    std::string left = strVal.substr(0, midPoint);
    std::string right = strVal.substr(midPoint);
    
    
      
    if(left == right){
        std::cerr << "repeating pattern detected, val: " << val << ", left: " << left << ", right: " << right << "\n"; 
        return true;
    }

    return false;
}


int getLesserExponent(int number)
{
    if(number == 0){
        // log10(0) is undefined
        return 1;
    }

    double logVal = std::log10(number);

    std::cerr << "logval: " << logVal << "\n";
    return static_cast<int>(std::floor(logVal));
}

/*
    Assumptions:

        The std::pair first value <= second value.

    Problem Statement:
        - None of the numbers have leading zeroes; 0101 isn't an ID at all.
          Does not indicate if there may be an id 0.
*/
void part1(const std::vector<std::pair<int, int>> &idRanges)
{
    int invalidIDCount = 0;
    for(const auto& r : idRanges){
        std::cerr << "range first: " << r.first << ", second: " <<  r.second << "\n";

        int lesserExponent = 0;
        int greaterExponent = 0;
        lesserExponent = getLesserExponent(r.first);

        // Duplicate patterns only found in numbers with even number of digits 
        // lesserExponent must be odd. 
        //  Examples: 33 is duplicate, log10(33) is 1
        //            333 is not considered invalid, log10(333) is 2 
        //            6565 is invalid, log10(6565) is 3
        if(lesserExponent & 0x1 == 0)
        {
            ++lesserExponent;
        }

        // Lesser exponent is an odd number
        // Odd number defined as 2n + 1
        // lesserExponent = 2n + 1 
        // n = (lesserExponent - 1) / 2
        int n = (lesserExponent - 1) / 2;

        // n is used to find first possible value that is invalid 
        // Examples:
        //   lesserExponent = 1 
        //   n = (1-1)/ 2 = 0
        //   10^1 + 10^0 = 10 + 1 = 11

        //   lesserExponent = 3 
        //   n = (3-1)/2 = 1 
        //   10^3 + 10^1 = 1000 + 10 = 1010
        int startVal = static_cast<int>(std::pow(10, lesserExponent)) + static_cast<int>(std::pow(10, n));

        //  Increment for 2 digit numbers should be 11 
        //                4 digit numbers should be 101 
        //                6 digit numbers should be 1001 
        //  When lesser exponent is 1, we have 2 digit numbers, starting at 11 and incrementing by 11 
        //  (11, 22, 33, ..., 88, 99) are all invalid IDs 

        //  When lesser exponent is 3, we have 4 digit numbers, starting at 1010 and incremeting by 101 
        //  (1010, 1111, 1212, 1313, ..., 9898, 9999) are all invalid IDs 

        //  When lesser exponent is 5, we have 6 digit numbers starting at 100100 and incrmenting 10001
        int increment = 

        std::cerr << "lesserExponent " << lesserExponent << ", startVal" << startVal<< "\n";

    }

    std::cerr << "Invalid ID Count: " << invalidIDCount << "\n";
}

#if 0
void part1(const std::vector<std::pair<int,int>> &ranges)
{
    int invalidCount = 0;
    for( const auto& r : ranges){
        std::cerr << "low: " << r.first << ", high: " << r.second << "\n";
        int lowThreshold = 10;
        int highThreshold = 100;
        int thresholdIncrement = 1;
        int increment = 1;

        // TODO: increments 11, 101, 1001, 10001, ...

        for(int val = r.first; val <= r.second;){

            while(val > lowThreshold){
                std::cerr << "\ndetected val > lowThreshold, val: " << val << ", lowThreshold: " << lowThreshold << "\n";
                lowThreshold *= 10;
                highThreshold *= 10;
                std::cerr << "updated lowThreshold: " << lowThreshold << ", highThreshold: " << highThreshold << "\n";
                std::cerr << "Press enter key to continue...";
                std::cin.get();
            }

            if(val == lowThreshold){
                std::cerr << "\ndetected val == lowThreshold, val: " << val << ", lowThreshold: " << lowThreshold << "\n";
                
                thresholdIncrement = lowThreshold / 100;
                increment = lowThreshold + 1;
                val = lowThreshold + thresholdIncrement;
                
                std::cerr << "updated val: " << val << ", thresholdIncrment: " 
                    << thresholdIncrement << ", increment: " << increment << "\n";
                std::cerr << "Press enter to continue...";
                std::cin.get();
                continue;
            }

            else if(val < lowThreshold){
                std::cerr << "\ndetected val < lowThreshold\n";
                std::cerr << "current val: " << val << ", low threshold: " << lowThreshold 
                    << ", thresholdIncrement: " << thresholdIncrement 
                    << ", increment: " << increment << "\n";
                
                val = lowThreshold + thresholdIncrement;
                increment = lowThreshold + 1;
                
                
                std::cerr << "updated val: " << val << ", low threshold: " << lowThreshold 
                    << ", thresholdIncrement: " << thresholdIncrement 
                    << ", increment: " << increment << "\n";
                std::cerr << "Press enter key to continue...";
                std::cin.get();
                continue;
            }
            else if(val >= highThreshold){
                std::cerr << "\ndetected val >= highThreshold\n";
                std::cerr << "current val: " << val << ", low threshold: " << lowThreshold 
                    << ", highThreshold: " << highThreshold 
                    << ", thresholdIncrement: " << thresholdIncrement 
                    << ", increment: " << increment << "\n";
                lowThreshold *= 100;
                highThreshold *= 100;
                thresholdIncrement = lowThreshold / 100;
                

                std::cerr << "detected val >= highThreshold\n";
                std::cerr << "updated  low threshold: " << lowThreshold 
                    << ", highThreshold: " << highThreshold << "\n";
                std::cerr << "Press enter key to continue...";
                std::cin.get();
                continue;
            }

            if(checkTwiceRepeated(val)){
                //std::cerr << "repeating pattern detected: " << val << "\n";
                invalidCount++;
            }

            val += increment;

        }
    }

    std::cerr << "Part 1 answer, invalid ID count: " << invalidCount << "\n";
}
#endif


int main(int argc, char* argv[])
{
    std::string fileName("test.txt");
    if(argc == 2){
        fileName = argv[1];
    }

    std::vector<std::pair<int,int>> ranges;
    if(!loadInput(fileName, ranges)){
        std::cerr << "Terminating due to input failure\n";
        return 1;
    }

    part1(ranges);
    return 0;
}