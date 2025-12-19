#include <fstream>
#include<iostream>
#include<string>
#include <vector>
#include <iomanip>

/*  Answers
    
    Part 1
        test.txt  - Part 1 numSplits: 21
        input.txt - Part 1 numSplits: 1638
    
    Part 2
        test.txt  -
        input.txt - 
*/


static constexpr char START = 'S';
static constexpr char SPLITTER = '^';
static constexpr char BEAM = '|';
static constexpr char EMPTY = '.';


void displayGrid(const std::vector<std::string>& grid)
{
    for(const auto& g : grid){
        std::cout << g << "\n";
    }
}

bool loadInput(const std::string& filename, std::vector<std::string>& grid)
{
    std::ifstream infile(filename);
    if(!infile.is_open()){
        std::cerr << "[ERROR] opening file: " << filename << "\n";
        return false;
    }

    std::string line;
    while(std::getline(infile, line)){
        grid.push_back(line);
    }

    infile.close();
    return true;
}


// Assumption: grid not empty
bool findStartLocation(const std::vector<std::string>& grid, std::pair<int, int>& location)
{
    int totalRows = static_cast<int>(grid.size());
    int totalCols = static_cast<int>(grid[0].size());   // will crash if grid empty

    for(int r = 0; r < totalRows; r++){
        for(int c = 0; c < totalCols; c++){
            if(grid[r][c] == START){
                location.first = r;
                location.second = c;
                return true;
            }
        }
    }

    return false;
}

bool inbounds(int rows, int cols, int r, int c)
{
    return(r >= 0 && r < rows && c >= 0 && c < cols);
}

long long processRow(int totalRows, int totalCols, int testRow, std::vector<std::string>& grid, std::vector<int>& checkCols)
{
    long long splits = 0;
    std::vector<int> nextCols;

    for(auto testCol : checkCols){
        if(inbounds(totalRows, totalCols, testRow, testCol)){
            if(grid[testRow][testCol] == EMPTY){
                grid[testRow][testCol] = BEAM;
                nextCols.push_back(testCol);
            }
            else if(grid[testRow][testCol] == SPLITTER){
                splits++;
                // assuming columns to left and right of splitter are empty
                if(inbounds(totalRows, totalCols, testRow, testCol-1)){
                    grid[testRow][testCol-1] = BEAM;
                    nextCols.push_back(testCol-1);
                }

                if(inbounds(totalRows, totalCols, testRow, testCol+1)){
                    grid[testRow][testCol+1] = BEAM;
                    nextCols.push_back(testCol+1);
                }
            }
            else if(grid[testRow][testCol] == BEAM){
                continue;
            }
            else
            {
                std::cerr << "[UNEXPECTED CHARACTER] row: " << testRow << ", col: " << testCol 
                        << ", character: " << grid[testRow][testCol] << "\n"; 
            }
        }
    }

    checkCols = nextCols;
    return splits;
}


// Assumption: all strings are the same length
long long part01(const std::vector<std::string>& grid)
{
    long long numSplits = 0;
    std::vector<std::string> workingGrid(grid);
    std::pair<int, int> startLocation;

    if(grid.empty()){
        std::cerr << "[WARNING] part 1 parameter grid is empty\n";
        return 0;
    }

    if(!findStartLocation(workingGrid, startLocation)){
        std::cerr << "[WARNING] part 1 start location not found in grid\n";
        return 0;
    }

    int totalRows = static_cast<int>(grid.size());
    int totalCols = static_cast<int>(grid[0].size());
    int testRow = startLocation.first + 1;

    // checkCols contain locations of start or beams
    std::vector<int> checkCols;
    checkCols.push_back(startLocation.second);

    while(testRow < totalRows)
    {
        numSplits += processRow(totalRows, totalCols, testRow, workingGrid, checkCols);
        #if 0
        std::cout << "\nGrid State\n";
        displayGrid(workingGrid);
        std::cerr << "\nPress enter key to continue...";
        std::cin.get();
        #endif
        testRow++;
    }

    return numSplits;   
}

void printGrid(const std::vector<std::vector<int>>& grid, int rows, int cols)
{
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            std::cout << std::setw(2) << grid[r][c] << " ";
        }
        std::cout << "\n";
    }
}


bool inBetweenSplitters(int rows, int cols, int r, int c, const std::vector<std::string>& grid)
{
    if(inbounds(rows, cols, r, c-1) && inbounds(rows, cols, r, c+1))
    {
        if(grid[r][c-1] == SPLITTER && grid[r][c+1] == SPLITTER){
            return true;
        }
    }

    return false;
}

void countPaths(int totalRows, int totalCols, const std::vector<std::string>& grid, 
                std::vector<std::vector<int>>& pathCount)
{
    for(int r = 0; r < totalRows; r++){
        for(int c = 0; c < totalCols; c++){
            if(grid[r][c] == BEAM){
                if(inBetweenSplitters(totalRows, totalCols, r, c, grid)){
                    std::cout << "merging paths between two splitters in this row\n";
                    pathCount[r][c] = pathCount[r-1][c-1] + pathCount[r-1][c+1] + pathCount[r-1][c];
                }
                else{
                    #if 0
                    std::cout << "not between two splitters in this row\n";
                    std::cout << "row above, same col:  " << grid[r-1][c] << "\n";
                    std::cout << "row above, col left:  " << grid[r-1][c-1] << "\n";
                    std::cout << "row above, col right: " << grid[r-1][c+1] << "\n";
                    std::cout << "Press enter to continue...";
                    std::cin.get();
                    #endif

                    // Check row above same column: Can be a beam, start, or empty 
                    // Cannot have a splitter in the same column, row above
                    if(grid[r-1][c] == BEAM || grid[r-1][c] == START){
                        //std::cout << "BEAM or START directly above: " << grid[r-1][c] << "\n";
                        pathCount[r][c] = pathCount[r-1][c];
                    }
                    else{
                        //std::cout << "Checking if left or right neighbor in this row is a splitter\n";
                        if(inbounds(totalRows, totalCols, r, c-1) && grid[r][c-1] == SPLITTER){
                            //std::cout << "left neighbor is splitter\n";
                            pathCount[r][c] = pathCount[r-1][c-1];
                        }
                        else if(inbounds(totalRows, totalCols, r, c+1) && grid[r][c+1] == SPLITTER){
                            //std::cout << "right neighbor is splitter\n";
                            pathCount[r][c] = pathCount[r-1][c+1];
                        }
                        else{
                            std::cout << "[WARNING] case not considered, r: " << r << ", c: "
                            << c << " grid[r][c]: " << grid[r][c] << " is a BEAM? how did we get here?\n";
                        }
                    }
                    
                }
            }
        }
    }
    printGrid(pathCount, totalRows, totalCols);
    std::cout << "Press enter key to continue...";
    std::cin.get();
}

// Assumes each string is same length
long long part02(const std::vector<std::string>& grid)
{
    long long sum = 0;
    if(grid.empty()){
        return 0;
    }

    int totalRows = static_cast<int>(grid.size());
    int totalCols = static_cast<int>(grid[0].size());

    std::vector<std::string> workingGrid(grid);

    std::pair<int, int> startLocation;
    if(!findStartLocation(workingGrid, startLocation)){
        std::cerr << "[WARNING] part 1 start location not found in grid\n";
        return 0;
    }
   
    int testRow = startLocation.first + 1;
    std::vector<int> checkCols;                 // checkCols contain locations of start or beams
    checkCols.push_back(startLocation.second);

    while(testRow < totalRows)
    {
        processRow(totalRows, totalCols, testRow, workingGrid, checkCols);
        testRow++;
    }


    #if 1
    std::cout << "\nGrid State\n";
    displayGrid(workingGrid);
    std::cerr << "\nPress enter key to continue...";
    std::cin.get();
    #endif

    // zero-initialized 2d grid 
    std::vector<std::vector<int>> pathCount(totalRows, std::vector<int>(totalCols, 0));
    pathCount[startLocation.first][startLocation.second] += 1;

    countPaths(totalRows, totalCols, workingGrid, pathCount);

    std::cout << "\nCounts\n";
    printGrid(pathCount, totalRows, totalCols);

    // Sum counts in last row
    for(int c = 0; c < totalCols; c++){
        sum += pathCount[totalRows-1][c];
    }
    return sum;
}

int main(int argc, char* argv[])
{
    std::string filename("test.txt");
    if(argc == 2){
        filename = argv[1];
    }

    std::vector<std::string> grid;
    if(!loadInput(filename, grid)){
        std::cerr << "[FATAL] terminating due to input failure\n";
        return 1;
    }

    //long long numSplits = part01(grid);
    //std::cerr << "Part 1 numSplits: " << numSplits << "\n";

    long long quantumSplits = part02(grid);
    std::cerr << "Part 2 quantum splits: " << quantumSplits << "\n";
    return 0;
}