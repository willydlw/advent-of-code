#include <fstream>
#include<iostream>
#include <iomanip>
#include <queue>
#include <string>
#include <vector>

/*  Answers
    
    Part 1
        test.txt  - Part 1 numSplits: 21
        input.txt - Part 1 numSplits: 1638
    
    Part 2
        test.txt  - Part 2 quantum splits: 40
        input.txt - Part 2 quantum splits: 7759107121385
*/


static constexpr char START = 'S';
static constexpr char SPLITTER = '^';
static constexpr char BEAM = '|';
static constexpr char EMPTY = '.';

// Inidicates path parent
static constexpr int ABOVE       =  0;  // row above, same Column
static constexpr int ABOVE_LEFT  = -1;  // row above, column to left
static constexpr int ABOVE_RIGHT = +1;  // row above, column to right

struct Location{
    int row = 0;
    int col = 0;
    Location() = default;
    Location(int r, int c) : row(r), col(c) {}
    Location(const Location& other){
        row = other.row;
        col = other.col;
    }
};

struct PathElement{
    char symbol = EMPTY;
    long long count = 0;
    bool visited = false;
    std::vector<Location> parents;
};

struct ChildElement{
    Location child;
    Location parent;
   
    ChildElement() = default;
    ChildElement(Location child, Location parent) :
    child(child), parent(parent) {}
};



void printGridSymbols(const std::vector<std::string>& grid)
{
    for(const auto& g : grid){
        std::cout << g << "\n";
    }
}

void printGridSymbols(int rows, int cols, const std::vector<std::vector<PathElement>>& pathElements)
{
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            std::cout << pathElements[r][c].symbol;
        }
        std::cout << "\n";
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


bool findStartLocation(int rows, int cols, const std::vector<std::string>& grid, Location& location)
{
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            if(grid[r][c] == START){
                location.row = r;
                location.col = c;
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
    if(grid.empty()){
        std::cerr << "[WARNING] part 1 parameter grid is empty\n";
        return 0;
    }

    long long numSplits = 0;
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());

    std::vector<std::string> workingGrid(grid);
    Location startLocation;

    if(!findStartLocation(rows, cols, workingGrid, startLocation)){
        std::cerr << "[WARNING] part 1 start location not found in grid\n";
        return 0;
    }

    
    int testRow = startLocation.row + 1;

    // checkCols contain locations of start or beams
    std::vector<int> checkCols;
    checkCols.push_back(startLocation.col);

    while(testRow < rows)
    {
        numSplits += processRow(rows, cols, testRow, workingGrid, checkCols);
        testRow++;
    }

    return numSplits;   
}

void printPathCounts(const std::vector<std::vector<PathElement>>& pathElements, int rows, int cols)
{
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            if(pathElements[r][c].count != 0){
                std::cout << std::setw(2) << pathElements[r][c].count << " ";
            }
            else{
                std::cout << " . ";
            }
        }
        std::cout << "\n";
    }
}


/*  Parameters 
        Input: 
            rows - number of grid rows 
            cols - number of columns in each grid string (same for all strings)
            children - list of children to be processed
            manifold - input character grid 

        Output:
            pathStats - Size is rows, cols. 
                        Each element contains its character symbol,
                        a count of number of paths passing through this point,
                        and the parents (in row above) that created a path through this location
*/
void markPaths(int rows, int cols, const Location& start, std::vector<std::vector<PathElement>>& pathElements)
{
    std::queue<ChildElement> unmarked;
    
    pathElements[start.row][start.col].count++;
    
    if(inbounds(rows, cols, start.row+1, start.col)){
        ChildElement ce{{start.row+1, start.col}, {start.row, start.col}};
        unmarked.push(ce);
    }

    while(!unmarked.empty()){
        ChildElement current = unmarked.front();
        unmarked.pop();

        int r = current.child.row;
        int c = current.child.col;

        if(inbounds(rows, cols, r, c)){

            switch(pathElements[r][c].symbol){
                case BEAM:
                {
                    pathElements[r][c].count++;
                    pathElements[r][c].parents.push_back(current.parent);
                    //ChildElement ce ({r+1, c}, {r, c});
                    //unmarked.push(ce);
                }
                break;
                case EMPTY:
                {
                    pathElements[r][c].symbol = BEAM;
                    pathElements[r][c].count++;
                    pathElements[r][c].parents.push_back(current.parent);
                    ChildElement ce ({r+1, c}, {r, c});
                    unmarked.push(ce);
                }
                break;
                case SPLITTER:
                {
                    ChildElement left({r, c-1}, current.parent);
                    ChildElement right({r, c+1},current.parent);
                    unmarked.push(left);
                    unmarked.push(right);        
                }
                break;
                default:
                    std::cerr << "[WARNING] func: " << __func__ << " unknown symbol: " 
                        << pathElements[r][c].symbol << " at row: " << r << ", col: "
                        << c << "\n";
            }
        }
    } 
}

void updateCounts(int rows, int cols, Location start, std::vector<std::vector<PathElement>>& pathElements)
{
    // Assumption: start will be in a row all by itself. 
    // The next path connection will be in the row below.
    for(int r = start.row+1; r < rows; r++){
        for(int c = 0; c < cols; c++){
            int numParents = pathElements[r][c].count;
            if(numParents == 1){
                Location parent = pathElements[r][c].parents.front();
                pathElements[r][c].count = pathElements[parent.row][parent.col].count;
            }
            else if(numParents > 1){
                long long sum = 0;
                for(auto p : pathElements[r][c].parents){
                    sum += pathElements[p.row][p.col].count;
                }
                pathElements[r][c].count = sum;
            }
        }
    }
}


// Assumes each string is same length
long long part02(const std::vector<std::string>& grid)
{
    long long sum = 0;
    if(grid.empty()){
        return 0;
    }

    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());

    // Declare and intialize PathElement grid
    std::vector<std::vector<PathElement>> pathElements(rows, std::vector<PathElement>(cols));
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            pathElements[r][c].symbol = grid[r][c];
            pathElements[r][c].count = 0;               // TODO: redundant, should be default initialized to zero
        }
    }

    Location startLocation;
    if(!findStartLocation(rows, cols, grid, startLocation)){
        std::cerr << "[FATAL] part 1 start location not found in grid\n";
        std::exit(EXIT_FAILURE);
    }
    
    markPaths(rows, cols, startLocation, pathElements);
    updateCounts(rows, cols, startLocation, pathElements);

    //printGridSymbols(rows, cols, pathElements);
    //printPathCounts(pathElements, rows, cols);

    int lastRow = rows-1;

    for(int c = 0; c < cols; c++){
        sum += pathElements[lastRow][c].count;
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