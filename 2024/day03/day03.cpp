#include<fstream>
#include <iostream>
#include <string>
#include <stdio.h>


/*  Answers

    Part 1:
        example.txt     161
        input.txt       161085926

    Part 2:
        example2.txt    48
        input.txt       82045421
*/


int extract_value(std::string& line, size_t& index)
{
    int value = 0;

    while(isdigit(line[index])){
        value = value * 10 + line[index] - '0';
        index++;
    }

    return value;
}


long part02_parse_line(std::string& line)
{
    const std::string doString = "do()";
    const std::string dontString = "don't()";
   
    long total = 0;
    int value1 = 0;
    int value2 = 0;

    // input file contains line breaks which cause this function to be called
    // multiple times. enable is static so that it retains the correct state
    // between calls
    static bool enable = true;
    size_t i = 0;

    static int doCount = 0;
    static int dontCount = 0;

    // adding characters to end to avoid exceeding length when
    // checking for characters at index > i to avoid if condition
    line.append("**********");

    size_t n = line.length();

    while(i < n){
        if(line.substr(i, 4) == "do()"){
            enable = true;
            i += 4;
            doCount++;
            //std::cout << "found do(), i: " << i << " next char at line[i] is " << line[i] << '\n';
        }
        else if(line.substr(i, 7) == "don\'t()"){
            enable = false;
            i += 7;
            dontCount++;
            //std::cout << "found don't(), i: " << i << ", next char at line[i] is " << line[i] << '\n';
        }

        if(enable){
            if(line[i] == 'm' && line[i+1] == 'u' && line[i+2] == 'l' && line[i+3] == '('){
                //std::cout << "found mul( at i: " << i << '\n';
                i += 4;
                value1 = extract_value(line, i);
                if(value1 >= 1 && value1 <= 999){
                    if(line[i] == ','){
                        i++;
                        value2 = extract_value(line, i);
                        if(value2 >= 1 && value2 <= 999){
                            if(line[i] == ')'){
                                total += value1 * value2;
                                //std::cout << "values: (" << value1 << ", " << value2 << ")\n";
                            }
                        }
                    }
                }
            }
        }
        
        i++;
    }

    std::cout << "do count: " << doCount << ", dont count: " << dontCount << '\n';

    return total;

}

long part01_parse_line(std::string& line)
{
    const std::string mulString = "mul(";

    long total = 0;
    int value1 = 0;
    int value2 = 0;

    size_t idx = line.find(mulString);
    while(idx != std::string::npos){
        idx += mulString.length();
       
        value1 = extract_value(line, idx);

        if(value1 >= 1 && value1 <= 999){
            if(line[idx] == ','){
                idx++;
                value2 = extract_value(line, idx);
                if(value2 >= 1 && value2 <= 999){
                    if(line[idx] == ')'){
                        idx++;
                        total += value1 * value2;
                    }
                }
            }
        }

        idx = line.find(mulString, idx);
    }

    return total;
}

int main(int argc, char* argv[])
{
    std::string fileName("example.txt");
    if(argc == 2){
        fileName = argv[1];
    }

    std::ifstream inFile(fileName);
    if(!inFile.is_open()){
        std::cerr << "Error opening file: " << fileName << '\n';
        return 1;
    }

    long total1 = 0;
    long total2 = 0;

    int loopCount = 9;
    std::string line;
    while(std::getline(inFile, line)){
        //std::cout << "getline read: " << line << '\n';
        total1 += part01_parse_line(line);
        total2 += part02_parse_line(line);
        loopCount++;
        line.clear();
    }


    inFile.close();

    std::cout << "part 1: " << total1 << '\n';
    std::cout << "part 2: " << total2 << '\n';
    std::cout << "loopCount: " << loopCount << "\n";

    return 0;
}