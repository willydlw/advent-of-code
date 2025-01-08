/* Day 12 - Garden Groups
        part 1
            example.txt     1930
            input.txt       1424472

        part 2
            
*/

#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>

#include <vector>
#include <queue>

struct Plant{
    int row;
    int col;
    int perimeter;
    char type;
    bool visited;

    Plant():row(0), col(0), perimeter(0), type('\0'), visited(false) {}
    Plant(int r, int c, int perim, char letter) : row(r), col(c), 
            perimeter(perim), type(letter), visited(false) {}
};

struct Location{
    int row;
    int col;

    Location() : row(-1), col(-1) {}
    Location(int r, int c): row(r), col(c) {}
};

struct Region{
    std::vector<Location> gardenLocation;
};

void populate_garden(std::string filename, std::vector<std::vector<Plant>>& garden)
{
    std::ifstream infile(filename);
    if(!infile.is_open()){
        std::cerr << "Error opening file " << filename << "\n";
        exit(EXIT_FAILURE);
    }

    std::vector<std::string> grid;
    std::string line;
    while(std::getline(infile, line)){
        grid.push_back(line);
    }

    for(size_t r = 0; r < grid.size(); r++){
        std::vector<Plant> row;
        for(size_t c = 0; c < grid[r].size(); c++){
            // initial perimeter for all plants should be 4
            Plant plant(r, c, 4, grid[r][c]);
            row.push_back(plant);
        }

        garden.push_back(row);
    }
}


bool inbounds(int x, int min, int max){
    return(x >= min && x < max);
}


void discover_neighbors(Plant origin, std::vector<std::vector<Plant>>& garden, 
        Region* region)
{
    // below, above, left, right 
    static const int dx[4] = {0, 0, -1, 1};
    static const int dy[4] = {-1, 1, 0, 0};

    int numRows = static_cast<int>(garden.size());
    int numCols = static_cast<int>(garden[0].size());

    region->gardenLocation.push_back(Location(origin.row, origin.col));

    std::queue<Plant> neighbors;
    neighbors.push(origin);

    while(!neighbors.empty()){
        Plant current = neighbors.front();
        neighbors.pop();

        // check up, down, left, right neighbors to see if they 
        // are the same type
        for(int i = 0; i < 4; i++){
            int r = current.row + dy[i];
            int c = current.col + dx[i];
            if(inbounds(r, 0, numRows) && inbounds(c,0,numCols)){
                if(garden[r][c].type == current.type){
                    // reduce common boundary perimeter
                    garden[current.row][current.col].perimeter -= 1;
                    if(garden[r][c].visited == false){
                        // mark as visited
                        garden[r][c].visited = true;

                        // add new neighbor discovered to queue
                        // so its unvisited neighbors can be checked
                        neighbors.push(garden[r][c]);
                        region->gardenLocation.push_back(Location(r,c));
                    }
                }
            }
        }
    }
}

void find_regions(std::vector<std::vector<Plant>>& garden, std::vector<Region>& regions)
{
    for(size_t r = 0; r < garden.size(); r++){
        for(size_t c = 0; c < garden[r].size(); c++){
            if(garden[r][c].visited == false){
                // found start of new region, are there neighbors
                // of the same plant type?
                garden[r][c].visited = true;

                Region newRegion;
                discover_neighbors(garden[r][c], garden, &newRegion);
                regions.push_back(newRegion);
            }
        }
    }
}

int calc_fencing_price(const std::vector<std::vector<Plant>>& garden, const std::vector<Region>& regions)
{
    int totalPrice = 0;

    for(auto r : regions){
        int area = static_cast<int>(r.gardenLocation.size());
        int perimeter = 0;
        for(auto loc : r.gardenLocation){
            perimeter += garden[loc.row][loc.col].perimeter;
        }

        std::cout << "region info: type: " << garden[r.gardenLocation[0].row][r.gardenLocation[0].col].type 
            << ", area "        << area 
            << ", perimeter: "  << perimeter 
            << ", price: "      << area * perimeter << "\n";
        totalPrice += (area * perimeter);
    }
    
    return totalPrice;
}


int part01(std::vector<std::vector<Plant>>& garden)
{
    std::vector<Region> regions;
    find_regions(garden, regions);
    int fencingPrice = calc_fencing_price(garden, regions);
    return fencingPrice;
}


int main(int argc, char* argv[])
{
    std::string filename = "example.txt";

    if(argc == 2){
        filename = argv[1];
    }

    std::vector<std::vector<Plant>> garden;
    populate_garden(filename, garden);
    int price1 = part01(garden);

    std::cout << "\npart 1: " << price1 << "\n";
}


