#include <fstream>
#include <iostream>

#include <vector>
#include <queue>

#include <string>
#include <cstdlib>


#define MAX_HEIGHT 9


struct TrailData{
    int height;
    int row;
    int col;

    TrailData(): height(-1), row(-1), col(-1){}
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

bool inbounds(int x, int min, int max)
{
    return(x >= min && x < max);
}

void search_for_trails(TrailData origin,
        const std::vector<std::vector<int>>& topoMap, int mapRows,
        int mapCols, std::queue<std::vector<TrailData>>& pathsFound)
{
    // neighbors: up, down, left, right
    const int NUM_NEIGHBORS = 4;
    int dx[NUM_NEIGHBORS] = { 0, 0, -1, 1};
    int dy[NUM_NEIGHBORS] = {-1, 1,  0, 0};

    // Path starts at trailhead origin
    std::vector<TrailData> path;
    path.push_back(origin);

    // temporary queue used to search for possible trails
    std::queue<std::vector<TrailData>> possiblePaths;
    possiblePaths.push(path);

    
    while(!possiblePaths.empty()){

        std::vector<TrailData> currentPath = possiblePaths.front();
        pathsFound.pop();

        int nextHeight = currentPath.back().height + 1;
        int row = currentPath.back().row;
        int col = currentPath.back().col;

        if(nextHeight > MAX_HEIGHT){
            // found a complete trail
            pathsFound.push(currentPath);
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
                    currentPath.push_back(neighbor);    // add neighbor to path
                    possiblePaths.push(currentPath);    // add path to search queue
                }
            }
        }
    }

    

}

int part01(const std::vector<std::vector<int>>& topoMap)
{
    int trailheadScore = 0;

    std::queue<std::vector<TrailData>> pathsFound;

    for(size_t r = 0; r < topoMap.size(); r++){
        for(size_t c = 0; c < topoMap[r].size(); c++){
            // trailhead can only start at 0 height 
            if(topoMap[r][c] == 0){
                TrailData origin(topoMap[r][c], r, c);
                search_for_trails(origin, topoMap, topoMap.size(), topoMap[0].size(),
                    pathsFound);
                if(pathsFound.size() > 0){
                    std::cerr << "found " << pathsFound.size() << "\n";
                }
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