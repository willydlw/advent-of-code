/*  Day 10
        part 1 
            example.txt     36
            input.txt       733
        part 2
            example.txt     
            input.txt
*/

#include <fstream>
#include <iostream>

#include <vector>
#include <queue>
#include <map>

#include <string>
#include <cstdlib>

#define MAX_HEIGHT 9


struct TrailData{
    int height;
    int row;
    int col;

    TrailData(): height(-1), row(-1), col(-1) {}
    TrailData(int h, int r, int c): height(h), row(r), col(c) {}
};

void print_map(const std::vector<std::vector<int>>& map)
{
    for(size_t r = 0; r < map.size(); r++){
        for(size_t c = 0; c < map[r].size(); c++){
            std::cout << map[r][c];
        }
        std::cout << "\n";
    }
}

void populate_map(const std::string& filename, std::vector<std::vector<int>>& topoMap)
{
    std::ifstream infile(filename);
    if(!infile.is_open()){
        std::cerr << "Error opening file: " << filename << "\n";
        exit(EXIT_FAILURE);
    }

    std::string line;
    while(std::getline(infile, line)){
        std::vector<int> row;
        for(size_t i = 0; i < line.size(); i++){
            row.push_back(line[i]-'0');
        }
        topoMap.push_back(row);
    }


    infile.close();
}

void display_path(const std::vector<TrailData>& path, int numRows, int numCols)
{
    std::vector<std::vector<char>> grid;
    for(int r = 0; r < numRows; r++){
        std::vector row(numRows, '.');
        grid.push_back(row);
    }

    for(auto data : path){
        grid[data.row][data.col] = data.height + '0';
    }

    for(int r = 0; r < numRows; r++){
        for(int c = 0; c < numCols; c++){
            std::cout << grid[r][c];
        }
        std::cout << "\n";
    }
}

bool inbounds(int x, int min, int max)
{
    return(x >= min && x < max);
}

/* Passing a copy of the topoMap so changes made to visited are local
   only.
*/
int search_for_trails(TrailData origin,
        std::vector<std::vector<int>> topoMap, int mapRows,
        int mapCols)
{
    // neighbors: up, down, left, right
    static const int NUM_NEIGHBORS = 4;
    static const int dx[NUM_NEIGHBORS] = { 0, 0, -1, 1};
    static const int dy[NUM_NEIGHBORS] = {-1, 1,  0, 0};

    int trailheadScore = 0;

    // Path starts at trailhead origin
    std::vector<TrailData> path;
    path.push_back(origin);

    // temporary queue used to search for possible trails
    std::queue<std::vector<TrailData>> possiblePaths;
    possiblePaths.push(path);

    // track peaks visited from this trailhead origin
    std::map<int, int> peaksVisited;

    
    while(!possiblePaths.empty()){

        std::vector<TrailData> currentPath = possiblePaths.front();
        possiblePaths.pop();

        int nextHeight = currentPath.back().height + 1;
        int row = currentPath.back().row;
        int col = currentPath.back().col;

        if(currentPath.back().height == MAX_HEIGHT){
            // found a complete trail, has this peak already been
            // visited from this trailhead origin?
            auto itr = peaksVisited.find(row*mapCols + col);
            if(itr != peaksVisited.end()){
                itr->second += 1;
            }
            else{
                peaksVisited[row*mapCols+col] = 1;
                trailheadScore += 1;
            }
    
            /*std::cout << "\nPath Found\n";
            display_path(currentPath, mapRows, mapCols);
            std::cout << "Trail head score: " << trailheadScore << '\n';
            std::cerr << "press enter to continue... ";
            getchar();
            */
            continue;
        }

        // path not yet complete
        // Find all neighbors which continue the path
        for(int i = 0; i < NUM_NEIGHBORS; i++){
            int r = row + dy[i];
            int c = col + dx[i];
            if(inbounds(r, 0, mapRows) && inbounds(c, 0, mapCols))
            {
                if(topoMap[r][c] == nextHeight){
                    TrailData neighbor(topoMap[r][c], r, c);
                    // if I don't make a copy, then multiple neighbors 
                    // of the same height will be added to the current
                    // path. Need to maintain unique paths due to the
                    // way trailScore is accumulated.
                    std::vector<TrailData> pathCopy(currentPath);
                    pathCopy.push_back(neighbor);    // add neighbor to path
                    possiblePaths.push(pathCopy);    // add path to search queue
                }
            }
        }
    }

    return trailheadScore;
}

int part01(const std::vector<std::vector<int>>& topoMap)
{
    int trailheadScore = 0;

    for(size_t r = 0; r < topoMap.size(); r++){
        for(size_t c = 0; c < topoMap[r].size(); c++){
            // trailhead can only start at 0 height 
            if(topoMap[r][c] == 0){
                TrailData origin(topoMap[r][c], r, c);
                trailheadScore += search_for_trails(origin, topoMap, topoMap.size(), 
                                                    topoMap[r].size());
            }
        }
    }
    return trailheadScore;
}


int main(int argc, char* argv[])
{
    std::string filename("example.txt");
    if(argc == 2){
        filename = argv[1];
    }

    std::vector<std::vector<int>> topoMap;
    populate_map(filename, topoMap);
    print_map(topoMap);

    int score1 = part01(topoMap);
    std::cout << "\npart 1: " << score1 << "\n";

    return 0;
}