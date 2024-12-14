/*  Part 1
        example.txt 18
        input.txt   2483

*/



#include <fstream>
#include <iostream>
#include <vector>

const std::string XMAS{"XMAS"};


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


bool inbounds(int r, int c, int numRows, int numCols){
    return (r >= 0 && r < numRows && c >= 0 && c < numCols);
}


/* Constraints:
        puzzle contains at least one row
        all rows have same number of columns
        all letters are upper case


        Word to left
        ============
        word index  3       2       1       0
                    S       A       M       X
        grid index  r,c-3   r,c-2,  r, c-1  r,c     dr = 0      dc = -1

        Word to right
        ============+
        word index  0       1       2       3
                    X       M       A       S
        grid index  r,c     r,c+1,  r, c+2  r,c+3   dr = 0      dc = -1


        Word above
        ============
                                Grid    Word
                                Index   Index       dr = -1     dc = 0
                            S   r-3,c   3           r + dr * i  c + dc * i
                            A   r-2,c   2           
                            M   r-1,c   1
                            X   r-0,c   0
        
        Word below
        ============
                                Grid    Word
                                Index   Index       dr = 1      dc = 0
                            X   r+0,c   0           r + dr * i  c + dc * i
                            M   r+1,c   1
                            A   r+2,c   2
                            S   r+3,c   3


        Word diagonal up right
        =======================

                            Grid    Word
                            Index   Index       dr = -1     dc = 1
                    S       r-3,c+3   3
                A           r-2,c+2   2
            M               r-1,c+1   1
        X                   r  ,c     0


        Word diagonal up left
        =======================

                            Grid    Word
                            Index   Index     dr = -1         dc = -1
        S                   r-3,c-3   3
            A               r-2,c-2   2
                M           r-1,c-1   1
                    X       r  ,c     0


        Word diagonal down left
        =======================

                            Grid    Word
                            Index   Index     dr = 1           dc = -1   
                    X       r  ,c     0       r + dr * i       c + dc * i
                M           r+1,c-1   1
            A               r+2,c-2   2
        S                   r+3,c-3   3

        Word diagonal down right
        =======================

                            Grid    Word
                            Index   Index       dr = 1          dc = 1
        X                   r  ,c     0         r + dr * i      c + dc * i
            M               r+1,c+1   1         r + dr * i      c + dc * i 
                A           r+2,c+2   2         r + dr * i      c + dc * i
                    S       r+3,c+3   3         r + dr * i      c + dc * i

                            
        Check all grid positions for character 'X'
        When 'X' is found, must check all neighbors
*/
int part01(std::vector<std::vector<char>>& puzzle)
{
    int wordCount = 0;
    int numRows = puzzle.size();
    int numCols = puzzle[0].size();
    int numChars = XMAS.length();

    for(int r = 0; r < numRows; r++){
        for(int c = 0; c < numCols; c++){
            if(puzzle[r][c] == 'X'){
                // neighboring rows change by -1, 0, +1
                for(int dr = -1; dr <= 1; dr++){
                    // neighbor cols are c-1, c, c+1
                    for(int dc = -1; dc <= 1; dc++){
                        if(dr == 0 && dc == 0){
                            continue;
                        }
                        bool match = true;
                        for(int i = 0; i < numChars; i++){
                            if(!inbounds(r+dr*i, c+dc*i, numRows, numCols) ||
                                puzzle[r+dr*i][c+dc*i] != XMAS[i]){
                                    match = false;
                                    break;
                            }
                        }
                        if(match){
                            wordCount++;
                        }
                    }
                }
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
    

    //std::cout << "\nPuzzle Input\n";
    //print_puzzle(puzzle);

    int xmasCount1 = part01(puzzle);

    std::cout << "part 1: " << xmasCount1 << "\n";

    return 0;
}