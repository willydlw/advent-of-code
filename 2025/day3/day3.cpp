// Lobby 

#include <fstream>
#include <iostream>
#include <string>
#include <vector>


/*  Answers

    Part 1
        test.txt  - Total Joltage: 357
        input.txt - Total Joltage: 16854

    Part 2
        test.txt  - Part 2 total joltage: 3121910778619
        input.txt - Part 2 total joltage: 167526011932478

*/


bool loadInput(const std::string& fileName, std::vector<std::string>& batteryBanks)
{
    std::ifstream infile(fileName);
    if(!infile.is_open())
    {
        std::cerr << "[ERROR] opening file: " << fileName << "\n";
        return false;
    }

    std::string line;
    while(std::getline(infile, line))
    {
        batteryBanks.push_back(line);
    }

    infile.close();
    return true;
}


void part01(const std::vector<std::string>& batteryBanks)
{
    long totalJoltage = 0;
   
    for(const auto& bank : batteryBanks)
    {
        // search for leftmost max digit
        char tensDigit = '0';
        int tensIndex = -1;
        char onesDigit = '0';
        
        // find largest leftmost digit that is not the last digit in the string
        for(size_t i = 0; i < bank.size()-1; i++)
        {
            if(bank[i] > tensDigit)
            { 
                tensDigit = bank[i];
                tensIndex = (int)i;
            }
        }

        // search for largest digit to right of tensDigit
        for(size_t i = tensIndex + 1; i < bank.size(); i++)
        {
            if(bank[i] > onesDigit)
            { 
                onesDigit = bank[i];
            }
        }

        totalJoltage += (tensDigit - '0') * 10 + (onesDigit - '0');
    }

    std::cerr << "Part 1 Total Joltage: " << totalJoltage << "\n";
}


long long findMaxDigit(const std::string& bank, const size_t& startIndex, const size_t& stopIndex, size_t& selectionIndex)
{
    if(bank.size() < startIndex){
        std::cerr << "[FATAL] bank.size(): " << bank.size() << " < startIndex: " << startIndex << "\n";
        std::exit(-1);
    }

    char maxDigit = bank[startIndex];
    selectionIndex = startIndex;

    for(size_t i = startIndex + 1; i < stopIndex; i++)
    {
        if(bank[i] > maxDigit)
        {
            maxDigit = bank[i];
            selectionIndex = i;
        }
    }

    return {maxDigit - '0'};
}


void part02(const std::vector<std::string>& batteryBanks)
{
    long long totalJoltage = 0;

    for(const auto& bank :batteryBanks)
    {
        size_t remainingDigits = 12;
        size_t numDigits = bank.size();
        size_t startIndex  = 0;
        size_t stopIndex = numDigits - remainingDigits + 1;
        size_t selectionIndex = startIndex;


        long long joltage = 0;
        while(remainingDigits > 0)
        {
            joltage = joltage * 10 + findMaxDigit(bank, startIndex, stopIndex, selectionIndex);
            startIndex = selectionIndex + 1;
            remainingDigits--;
            stopIndex = numDigits - remainingDigits + 1;
        }

        totalJoltage += joltage;
    }

    std::cerr << "Part 2 Total Joltage: " << totalJoltage << "\n";
}


int main(int argc, char* argv[])
{
    std::string fileName = "test.txt";

    if(argc == 2)
    {
        fileName = argv[1];
    }
    
    std::vector<std::string> batteryBanks;
    if(!loadInput(fileName, batteryBanks))
    {
        std::cerr << "[FATAL] failed to load input\n";
        return 1;
    }

    part01(batteryBanks);
    part02(batteryBanks);
    return 0;
}