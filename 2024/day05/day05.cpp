#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <unordered_map>
#include <vector>

#include <cstdlib>

/*  Part 1
        exampleRules.txt, exampleOrder.txt      143
        pageOrder.txt, pageRules.txt            5964

    Part 2
        exampleRules.txt, exampleOrder.txt      123
        pageOrder.txt, pageRules.txt            ???

*/

void print_page_order(const std::vector<std::vector<int>>& order)
{
    for(auto row : order){
        for(auto col : row){
            std::cout << col << " ";
        }
        std::cout << "\n";
    }
}

void populate_page_order(std::string fileName, std::vector<std::vector<int>>& order)
{
    std::ifstream inFile(fileName);
    if(!inFile.is_open()){
        std::cerr << "Error opening file: " << fileName << "\n";
        exit(EXIT_FAILURE);
    }

    std::string line;
    while(std::getline(inFile, line))
    {
        std::vector<int> row;
        std::stringstream iss(line);
        int val1;
        char separator;
        while(iss >> val1){
            row.push_back(val1);
            iss >> separator;
        }

        order.push_back(row);
    }

    inFile.close();
}


void print_page_rules(const std::unordered_multimap<int,int>& rules)
{
    for(const auto& r : rules){
        std::cout << r.first << "|" << r.second << "\n";
    }
}

void populate_page_rules(std::string fileName, std::unordered_multimap<int,int>& rules)
{
    std::ifstream inFile(fileName);
    if(!inFile.is_open()){
        std::cerr << "Error opening file: " << fileName << "\n";
        exit(EXIT_FAILURE);
    }

    std::string line;
    while(std::getline(inFile, line))
    {
        std::stringstream iss(line);
        int val1, val2;
        char separator;
        iss >> val1 >> separator >> val2;
        if(iss){
            rules.insert(std::make_pair(val1, val2));
        }
    }

    inFile.close();
}

int part01(const std::unordered_multimap<int,int>& pageRules, const std::vector<std::vector<int>>& pageOrder,
                 std::vector<std::vector<int>>& inCorrectOrder)
{
    int middleTotal = 0;
    int rightOrderCount = 0;                // used only for debugging

    size_t numRows = pageOrder.size();

    for(size_t r = 0; r < numRows; r++){
        
        // count number of pages in order in this row
        size_t inOrderCount = 0; 

        // number of updates in this row                  
        size_t numUpdates = pageOrder[r].size();

        // inOrder will be true if all pages in this row are ordered
        // such that they comply with page rules
        bool inOrder = true;

        // iterate through all page number updates in this row
        // comparing them to all page numbers listed after them
        // to verify order is correct
        for(size_t i = 0; i < numUpdates && inOrder; i++){
            for(size_t j = i+1; j < numUpdates && inOrder; j++){   

                // search the page rules until a rule is found to check the order     
                bool foundRule = false;
                for(auto itr = pageRules.begin(); itr != pageRules.end() && !foundRule; itr++ ){
                    if(itr->first == pageOrder[r][i] && itr->second == pageOrder[r][j]){
                            foundRule = true;
                    }
                }

                if(!foundRule){
                    std::cerr << "Order Error, row number: "<< r 
                        << "  no rule for this order: " 
                        << pageOrder[r][i] << "|" << pageOrder[r][j] << "\n";
                    inOrder = false;
                }
            }

            if(inOrder){
                inOrderCount++;
            }
            else{
                inCorrectOrder.push_back(pageOrder[r]);
            }
        }

        if(inOrderCount == numUpdates){
            rightOrderCount++;
            int middlePageIndex = numUpdates / 2;
            middleTotal += pageOrder[r][middlePageIndex];
        }
    }

    std::cout << "rightOrderCount: " << rightOrderCount << "\n";
    return middleTotal;
}


int part02(const std::unordered_multimap<int,int>& pageRules, std::vector<std::vector<int>>& pageOrder)
{
    int middleTotal = 0;
    int rightOrderCount = 0;                // used only for debugging

    size_t numRows = pageOrder.size();

    for(size_t r = 0; r < numRows; r++){
        
        // count number of pages in order in this row
        size_t inOrderCount = 0; 

        // number of updates in this row                  
        size_t numUpdates = pageOrder[r].size();

        // inOrder will be true if all pages in this row are ordered
        // such that they comply with page rules
        bool inOrder = true;

        // iterate through all page number updates in this row
        // comparing them to all page numbers listed after them
        // to verify order is correct
        size_t i = 0;
        size_t j;
        while(i < numUpdates){
            j = i + 1;

            while(j < numUpdates && inOrder){

                // search the page rules until a rule is found to check the order     
                bool foundRule = false;
                for(auto itr = pageRules.begin(); itr != pageRules.end() && !foundRule; itr++ ){
                    if(itr->first == pageOrder[r][i] && itr->second == pageOrder[r][j]){
                            foundRule = true;
                    }
                }

                if(!foundRule){
                    /*std::cerr << "Order Error, row number: "<< r 
                        << "  no rule for this order: " 
                        << pageOrder[r][i] << "|" << pageOrder[r][j] << "\n";
                    */
                    inOrder = false;
                }
                else{
                    j++;
                }
            }

            if(inOrder){
                inOrderCount++;
                i++;
            }
            else{
                // switch places and then retest order
                int temp = pageOrder[r][j];
                pageOrder[r][j] = pageOrder[r][i];
                pageOrder[r][i] = temp;
                inOrder = true;
            }
        }

        if(inOrderCount == numUpdates){
            rightOrderCount++;
            int middlePageIndex = numUpdates / 2;
            middleTotal += pageOrder[r][middlePageIndex];
        }
    }

    std::cout << "rightOrderCount: " << rightOrderCount << "\n";
    return middleTotal;
}



int main(int argc, char* argv[])
{
    std::string rulesFile, orderFile;

    // multimap required due to duplicate key values
    std::unordered_multimap<int,int> pageRules;

    std::vector<std::vector<int>> pageOrder;
    std::vector<std::vector<int>> inCorrectOrder;

    if(argc < 3){
        std::cerr << "Usage: a.out rulesFile orderFile\n";
        return 1;
    }

    rulesFile = argv[1];
    orderFile = argv[2];

    populate_page_rules(rulesFile, pageRules);
    //print_page_rules(pageRules);

    populate_page_order(orderFile, pageOrder);
    //print_page_order(pageOrder);

    int middleTotal1;
    middleTotal1 = part01(pageRules, pageOrder, inCorrectOrder);

    std::cout << "part 1 middle total: " << middleTotal1 << "\n";

    std::cout << "part 1 returned the following incorrect order updates\n";
    print_page_order(inCorrectOrder);

    int middleTotal2;
    middleTotal2 = part02(pageRules, inCorrectOrder);
    std::cout << "part 2 middle total: " << middleTotal2 << "\n";

    return 0;
}