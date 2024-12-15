/*  Part 1
        example.txt 18
        input.txt   2483

    Part 2
        example.txt 9
        input.txt   1925
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

void print_puzzle(std::vector<std::vector<char>>& puzzle)
{
    for(auto r : puzzle){
        for(auto c : r){
            std::cout << c;
        }

        std::cout << "\n";
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

/* Find two MAS in shape of x

    M       S           r-1,c-1     r-1,c+1
        A                       r,c
    M       S           r+1,c-1     r+1,c+1

    S       M           r-1,c-1     r-1,c+1
        A                       r,c
    S       M           r+1,c-1     r+1,c+1

    S       S
        A
    M       M

    M       M
        A
    S       S


    Strategy: Find letter 'A' and check neighbors

*/
int part02(std::vector<std::vector<char>>& puzzle)
{
    int count = 0;

    size_t numRows = puzzle.size();
    size_t numCols = puzzle[0].size();

    /* searching for the letter 'A'
        If 'A' is found in row 0, the last row,
        col 0, or the last column, the pattern
        does not exist. Thus, no need to search
        those rows and columns for an 'A'
    */
    for(size_t r = 1; r < numRows-1; r++){
        for(size_t c = 1; c < numCols-1; c++){
            if(puzzle[r][c] == 'A')
            {            
                size_t rminus = r-1;
                size_t rplus = r+1;
                size_t cminus  = c-1;
                size_t cplus = c+1;
                if( inbounds(rminus, cminus, numRows, numCols) && 
                    inbounds(rminus,cplus, numRows, numCols) && 
                    inbounds(rplus, cminus, numRows, numCols) && 
                    inbounds(rplus, cplus, numRows, numCols))
                {
                    if(puzzle[rminus][cminus] == puzzle[rplus][cminus] && 
                        puzzle[rminus][cplus] == puzzle[rplus][cplus])
                    {
                        /* c-1 column characters are same 
                        c+1 column characters are same

                            M       S           r-1,c-1     r-1,c+1
                                A                       r,c
                            M       S           r+1,c-1     r+1,c+1

                        */
                        if(puzzle[rminus][cminus] == 'M' && puzzle[rminus][cplus] == 'S')
                        {
                            count++;
                        }

                        /*
                            S       M           r-1,c-1     r-1,c+1
                                A                       r,c
                            S       M           r+1,c-1     r+1,c+1
                        */
                        else if(puzzle[rminus][cminus] == 'S' && puzzle[rminus][cplus] == 'M')
                        {
                            count++;
                        }
                    }
                    else if(puzzle[rminus][cminus] == puzzle[rminus][cplus] &&
                            puzzle[rplus][cminus] == puzzle[rplus][cplus])
                    {
                        /*  row-1 characters are same
                            row+1 chaaracter are same

                            S       S       r-1,c-1     r-1,c+1
                                A                   r,c
                            M       M       r+1,c-1     r+1,c+1

                            M       M       r-1,c-1     r-1,c+1
                                A                   r,c
                            S       S       r+1,c-1     r+1,c+1
                        */

                        if(puzzle[rminus][cminus] == 'S' && puzzle[rplus][cminus] == 'M'){
                                count++;
                        }
                        else if(puzzle[rminus][cminus] == 'M' && puzzle[rplus][cminus] == 'S'){
                                count++;
                        }

                    } 
                }
            }
        }
    }

    //print_puzzle(debugPuzzle);  // visual debug confirmation of init state

    return count;
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
    int xmasCount2 = part02(puzzle);

    std::cout << "part 1: " << xmasCount1 << "\n";
    std::cout << "part 2: " << xmasCount2 << "\n";

    return 0;
}