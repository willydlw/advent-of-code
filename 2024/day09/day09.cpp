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


    // number of fileblocks for each file id
    // The file id is the corresponding vector index
    // Examples: 
    //      File ID 0 has two file blocks,  fileblocks[0] = 2;
    //      File ID 1 has three file block, fileblocks[1] = 3;
    std::vector<int> fileblocks;    // represents number of fileblocks

    // list of freespace
    // Input diskmap has pattern of file block, free space, file block, free space.
    // freespace[i] is the amount of free space available immediately after
    // fileblocks[i].
    std::vector<int> freespace;

    // separate disk map information into fileblocks and free space
    // fileblocks vector
    for(size_t i = 0; i < diskmap.length(); i++){
        if(i & 1){
            // freespace located at odd number indices in diskmap string
            freespace.push_back(diskmap[i] - '0');
        }
        else{ // file blocks located at even number indices in diskmap string
            fileblocks.push_back(diskmap[i] - '0');
        }
    }


    /*  defrag vector

        Each index location represents space for one file block and will contain
        the file id. 
    */
    std::vector<int> defrag;

    // store the leftmost file block as the first entry in defrag
    defrag.push_back(fileblocks[0]);

    // Move file blocks one at a time from the end of the disk map to 
    // the leftmost free space block until there are no gaps remaining 
    // between the file blocks.
    size_t numBlocks = fileblocks[0];
    size_t spaceIndex = 0;        // freespace index
    size_t defragIndex = 0;
    size_t fileID = 0;

    // Initialize defrag with the file ID 0 file blocks
    // Example: File ID 0 has 2 file blocks. Therefore, 
    // defrag[0] = 0, defrag[1] = 0
    for(size_t i = 0; i < numBlocks; i++)
    {
        defrag.push_back(fileID);
    }


    // now fill available free space with right most file block


    
    while(left < freespace.size() && right > 0){ 
        int spaceAvailable = freespace[left];
        while(spaceAvailable){

        }
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