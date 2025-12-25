/*  Day 8 - Playground 

    Equipped with a new understanding of teleporter maintenance, you confidently step 
    onto the repaired teleporter pad.
    
    You rematerialize on an unfamiliar teleporter pad and find yourself in a vast 
    underground space which contains a giant playground!
    
    Across the playground, a group of Elves are working on setting up an ambitious 
    Christmas decoration project. Through careful rigging, they have suspended a large 
    number of small electrical junction boxes.
    
    Their plan is to connect the junction boxes with long strings of lights. Most of 
    the junction boxes don't provide electricity; however, when two junction boxes are 
    connected by a string of lights, electricity can pass between those two junction boxes.
    
    The Elves are trying to figure out which junction boxes to connect so that electricity 
    can reach every junction box. They even have a list of all of the junction boxes' 
    positions in 3D space (your puzzle input).
*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cassert>

struct Point{
    long long x = 0;
    long long y = 0;
    long long z = 0;

    Point() = default;
    Point(long long x, long long y, long long z)
        : x(x), y(y), z(z) {}
};

struct Node{
    long long distance = 0;             // distance between two points
    // Points are stored in an array
    // index1, index2 are locations of points in that array
    int index1;                
    int index2;

    Node() = default;
    Node(long long dist, int p1Index, int p2Index) 
        : distance(dist), index1(p1Index), index2(p2Index) {}
};

bool compareDistances(const Node& a, const Node& b)
{
    return a.distance < b.distance;
}

bool loadInput(const std::string& filename, std::vector<Point>& points)
{
    std::ifstream infile(filename);
    if(!infile.is_open()){
        std::cerr << "[ERROR] file open failure\n";
        return false;
    }

    std::string line;
    while(std::getline(infile, line)){
        std::stringstream ss(line);
        char comma;
        Point p;
        if(!(ss >> p.x) || !(ss>> comma) || !(ss >> p.y) 
                || !(ss >> comma) || !(ss >> p.z))
        {
            std::cerr << "[ERROR] extracting points from line: "
                        << line << "\n";
            return false;    
        }
        points.push_back(p);
    }


    return true;
}

void printPoints(const std::vector<Point>& points)
{
    for(const auto& p : points){
        std::cerr << std::setw(6) << p.x << " " << 
            std::setw(6) << p.y << " " << std::setw(6) 
            << p.z << "\n";
    }
}

void calcDistances(const std::vector<Point>& points, std::vector<Node>& nodes)
{
    for(size_t i = 0; i < points.size()-1; i++){
        for(size_t j = i + 1; j < points.size(); j++){
            long long dx = points[i].x - points[j].x;
            long long dy = points[i].y - points[j].y;
            long long dz = points[i].z - points[j].z;
            long long squaredDistance = dx * dx + dy * dy + dz * dz;
            nodes.emplace_back(squaredDistance, i, j);
        }
    }
}


// Return the parent of the i's circuit
int find(std::vector<int>& parents, int i)
{
    // Base case - found root node
    if(parents[i] == i){
        return i;
    }

    // Recursive case - find parent of parent[i]
    // update parent[i]
    parents[i] = find(parents, i);

    return parents[i];
}


// merge i and j circuits
void merge_circuits(std::vector<int> parents, int i, int j)
{
    // i's parent node 
    int pi = find(parents, i);

    // j's parent node
    int pj = find(parents, j);

    // join the circuits by changing j's parent to i's parent
    parents[pj] = pi;
}


// Union-Find Operation 1 make set 
//  Keys are some distance node i from 0 to length of nodes
//  Value is its parent node
//  Initially, in the make set step each node is it's own parent
void make_set(std::vector<int>& parents, size_t numNodes)
{
    for(size_t i = 0; i < numNodes; i++){
        parents[i] = i;
    }
}

/*  Connect pairs of junction boxes that are as close together as possible 
    according to the straight line distance.

    Union-Find Data Structure for managing disjoint sets 
        Quickly determine if two elements belong to the same set
        Quickly merge two sets

        Operations
            1. make set - creates a new set for each element
            2. find - locate the representative (root node) of the set
            3. union - merge two sets

*/
void part01(const std::vector<Point>& junctionBoxes)
{
    // Union-Find Operation 1, make_set
    //  Keys are junction box i from 0 to length of junction boxes
    //  Value is its parent node
    //  Initially, in the make set step each node is it's own parent
    std::vector<int> parents(junctionBoxes.size());
    make_set(parents, junctionBoxes.size());

    std::cerr << "Parent's list\n";
    for(int i : parents){
        std::cerr << i << " ";
    }

    std::cerr << "\n";


    // Create an array of tree nodes (set elements) that store
    // the distance between two points and the index location of 
    // those two points in the junction box positions array
    std::vector<Node> distances;

    // sum integers from 1 to n is n(n+1)/2 
    // distance matrix is symmetric
    // 1 2 3 4
    // 0 1 2 3
    // 0 0 1 2
    // 0 0 0 1 
    // num distances = 4 + 3 + 2 + 1 
    distances.reserve(junctionBoxes.size()*(junctionBoxes.size()+1)/2);       

    // calculate the distance between two points, storing the distance
    // and the 
    calcDistances(junctionBoxes, distances);
    
    

    #if 0
    std::cerr << "Sorted Squared Distances";
    int printCount = 0;
    for(const auto& n : distanceNodes){
        if(printCount % 6 == 0){
            std::cerr << "\n";
        }

        std::cerr << std::setw(10) << n.distance << " ";
        printCount++;
    }

    std::cerr << "\n";
    #endif


    // Use union find to combine nodes into circuits 

    // connect all points starting from shortest distance to longest distance 
    std::sort(distances.begin(), distances.end(), compareDistances);

    // Puzzle specifies joining 1000 shortest distances 
    // test files may be smaller, so use min
    size_t stop = distances.size() >= 1000? 1000 : distances.size();
    for(size_t k = 0; k < stop; k++){
        int i = distances[k].index1;        // index in junction box array
        int j = distances[k].index2;

        // skip over junction boxes that are already in the same circuit
        if(find(parents, i) == find(parents, j)){
            continue;
        }

        // combine the circuits 
        merge_circuits(parents, i , j);
    }

    // find the sizes of the three largest circuits
    // keys: root node number, values: how many nodes belong to that root




    // Operation 2
}

int main(int argc, char* argv[])
{
    std::string filename{"test.txt"};
    if(argc == 2){
        filename = argv[1];
    }

    std::vector<Point> jboxPositions;
    if(!loadInput(filename, jboxPositions)){
        std::cerr << "[FATAL] loadInput failure\n";
        return 1;
    }

    part01(jboxPositions);

    return 0;
}