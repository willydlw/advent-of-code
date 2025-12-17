#include <algorithm>    // std::count
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

/*  Answers
    Part 1
        test.txt  - Part 1 sum: 4277556
        input.txt - Part 1 sum: 6209956042374
    Part 2
        test.txt  - Part 2 sum 3263827
        input.txt - Part 2 sum 12608160008022
*/

struct Worksheet{
    std::size_t rows = 0;
    std::size_t cols = 0;
    std::vector<int> operands;
    std::vector<char> operators;
};


bool loadPart2Input(const std::string& filename, std::vector<std::string>& symbols)
{
    std::ifstream infile(filename);
    if(!infile.is_open()){
        std::cerr << "[ERROR] opening file: " << filename << "\n";
        return false;
    }

    std::string line;
    while(std::getline(infile, line)){
        symbols.push_back(line);
    }

    infile.close();
    return true;
}

bool loadInput(const std::string& filename, Worksheet& worksheet)
{
    std::ifstream infile(filename);
    if(!infile.is_open()){
        std::cerr << "[ERROR] opening file: " << filename << "\n";
        return false;
    }

    // each row, except the last is terminated with newline character
    std::size_t rowCount = std::count(std::istreambuf_iterator<char>(infile),
        std::istreambuf_iterator<char>(), '\n');

    //std::cout << "row count: " << rowCount << "\n";

    infile.clear();         // clears EOF flag 
    infile.seekg(0, std::ios::beg);     // move pointer to start of file

    std::string line;
    while(worksheet.rows < rowCount && std::getline(infile, line)){
        std::stringstream ss(line);
        int value;
        while(ss >> value){
            worksheet.operands.push_back(value);
        }
        worksheet.rows++;
    }

    if(worksheet.rows != rowCount){
        std::cerr << "[ERROR] reading operands, rowCount: " << rowCount 
            << " != worksheet.rows: " << worksheet.rows << "\n";
        infile.close();
        return false;
    }

    worksheet.cols = worksheet.operands.size() / worksheet.rows;

    while(getline(infile,line)){
        std::stringstream ss(line);
        char opcode;
        while(ss >> opcode){
            worksheet.operators.push_back(opcode);
        }
    }

    infile.close();

    if(worksheet.operators.size() != worksheet.cols){
        std::cerr << "[ERROR] worksheet.operators.size(): " << worksheet.operators.size() 
            << " != worksheet.cols: " << worksheet.cols << "\n";
        return false;
    }

    return true;
}

void part01(const Worksheet& worksheet)
{
    long long total = 0;
    size_t opIndex = 0;
    for(size_t c = 0; c < worksheet.cols && opIndex < worksheet.cols; c++, opIndex++){
        switch(worksheet.operators[opIndex]){
            case '*':
            {
                long long product = 1;
                for(size_t r = 0; r < worksheet.rows; r++){
                    product *= worksheet.operands[r * worksheet.cols + c];
                }
                total += product;
            }
            break;
            case '+':
            {
                long long sum = 0;
                for(size_t r = 0; r < worksheet.rows; r++){
                    sum += worksheet.operands[r * worksheet.cols + c];
                }
                total += sum;
            }
            break;
        }
    }

    std::cerr << "Part 1 sum: " << total << "\n";
}


// Assumption: each string is the same length
void part02(const std::string& filename)
{
    long long total = 0;
    std::vector<std::string> symbols;

    if(!loadPart2Input(filename, symbols)){
        std::cerr << "[ERROR] part 2 failed to load input\n";
        std::exit(EXIT_FAILURE);
    }

    if(symbols.empty()){
        std::cerr << "[ERROR] part 2, symbols empty\n";
        std::exit(EXIT_FAILURE);
    }

    int rows = static_cast<int>(symbols.size());
    int cols = static_cast<int>(symbols[0].size());

    int operatorCol = cols - 1;         // start at rightmost column

    char operand = ' ';
    while(operatorCol > 0){
        int stopCol = operatorCol;
        int startCol = operatorCol;
        int operatorRow = rows - 1;
        while(stopCol >= 0){
            operand = symbols[operatorRow][stopCol];
            if( operand != ' '){
                //std::cerr << "found operand " << operand << " in column " << stopCol << "\n";
                break;
            }
            stopCol--;
        }

        // extract numbers
        std::vector<int> numbers;

        for(int c = startCol; c >= stopCol; c--){
            int value = 0;
            for(int r = 0; r < rows-1; r++){
                if(isdigit(symbols[r][c])){
                    value = value * 10 + static_cast<int>(symbols[r][c] - '0');
                }
            }

            //std::cerr << "extracted value: " << value << "\n";
            numbers.push_back(value);
        }

        switch(operand){
            case '+':
            {
                long long sum = 0;
                for(auto num : numbers){
                    sum += num;
                }
                //std::cerr << "sum: " << sum << "\n";
                total += sum;
            }
            break;
            case '*':
            {
                long long product = 1;
                for(auto num : numbers){
                    product *= num;
                }
                //std::cerr << "product: " << product << "\n";
                total += product;
            }
            break;
        }
        operatorCol = stopCol - 2;  // -1 moves to column left of operator, another -1 skips over space column
    }

    std::cerr << "Part 2 sum " << total << "\n";
}

int main(int argc, char* argv[])
{
    std::string filename{"test.txt"};
    if(argc ==  2){
        filename = argv[1];
    }

    Worksheet worksheet;
    if(!loadInput(filename, worksheet)){
        std::cerr << "Terminating due to load input failure\n";
    }

    part01(worksheet);
    part02(filename);
    return 0;
}