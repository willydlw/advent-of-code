/*  Part 1
        example.txt 18
        input.txt

    
*/



#include <fstream>
#include <iostream>
#include <vector>
#include <stack>

const std::string TARGET_WORD{"XMAS"};


struct Location{
    int row;
    int col;
    Location(): row(0), col(0) {}
    Location(int r, int c): row(r), col(c) {}
};

struct Word{
    std::string letters;                // store target letters as they are found
    Location gridLocations[4];          // store locations of target letters found
    Location currentLocation;           // search neighbors of this location to find
                                        // next letter
    int numFound;                       // number of target letters found
    char nextLetter;                    // next letter to find in word search
};


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

void add_border_to_puzzle(std::vector<std::vector<char>>& puzzle)
{
    int numRows = puzzle.size() + 2;
    int numCols = puzzle[0].size() + 2;
    std::vector<std::vector<char>> puzzCopy(numRows, std::vector<char>(numCols, '.'));
    for(int i = 1; i < numRows-1; i++){
        for(int j = 1; j < numCols-1; j++){
            puzzCopy[i][j] = puzzle[i-1][j-1];
        }
    }

    puzzle = puzzCopy;

}


std::vector<Location> checkNeighbors(const std::vector<std::vector<char>>& puzzle,
        const Word* searchWord)
{
    std::vector<Location> locations;
    int r = searchWord->currentLocation.row;
    int c = searchWord->currentLocation.col;

    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if(puzzle[r+i][c+i] == searchWord->nextLetter){
                locations.push_back((Location){r+i,c+i});
            }
        }
    }

    return locations;
}

void find_words(std::vector<std::vector<char>>& puzzle, 
                Word* startWord, std::vector<Word>& wordsFound)
{
    std::stack<Word> searchStack;
    searchStack.push(*startWord);

    while(!searchStack.empty()){

        Word searchWord = searchStack.top();
        searchStack.pop();

        std::vector<Location> neighbors = checkNeighbors(puzzle, &searchWord);

        for(auto n : neighbors){
            Word temp = searchWord;

            // add the letter found to the word
            temp.letters.append(1, temp.nextLetter);

            // add location of letter found
            temp.gridLocations[temp.numFound] = n;

            temp.currentLocation = n;

            temp.numFound += 1;

            if(temp.numFound < (int)TARGET_WORD.length()){
                temp.nextLetter = TARGET_WORD[temp.numFound];
                searchStack.push(temp);
            }
            else {
                // found target word
                wordsFound.push_back(temp);
            }
        }
    }
}


/* Constraints:
        puzzle contains at least one row
        all rows have same number of columns
        all letters are upper case
*/


int part01(std::vector<std::vector<char>>& puzzle)
{
    int wordCount = 0;
    int numRows = puzzle.size();
    int numCols = puzzle[0].size();
    std::vector<Word> words;

    for(int r = 0; r < numRows; r++){
        for(int c = 0; c < numCols; c++){
            if(puzzle[r][c] == 'X'){
                Word word;
                word.letters = "X";
                word.currentLocation = (Location){r,c};
                word.gridLocations[0] = (Location){r,c};
                word.numFound = 1;
                word.nextLetter = 'M';
                
                std::vector<Word> wordsFound;
                find_words(puzzle, &word, wordsFound);
                wordCount += wordsFound.size();

                std::cout << "Words found for X at r: " << r << ", c: "
                    << c << ", count: " << wordsFound.size() << "\n";
            }
        }
    }

    return wordCount;
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

    std::cout << "\nPuzzle with Border\n";
    add_border_to_puzzle(puzzle);
    print_puzzle(puzzle);


    int xmasCount1 = part01(puzzle);

    std::cout << "part 1: " << xmasCount1 << "\n";

    return 0;
}