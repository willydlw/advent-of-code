#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>

/*  Part 1
        example.txt     41
        input.txt       4939

    Part 2
        example.txt     
        input.txt       ???
*/

static const char GUARD_SYMBOL = '^';
static const char VISITED = 'X';
static const char OBSTACLE = '#';

enum DIRECTION {LEFT, RIGHT, UP, DOWN};

struct Location{
    int row;
    int col;
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
        for(size_t c = 0; c < map[r].size(); c++){
            if(map[r][c] == GUARD_SYMBOL){
                guardPosition->row = r;
                guardPosition->col = c;
                return true;
            }
        }
    }

    return false;
}


int count_visited(const std::vector<std::vector<char>>& map)
{   
    int total = 0;
    for(size_t r = 0; r < map.size(); r++){
        for(size_t c = 0; c < map[r].size(); c++){
            if(map[r][c] == VISITED){
                total++;
            }
        }
    }

    return total;
}

/*  Assumptions:
        map
            - at least one row
            - all rows have same number of columns
        
        Guard Position
            - located within map boundary
            - initial movement direction is up
*/
int part01(std::vector<std::vector<char>>& map)
{
    bool guardInMappedArea = true;
    Location guardLocation;

    int numRows = (int)map.size();
    //int numCols = (int)map[0].size();

    int uniquePositionsVisited = 0;

    if(!find_guard(map, &guardLocation)){
        std::cerr << "Error, cannot find Guard location\n";
        return uniquePositionsVisited;
    }

    map[guardLocation.row][guardLocation.col] = VISITED;

    while(guardInMappedArea){
        int r = guardLocation.row;
        int c = guardLocation.col;
        switch(guardLocation.direction){
            case LEFT:
                if(c-1 >= 0){
                    if(map[r][c-1] == OBSTACLE){
                        // turn right 90 degrees
                        guardLocation.direction = UP;
                    }
                    else{
                        map[r][c-1] = VISITED;
                        guardLocation.col = c-1;
                    }
                }
                else {
                    guardInMappedArea = false;
                }
            break;

            case RIGHT:
                if(c+1 < (int)map[r].size()){
                    if(map[r][c+1] == OBSTACLE){
                        // turn right 90 degrees
                        guardLocation.direction = DOWN;
                    }
                    else{
                        map[r][c+1] = VISITED;
                        guardLocation.col = c+1;
                    }
                }
                else {
                    guardInMappedArea = false;
                }
            break;

            case UP:
                if(r-1 >= 0){
                    if(map[r-1][c] == OBSTACLE){
                        // turn right 90 degrees
                        guardLocation.direction = RIGHT;
                    }
                    else{
                        map[r-1][c] = VISITED;
                        guardLocation.row = r-1;
                    }
                }
                else {
                    guardInMappedArea = false;
                }
            break;
            case DOWN:
                if(r+1 < numRows){
                    if(map[r+1][c] == OBSTACLE){
                        // turn right 90 degrees
                        guardLocation.direction = LEFT;
                    }
                    else{
                        map[r+1][c] = VISITED;
                        guardLocation.row = r+1;
                    }
                }
                else {
                    guardInMappedArea = false;
                }
            break;
            default:
                std::cerr << "Error, function: " << __func__ << ", unknown direction: " 
                    << guardLocation.direction << "\n";
                exit(EXIT_FAILURE);
        }
    }

    uniquePositionsVisited = count_visited(map);
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
    //print_map(map);
    
    int count1 = part01(map);

    std::cout << "part 1: " << count1 << "\n";

    return 0;
}