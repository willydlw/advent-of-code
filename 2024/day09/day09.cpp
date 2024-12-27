/*  Day 9 - Disk Fragmenter

    Part 1 solution
        example.txt     
        input.txt       
    
    Part 2 solution
        example.txt     
        input.txt       
*/
#include <fstream>
#include <iostream>
#include <string>

#include <vector>

void read_diskmap(const std::string& filename, std::string& diskmap)
{
    std::ifstream infile(filename);
    if(!infile.is_open()){
        std::cerr << "Error opening file " << filename << "\n";
        exit(EXIT_FAILURE);
    }

    diskmap.clear();
    if(!(infile >> diskmap)){
        std::cerr << "Error reading file input\n";
        exit(EXIT_FAILURE);
    }

    infile.close();

}

void part01(const std::string& diskmap)
{
    /*  Disk Map Format - File Layout and Free Space
        ---------------------------------------------

        Disk map uses a dense format to represent file layout and free space.
        The digits alternate between indicating the length of a file and the 
        length of free space.

        Examples: 
        
        Disk map 12345 represents a one-block file, two blocks of
        free space, a three block file, four blocks of free space, and then
        a five-block file.

        Disk map 90909 represents three nine-block files in a row with no
        free space between them.


        ID number
        ---------

            Each file on disk has an ID number based on the order of the files
            as they appear before they are rearranged.

        Example: Disk map 12345 has three files.
            A one-block file with ID 0
            A three-block file with ID 1
            A five-block file with ID 2

        Using one character fore each block where digits are the file ID and . is free space, 
        the disk map 12345 represents these individual blocks:
        0..111....22222 
    */

    std::string fileBlocks;
    int fileID = 0;

    for(int i = 0; diskmap.length(); i++){
        
    }
}


int main(int argc, char* argv[])
{
    std::string diskmap;
    std::string filename = "example.txt";
    if(argc == 2){
        filename = argv[1];
    }

    read_diskmap(filename, diskmap);

    std::cout << "diskmap.length() is " << diskmap.length() << "\n";

    return 0;
}