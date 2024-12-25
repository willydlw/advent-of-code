#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cctype>
#include <map>

/*  Part 1
        example.txt     14
        input.txt       242 is too low
*/

struct Location{
    int row;
    int col;

    Location():row(0), col(0) {}
    Location(int r, int c) : row(r), col(c){}
};

void print_grid(const std::vector<std::vector<char>>& grid)
{
    for(auto row : grid){
        for(auto col : row){
            std::cout << col;
        }

        std::cout << "\n";
    }
}

void print_map(const std::multimap<char, Location>& antennaMap)
{
    for(auto itr = antennaMap.begin(); itr != antennaMap.end(); itr++){
        std::cout << "key: " << itr->first << ", row: " << itr->second.row 
        << ", col: " << itr->second.col << "\n";
    }
}


void populate_grid(std::string& fileName, std::vector<std::vector<char>>& grid,
                    std::multimap<char, Location>& antennaMap)
{
    std::ifstream infile(fileName);
    if(!infile.is_open()){
        std::cerr << "Error opening file: " << fileName << "\n";
        exit(EXIT_FAILURE);
    }

    std::string line;
    int rowCount = 0;
    while(std::getline(infile, line)){
        std::vector<char> row(line.length());
        for(size_t i = 0; i < line.length(); i++)
        {
            row[i] = line[i];
            if(isalnum(line[i])){
                Location loc(rowCount, i);
                antennaMap.insert(std::pair<char,Location>(line[i], loc));
            }
        }

        grid.push_back(row);
        rowCount++;
    }

    infile.close();
}

bool inbounds(int x, int min, int max)
{
    return (x >= min && x < max);
}


/*  Assumptions:
        grid contains at least one row
        all columns in grid are same width
*/
int part01(std::vector<std::vector<char>>& grid, std::multimap<char, Location> antennaMap)
{
    int antiNodeCount = 0;
    int numRows = grid.size();
    int numCols = grid[0].size();
    std::vector<std::vector<char>> gcopy;
    gcopy.assign(grid.begin(), grid.end());

    for(int r = 0; r < numRows; r++){
        for(int c = 0; c < numCols; c++){
            if(isalnum(grid[r][c])){
                // antenna location

                // find the range of elements with key grid[r][c]
                // equal_range returns a pair of iterators, where the first 
                // iterator points to the first element with this key 
                // and the second iterator points to the element immediately after 
                // the last element with this key
                auto range = antennaMap.equal_range(grid[r][c]);

                // in a multimap, elements are sorted by keys, so all elements
                // with same key are adjacent to each other
                for(auto it = range.first; it != range.second; ++it){
                    if(it->second.row == r && it->second.col == c){
                        // don't make an antinode with myself
                        std::cout << "found myself in the map\n";
                        continue;
                    }

                    if(it->second.row * numCols + it->second.col < r * numCols + c){
                        // already generated antinode for this pair
                        continue;
                    }

                    Location ant1(r,c);
                    Location ant2(it->second.row, it->second.col);
                    int dx = ant1.col - ant2.col;
                    int dy = ant1.row - ant2.row;

                    // antinodes
                    int r1 = ant2.row - dy;
                    int c1 = ant2.col - dx;
                    int r2 = ant2.row + 2 * dy;
                    int c2 = ant2.col + 2 * dx;
                    
                    // add new nodes to grid copy
                    if( inbounds(r1, 0, numRows) && 
                        inbounds(c1, 0, numCols)){                            
                            // don't place antenna where one is
                            // alreay located
                            if(!isalnum(grid[r1][c1])){
                                gcopy[r1][c1] = '#';
                                antiNodeCount++;
                            }
                    }

                    if( inbounds(r2, 0, numRows) && 
                        inbounds(c2, 0, numCols)){
                            if(!isalnum(grid[r2][c2])){
                                gcopy[r2][c2] = '#';
                                antiNodeCount++;
                            }
                    }
                }
            }
        }
    }

    std::cout << "\nGrid Copy with antinodes\n";
    print_grid(gcopy);

    // double counted in loop above due to symmetry duplicate
    // calculations of antinode placements
    return antiNodeCount;
}


int main(int argc, char* argv[])
{
    std::vector<std::vector<char>> grid;
    std::multimap<char, Location> antennaMap;

    std::string fileName = "test1.txt";
    if(argc == 2){
        fileName = argv[1];
    }


    populate_grid(fileName, grid, antennaMap);
    std::cout << "Grid before antinode\n";
    print_grid(grid);

    std::cout << "\nAntenna Map\n";
    print_map(antennaMap);

    int count1;
    count1 = part01(grid, antennaMap);
    std::cout << "part 1: " << count1 << "\n";

    return 0;
}