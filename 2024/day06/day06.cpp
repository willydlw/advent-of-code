#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>

/*  Part 1
        example.txt     41
        input.txt       ???

    Part 2
        example.txt     
        input.txt       ???
*/

#define GUARD_SYMBOL '^'

enum DIRECTION {LEFT, RIGHT, UP, DOWN};

struct Location{
    size_t row;
    size_t col;
    enum DIRECTION direction;
    Location() : row(0), col(0), direction(UP) {}
    Location(int r, int c) : row(r), col(c), direction(UP) {}
};


void print_map(const std::vector<std::vector<char>>& map)
{
    for(size_t r = 0; r < map.size(); r++){
        for(size_t c = 0; c < map[r].size(); c++){
            std::cout << map[r][c];
        }
        std::cout << "\n";
    }
}

// Assumes map size is zero 
void populate_map(const std::string& fileName, std::vector<std::vector<char>>& map)
{
    std::ifstream inFile(fileName);
    if(!inFile.is_open()){
        std::cerr << "Error opening file " << fileName << "\n";
        exit(EXIT_FAILURE);
    }

    std::string line;
    while(getline(inFile, line))
    {
        std::vector<char> charLine(line.begin(), line.end());
        map.push_back(charLine);
    }

    inFile.close();
}

bool find_guard(const std::vector<std::vector<char>>& map, Location* guardPosition)
{
    for(size_t r = 0; r < map.size(); r++){
        for(size_t c = 0; c < map[r].size(), c++){
            if(map[r][c] == GUARD_SYMBOL){
                guardPosition->row = r;
                guardPosition->col = c;
                return true;
            }
        }
    }

    return false;
}

/*  Assumptions:
        map
            - at least one row
            - all rows have same number of columns
        
        Guard Position
            - located within map boundary
            - initial movement direction is up
*/
size_t part01(std::vector<std::vector<char>>& map)
{
    const char VISITED = 'X';
    const char OBSTACLE = '#';

    bool guardInMappedArea = true;
    Location guardLocation;

    size_t numRows = map.size();
    size_t numCols = map[0].size();

    size_t uniquePositionsVisited = 0;

    if(!find_guard(map, &guardLocation)){
        std::cerr << "Error, cannot find Guard location\n";
        return uniquePositionsVisited;
    }


    while(guardInMappedArea){
        size_t r = guardLocation.row;
        size_t c = guardLocation.col;
        switch(guardLocation.direction){
            case LEFT:
                if(c-1 >= 0){
                    if(map[r][c-1] == OBSTACLE){
                        // turn right 90 degrees
                        guardLocation.direction = UP;
                    }
                    else{
                        map[r][c-1] == VISITED;
                        guardLocation.col = c-1;
                    }
                }
            break;

            case RIGHT:
            break;
            case UP:
            break;
            case DOWN:
            break;
            default:
                std::cerr << "Error, function: " << __func__ << ", unknown direction: " 
                    << guardLocation.direction << "\n";
                exit(EXIT_FAILURE);
        }

    }

    return uniquePositionsVisited;
}


int main(int argc, char* argv[])
{
    std::string fileName = "example.txt";
    std::vector<std::vector<char>> map;
    Location guardLocation;

    if(argc == 2){
        fileName = argv[1];
    }

    populate_map(fileName, map);
    print_map(map);
    

    return 0;
}