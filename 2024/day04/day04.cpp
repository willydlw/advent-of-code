/*  Part 1
        example.txt 18
        input.txt

    
*/



#include <fstream>
#include <iostream>
#include <vector>


void populate_puzzle(std::string fileName, std::vector<std::vector<char>>& puzzle)
{
    std::ifstream inFile(fileName);
    if(!inFile.is_open()){
        std::cerr << "Error opening file: " << fileName << "\n";
        return;
    }

    std::string line;
    while(std::getline(inFile, line)){
        std::vector<char> row;
        for(auto c : line){
            row.push_back(c);
        }
        puzzle.push_back(row);
    }
}


void remove_letters(std::vector<std::vector<char>>&puzzle)
{
    for(size_t r = 0; r < puzzle.size(); r++)
    {
        for(size_t c = 0; c < puzzle[r].size(); c++){
            switch(puzzle[r][c]){
                case 'X':
                break;
                case 'M':
                break;
                case 'A':
                break;
                case 'S':
                break;
                default:
                    std::cerr << "Unexpected puzzle letter: " << puzzle[r][c] 
                        << ", at row: " << r << ", col: " << c << "\n";
            }
        }
    }
}


void print_puzzle(std::vector<std::vector<char>>& puzzle)
{
    for(auto r : puzzle){
        for(auto c : r){
            std::cout << c;
        }

        std::cout << "\n";
    }
}

int main(int argc, char* argv[])
{
    std::string fileName{"example.txt"};
    if(argc == 2){
        fileName = argv[1];
    }

    std::vector<std::vector<char>> puzzle;
    populate_puzzle(fileName, puzzle);

    std::cout << "\nPuzzle Input\n";
    print_puzzle(puzzle);

    return 0;
}