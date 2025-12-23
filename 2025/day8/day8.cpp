#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>

struct Point{
    long long x = 0;
    long long y = 0;
    long long z = 0;

    Point() = default;
    Point(long long x, long long y, long long z)
        : x(x), y(y), z(z) {}
    Point(const Point& p){
        x = p.x;
        y = p.y;
        z = p.z;
    }
};

struct Distance{
    long long distance = 0;
    Point p1;
    Point p2;

    Distance() = default;
    Distance(long long dist, const Point& p1, const Point& p2) 
        : distance(dist), p1(p1), p2(p2) {}
    Distance(const Distance& d)
    {
        distance = d.distance;
        p1 = d.p1;
        p2 = d.p2;
    }
};

bool compareDistances(const Distance& a, const Distance& b)
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

void calcDistances(const std::vector<Point>& points, std::vector<Distance>& distances)
{
    for(size_t i = 0; i < points.size()-1; i++){
        for(size_t j = i + 1; j < points.size(); j++){
            long long dx = points[i].x - points[j].x;
            long long dy = points[i].y - points[j].y;
            long long dz = points[i].z - points[j].z;
            long long squaredDistance = dx * dx + dy * dy + dz * dz;
            distances.emplace_back(squaredDistance, points[i], points[j]);
        }
    }
}

void part01(const std::vector<Point>& points)
{
    std::vector<Distance> distances;

    // sum integers from 1 to n is n(n+1)/2 
    // distance matrix is symmetric
    // 1 2 3 4
    // 0 1 2 3
    // 0 0 1 2
    // 0 0 0 1 
    // num distances = 4 + 3 + 2 + 1 
    distances.reserve(points.size()*(points.size()+1)/2);       

    calcDistances(points, distances);
    
    std::sort(distances.begin(), distances.end(), compareDistances);

    std::cerr << "Sorted Squared Distances\n";
    int printCount = 0;
    for(const auto& d : distances){
        if(printCount % 6 == 0){
            std::cerr << "\n";
        }

        std::cerr << std::setw(10) << d.distance << " ";
        printCount++;
    }

    std::cerr << "\n";
}

int main(int argc, char* argv[])
{
    std::string filename{"test.txt"};
    std::vector<Point> points;
    if(!loadInput(filename, points)){
        std::cerr << "[FATAL] loadInput failure\n";
        return 1;
    }

    //printPoints(points);
    part01(points);

    return 0;
}