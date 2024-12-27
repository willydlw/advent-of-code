#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cctype>
#include <map>

/*  Part 1
        example.txt     14
        input.txt       247

    Part 2
        example.txt     34
        input.txt       861
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

    Parameters are passed by value because this function will change the
    contents and do not want to affect original content.
*/
int part01(std::vector<std::vector<char>> grid, std::multimap<char,Location> antennaMap)
{
    char key;
    int r, c;

    int antiNodeCount = 0;
    int numRows = grid.size();
    int numCols = grid[0].size();


    // get first antenna location in map
    auto itr = antennaMap.begin();
    if(itr != antennaMap.end()){
        key = itr->first;
        r = itr->second.row;
        c = itr->second.col;
        antennaMap.erase(itr);
    }

    while(!antennaMap.empty())
    {
        // find the range of elements with the same key
        // equal_range returns a pair of iterators, where the first 
        // iterator points to the first element with this key 
        // and the second iterator points to the element immediately after 
        // the last element with this key
        auto range = antennaMap.equal_range(key);

        // in a multimap, elements are sorted by keys, so all elements
        // with same key are adjacent to each other
        for(auto keyit = range.first; keyit != range.second; ++keyit){
            
            Location ant1(r,c);
            Location ant2(keyit->second.row, keyit->second.col);
            int dx = ant1.col - ant2.col;
            int dy = ant1.row - ant2.row;

            // possible antinodes
            int r1 = ant1.row + dy;             //ant2.row - dy;
            int c1 = ant1.col + dx;             //ant2.col - dx;
            int r2 = ant2.row - dy;             //ant2.row + 2 * dy;
            int c2 = ant2.col - dx;             //ant2.col + 2 * dx;
            
            // are antinodes locations within grid area
            if( inbounds(r1, 0, numRows) && inbounds(c1, 0, numCols)){
                grid[r1][c1] = '#';
            }

            if( inbounds(r2, 0, numRows) && inbounds(c2, 0, numCols)){
                grid[r2][c2] = '#';
            }
        }

        itr = antennaMap.begin();
        key = itr->first;
        r = itr->second.row;
        c = itr->second.col;
        antennaMap.erase(itr);
    }

    // count unique anti nodes
    for(size_t i = 0; i < grid.size(); i++){
        for(size_t j = 0; j < grid[i].size(); j++){
            if(grid[i][j] == '#'){
                antiNodeCount++;
            }
        }
    }
    return antiNodeCount;
}


/*  antinode occurs at any grid position exactly in line with at least two antennas of 
    the same frequency, regardless of distance. This means that some of the new 
    antinodes will occur at the position of each antenna (unless that antenna is the 
    only one of its frequency)

Assumptions:
        grid contains at least one row
        all columns in grid are same width
*/
int part02(std::vector<std::vector<char>> grid, std::multimap<char,Location> antennaMap)
{
    char key;
    int r, c;

    int antiNodeCount = 0;
    int numRows = grid.size();
    int numCols = grid[0].size();

    // get first antenna location in map
    auto itr = antennaMap.begin();
    if(itr != antennaMap.end()){
        key = itr->first;
        r = itr->second.row;
        c = itr->second.col;
        antennaMap.erase(itr);
    }

    while(!antennaMap.empty())
    {
        // find the range of elements with the same key
        // equal_range returns a pair of iterators, where the first 
        // iterator points to the first element with this key 
        // and the second iterator points to the element immediately after 
        // the last element with this key
        auto range = antennaMap.equal_range(key);

        // in a multimap, elements are sorted by keys, so all elements
        // with same key are adjacent to each other
        for(auto keyit = range.first; keyit != range.second; ++keyit){
            
            Location ant1(r,c);
            Location ant2(keyit->second.row, keyit->second.col);
            int dx = ant1.col - ant2.col;
            int dy = ant1.row - ant2.row;


            // place antinodes
            int aRow = ant1.row += dy;
            int aCol = ant1.col += dx;

            while(inbounds(aRow, 0, numRows) && inbounds(aCol, 0, numCols)){
                grid[aRow][aCol] = '#';
                aRow += dy;
                aCol += dx;
            }

            aRow = ant1.row - dy;
            aCol = ant1.col - dx;

            while(inbounds(aRow, 0, numRows) && inbounds(aCol, 0, numCols)){
                grid[aRow][aCol] = '#';
                aRow -= dy;
                aCol -= dx;
            }
        
        }

        itr = antennaMap.begin();
        key = itr->first;
        r = itr->second.row;
        c = itr->second.col;
        antennaMap.erase(itr);
    }

    //std::cout << "\nGrid Copy with antinodes\n";
    //print_grid(grid);

    for(int i = 0; i < numRows; i++){
        for(int j = 0; j < numCols; j++){
            if(grid[i][j] == '#'){
                antiNodeCount++;
            }
        }
    }
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
    //std::cout << "Grid before antinode\n";
    //print_grid(grid);

    //std::cout << "\nAntenna Map\n";
    //print_map(antennaMap);

    int count1;
    count1 = part01(grid, antennaMap);
    std::cout << "part 1: " << count1 << "\n";

    int count2 = part02(grid, antennaMap);
    std::cout << "part 2: " << count2 << "\n";

    return 0;
}