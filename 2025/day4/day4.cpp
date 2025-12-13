#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>


/*  Answers
    Part 1:
        test.txt  - Part 1 accessible paper rolls: 13
        input.txt - Part 1 accessible paper rolls: 1491

    Part 2:
        test.txt  - Part 2 accessible paper rolls: 43
        input.txt - Part 2 accessible paper rolls: 8722
*/

static constexpr char PAPER = '@';

static constexpr int NEIGHBOR_LIMIT = 4;


void printGrid(const std::vector<std::string>& grid)
{
    for(auto row : grid)
    {
        std::cerr << row << "\n";
    }
}


bool loadInput(const std::string& fileName, std::vector<std::string> &grid)
{
    std::string line;

    std::ifstream infile(fileName);
    if(!infile.is_open()){
        std::cerr << "[ERROR] opening file: " << fileName << "\n";
        return false;
    }

    while(std::getline(infile, line)){
        grid.push_back(line);
    }

    infile.close();
    return true;
}

bool inBounds(int r, int c, int rows, int cols)
{
    return (r >= 0 && r < rows && c >= 0 && c < cols);
}

/*  Decscription:
        Counts the number of paper rolls in the eight neigboring positions 
        of grid[sourceRow][sourceCol] location.

    Assumptions:
        All rows have the same number of columns.

    Returns the count.
*/
int countPaperNeighbors(int sourceRow, int sourceCol, int rows, int cols, const std::vector<std::string>& grid)
{
    int count = 0;
   
    for(int r = sourceRow-1; r <= sourceRow + 1; r++)
    {
        for(int c = sourceCol-1; c <= sourceCol + 1; c++)
        {
            if(inBounds(r, c, rows, cols))
            {
                if(grid[r][c] == PAPER)
                {
                    count++;
                }
            }
        }
    }
    return count - 1;       // subtract 1 because source is counted [r=0][c=0]
}

/*  How many rolls of paper are accessible?
    Paper rolls are accessible if fewer than 4 neighbors are paper rolls.
*/
void part01(const std::vector<std::string>& grid)
{
    int accessibleCount = 0;
    int rows = static_cast<int>(grid.size());
    int cols;

    if(rows > 0){
        cols = grid[0].size();
    }

    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < cols; c++)
        {
            if(grid[r][c] == PAPER)
            {
                int count = countPaperNeighbors(r, c, rows, cols, grid);
                if(count < NEIGHBOR_LIMIT)
                {
                    accessibleCount++;
                }
            }
        }
    }

    std::cerr << "Part 1 accessible paper rolls: " << accessibleCount << "\n";
}

std::vector<std::pair<int, int>> getAccessibleList(const std::vector<std::string>& grid)
{
    std::vector<std::pair<int, int>> accessbileRolls;

    int rows = static_cast<int>(grid.size());
    int cols;

    if(rows > 0){
        cols = grid[0].size();
    }

    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < cols; c++)
        {
            if(grid[r][c] == PAPER)
            {
                int count = countPaperNeighbors(r, c, rows, cols, grid);
                if(count < NEIGHBOR_LIMIT)
                {
                    accessbileRolls.push_back({r,c});
                }
            }
        }
    }

    return accessbileRolls;
}


/*  Part 2

    How many paper rolls are accessible if we keep removing accessible rolls until
    no more are accessible or all rolls are removed?
*/

void part02(const std::vector<std::string>& grid)
{
    size_t totalRemoved = 0;
    size_t currentCount = 0;

    // Since there isn't a part 3, we could remove the const qualifier
    // from the grid parameter and overwrite, but I'm choosing to create 
    // a copy of the original
    std::vector<std::string> gridCopy(grid);

    #if 0
    std::cerr << "Original Grid\n";
    printGrid(gridCopy);
    std::cerr << "\n";
    #endif

    do{
        std::vector<std::pair<int,int>> accessibleList = getAccessibleList(gridCopy);
        currentCount = accessibleList.size();
        totalRemoved += currentCount;

        // mark accessible rolls as removed
        for(const auto& a : accessibleList)
        {
            gridCopy[a.first][a.second] = 'x';
        }

        #if 0
        std::cerr << "Removed " << currentCount << " rolls of paper\n";
        std::cerr << "Updated Grid\n";
        printGrid(gridCopy);
        std::cerr << "\nPress enter key to continue...";
        std::cin.get();
        #endif

    }while(currentCount != 0);
    
    std::cerr << "Part 2 accessible paper rolls: " << totalRemoved << "\n"; 
}


int main(int argc, char* argv[])
{
    std::vector<std::string> grid;
    std::string fileName{"test.txt"};

    if(argc == 2){
        fileName = argv[1];
    }

    if(!loadInput(fileName, grid)){
        std::cerr << "[FATAL] load input failure\n";
        return 1;
    }

    part01(grid);
    part02(grid);
    return 0;
}