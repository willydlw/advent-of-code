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

#include <cstdlib>          // exit

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

long long part01(const std::string& diskmap)
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

    
    /* defrag vector
            File block values are stored in even number indices
                The standard pair first number is the file id. Note this is
                redundant as file id can be determined as index/2.

                The standard pair second int is the file block size

       Free space is stored in odd number indices
                The standard pair first number is amount of free space
                The standard pair second value is the file block size stored there
    */
    std::vector<int> defrag;

    std::vector<int> fileblocks;    // index is file id
    std::vector<int> freespace;

    // store file id and file block size in even numbered indices
    for(size_t i = 0; i < diskmap.length(); i++){
        if(i & 1){
            // freespace at odd number indices in string
            freespace.push_back(diskmap[i] - '0');
        }
        else{
            fileblocks.push_back(diskmap[i] - '0');
        }
    }

    // create a vector to store defragmented result
    // store the leftmost file block as the first entry in defrag
    defrag.push_back(fileblocks[0]);

    // Move file blocks one at a time from the end of the disk map to 
    // the leftmost free space block until there are no gaps remaining 
    // between the file blocks.
    size_t right = fileblocks.size()-1;
    size_t left = 0;        // freespace index
    
    while(left < freespace.size() && right > 0){ 
        if(fileblocks[right] <= freespace[left]){
            // amount of rightmost fileblocks is <= available free space
            defrag.push_back(fileblocks[right]);
            freespace[left] = freespace[left] - fileblocks[right]; // reduce free space
            right--;
        }
        else{ 
            // number of fileblocks is greater than the available free space
            // move some of the file blocks 
            defrag.push_back(fileblocks[right]);
            fileblocks[right] -= freespace[left];
            left++;
        }
    }

    std::cout << "Defrag pattern\n";
    for(size_t i = 0; i < defrag.size(); i++)
    {
        std::cout << defrag[i] << " ";
    }

    std::cout << "\n";
    
    // calculate checksum
    long long checksum = 0;
    for(size_t i = 0; i < defrag.size(); i++){
        checksum = checksum + i * defrag[i];
    }
    
    return checksum;
    
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

    long long sum1 = part01(diskmap);

    std::cout << "part 1: " << sum1 << "\n";

    return 0;
}