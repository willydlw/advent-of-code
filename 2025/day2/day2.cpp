#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <cmath>

// g++ -Wall -Wextra -pedantic day2.cpp 

/*  Correct Answers

    Part 1:
        test.txt  - Final Invalid ID Count: 8, sum invalid IDs: 1227775554
        input.txt - Final Invalid ID Count: 609, sum invalid IDs: 41294979841

    Part 2:
        test.txt  - Final, invalid ID count: 13, sum: 4174379265
        input.txt - Final, invalid ID count: 672, sum invalid IDs: 66500947346
*/


bool loadInput(const std::string& fileName, std::vector<std::pair<long long, long long>> &ranges)
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
       long long low, high;
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


bool checkTwiceRepeated(long long val)
{
    std::string strVal = std::to_string(val);
    size_t len = strVal.length();

    if(len % 2){
        std::cerr << "[LOGIC ERROR], strval: " << strVal << ", strVal.length(): " << len << "\n";
        exit(-1);
        //return false;
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


int getLesserExponent(long long number)
{
    if(number == 0){
        // log10(0) is undefined
        return 1;
    }

    double logVal = std::log10(number);
    return static_cast<int>(std::floor(logVal));
}

void setRangeParameters(long long number, long long& lowThreshold, long long& startVal, long long& increment)
{
        long long lesserExponent = getLesserExponent(number);

        // Duplicate patterns only found in numbers with even number of digits 
        // lesserExponent must be odd. 
        //  Examples: 33 is duplicate, log10(33) is 1
        //            333 is not considered invalid, log10(333) is 2 
        //            6565 is invalid, log10(6565) is 3
        if( (lesserExponent & 0x1) == 0)
        {
            ++lesserExponent;
        }

        lowThreshold = static_cast<long long>(std::pow(10, lesserExponent));

        // Lesser exponent is an odd number
        // Odd number defined as 2n + 1
        // lesserExponent = 2n + 1 
        // n = (lesserExponent - 1) / 2
        long long n = (lesserExponent - 1) / 2;

        // n is used to find first possible value that is invalid 
        // Examples:
        //   lesserExponent = 1 
        //   n = (1-1)/ 2 = 0
        //   10^1 + 10^0 = 10 + 1 = 11

        //   lesserExponent = 3 
        //   n = (3-1)/2 = 1 
        //   10^3 + 10^1 = 1000 + 10 = 1010
        startVal = static_cast<long long>(std::pow(10, lesserExponent)) + static_cast<long long>(std::pow(10, n));

        //  Increment for 2 digit numbers should be 11 
        //                4 digit numbers should be 101 
        //                6 digit numbers should be 1001 
        //  When lesser exponent is 1, we have 2 digit numbers, starting at 11 and incrementing by 11 
        //  (11, 22, 33, ..., 88, 99) are all invalid IDs 

        //  When lesser exponent is 3, we have 4 digit numbers, starting at 1010 and incremeting by 101 
        //  (1010, 1111, 1212, 1313, ..., 9898, 9999) are all invalid IDs 

        //  When lesser exponent is 5, we have 6 digit numbers starting at 100100 and incrmenting 10001
        increment = static_cast<long long>(std::pow(10, (lesserExponent+1)/2)) + 1;

        // Example: number is 47, startval will be 11, but 11 not part of the valid range
        // Increment until the startVal >= number
        if(startVal < number){
            long long diff = number - startVal;
            long long multiplier = diff/increment + ((diff % increment)? 1 : 0);
            startVal += multiplier * increment;
        }
}

/*
    Assumptions:

        The std::pair first value <= second value.

    Problem Statement:
        - None of the numbers have leading zeroes; 0101 isn't an ID at all.
          Does not indicate if there may be an id 0.
*/
void part1(const std::vector<std::pair<long long, long long>> &idRanges)
{
    long long invalidIDCount = 0;
    long long sumInvalidIDs = 0;
    
    for(const auto& r : idRanges){

        long long lowThreshold, highThreshold;      
        long long increment;
        long long startVal;            

        std::cerr << "range first: " << r.first << ", second: " <<  r.second << "\n";

        setRangeParameters(r.first, lowThreshold, startVal, increment);
        highThreshold = lowThreshold * 10;

        long long current = startVal;

        while(current <= r.second){
            if(current >= highThreshold){
                setRangeParameters(current, lowThreshold, startVal, increment);
                highThreshold = lowThreshold * 10;
                current = startVal;
                continue;
            }

            // TODO: This is not necessary if all startvalues and increments have been 
            // correctly calculated. At this point we could just skip the check and increment
            // the invalid id count if my other logic is correct. Will leave it in for now 
            // to verify the patterns are repeated.
            if(checkTwiceRepeated(current)){
                invalidIDCount++;
                sumInvalidIDs += current;
            }

            current += increment;
        }
    }

    std::cerr << "Partt 1, Invalid ID Count: " << invalidIDCount 
                    << ", sum invalid IDs: " << sumInvalidIDs << "\n";
}

bool checkPart2NumberPattern(const std::string& str)
{
    size_t len = str.length();
    if(len < 2){
        return false;
    }

    // len-1 to avoid comparing the entire string to itself
    for(size_t i = 0; i < len-1; i++){

        // can the string be broken up into blocks of the same size?
        if(len % (i+1) == 0){           // i + 1 because string index starts at 0
            size_t blockSize = i + 1;

            //std::cerr << "\nFor str: " << str << ", checking blocks of size: " << blockSize << "\n";

            // Extract the pattern we want to check
            std::string extract = str.substr(0, blockSize);
            //std::cerr << "Does this extracted pattern repeat: " << extract << "\n";

            // Create the pattern
            size_t numBlocks = len / (i+1);
            std::string pattern = extract;

            for(size_t j = 1; j < numBlocks; j++){
                pattern.append(extract);
            }

            // compare repeated pattern to original string 
            if(pattern == str){
                return true;
            }
        }
    }

    return false;
}

// Brute force
// Odd length strings can now have repeating patterns: 111 is invalid
// Can have multiple repeats such as 353535 or 656656656
void part2(std::vector<std::pair<long long, long long>> idRanges)
{
    long long invalidIDCount = 0;
    long long sumInvalidIDs = 0;
    
    for(const auto& r : idRanges){
        std::cerr << "Checking range r.first: " << r.first << "-" << r.second << "\n";
        long long current = r.first;
        while(current <= r.second){
            if(checkPart2NumberPattern(std::to_string(current))){
                invalidIDCount++;
                sumInvalidIDs += current;
            }

            current++;
        }
    }

    std::cerr << "Part 2, invalid ID count: " << invalidIDCount << ", sum invalid IDs: " << sumInvalidIDs << "\n";
}


int main(int argc, char* argv[])
{
    std::string fileName = "test.txt";

    if(argc == 2){
        fileName = argv[1];
    }

    std::vector<std::pair<long long, long long>> ranges;
    if(!loadInput(fileName, ranges)){
        std::cerr << "Terminating due to input failure\n";
        return 1;
    }

    part1(ranges);
    part2(ranges);
    return 0;
}