#include <iostream>
#include <fstream>
#include <string>
#include "assembler.h"

using namespace std;

enum InstructionType {
    A_INSTRUCTION,
    C_INSTRUCTION,
    L_INSTRUCTION
    };

int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::cerr << "no input file specified\n";
        return 1;
    }

    string input_file_name = argv[1];
    string output_file_name = input_file_name +".hack";

    ifstream input_file(input_file_name);
    ofstream output_file(output_file_name);
    
    if (!input_file.is_open() || !output_file.is_open()) {
        std::cerr << "Error opening file(s)\n";
        return 1;
    }

    // pass 1
    int count = 0;
    std::string line;
    while (std::getline(input_file, line)) {
        int instruction_type = instructionType(line);

        switch(instruction_type) {
            case L_INSTRUCTION:
                {
                    string l_inst = line.substr(line.find("(")+1,line.find(")")-(line.find("(")+1));
                    cout << l_inst << endl;
                    symbol_table.insert(pair<string,int>(l_inst,count+1));
                    break;
                }
            default:
                count += 1;
                break;
        }
    }

    for (map<string,int>::const_iterator it = symbol_table.begin(); it != symbol_table.end(); ++it) {
        std::cout << it->first << " " << it->second << "\n";
    }

    /*
    // pass 2
    std::string line;
    while (std::getline(input_file, line)) {
        line = line.substr(0,line.size()-1);
        int instruction_type = instructionType(line);

        switch(instruction_type) {
            case A_INSTRUCTION:
                output_file << decimalToBinary(atoi(line.substr(1).c_str())) << "\n";
                break;
            case L_INSTRUCTION:
                output_file << "TODO" << endl;
                break;
            case C_INSTRUCTION:
                int assignment = line.find('=');
                int semicolon = line.find(';');

                if (assignment != std::string::npos)
                {
                    output_file << "111" << comp(line.substr(assignment+1)) << dest(line.substr(0,assignment)) << "000" << "\n";
                }
                else if (semicolon != std::string::npos)
                {
                    output_file << "111" << comp(line.substr(0,semicolon)) << "000" << jump(line.substr(semicolon)) << "\n";
                }
                break;
        }
    }
    */

    input_file.close();
    output_file.close();

    return 0;
}

static int instructionType(string instruction)
{
    if (instruction.find('@') != std::string::npos)
    {
        // A instruction
        return A_INSTRUCTION;
    }
    else if (instruction.find('(') !=std::string::npos && instruction.find(')') !=std::string::npos)
    {
        // L instruction
        return L_INSTRUCTION;
    }
    else
    {
        // C instruction
        return C_INSTRUCTION;
    }
}

static string dest(string instruction) {
    if (instruction=="null") return "000";
    if (instruction=="M") return "001";
    if (instruction=="D") return "010";
    if (instruction=="DM") return "011";
    if (instruction=="A") return "100";
    if (instruction=="AM") return "101";
    if (instruction=="AD") return "110";
    if (instruction=="ADM") return "111";
    return "bad";
}

static string comp(string instruction) {

    cout << instruction << endl;

    if (instruction=="0") return "0101010";
    if (instruction=="1") return "0111111";
    if (instruction=="-1") return "0111010";
    if (instruction=="D") return "0001100";

    if (instruction=="A") return "0110000";
    if (instruction=="M") return "1110000";

    if (instruction=="!D") return "0001101";

    if (instruction=="!A") return "0110001";
    if (instruction=="!M") return "1110001";

    if (instruction=="-D") return "0001111";

    if (instruction=="-A") return "0110011";
    if (instruction=="-M") return "1110011";

    if (instruction=="D+1") return "0011111";

    if (instruction=="A+1") return "0110111";
    if (instruction=="M+1") return "1110111";

    if (instruction=="D-1") return "001110";

    if (instruction=="A-1") return "0110010";
    if (instruction=="M-1") return "1110010";

    if (instruction=="D+A") return "0000010";
    if (instruction=="D+M") return "1000010";

    if (instruction=="D-A") return "0000010";
    if (instruction=="D-M") return "1000010";

    if (instruction=="A-D") return "0000111";
    if (instruction=="M-D") return "1000111";

    if (instruction=="D&A") return "0000000";
    if (instruction=="D&M") return "1000000";

    if (instruction=="D|A") return "0010101";
    if (instruction=="D|M") return "1010101";

    return "bad";
}

static string jump(string instruction) {
    if (instruction=="null") return "000";
    if (instruction=="JGT") return "001";
    if (instruction=="JEQ") return "010";
    if (instruction=="JGE") return "011";
    if (instruction=="JLT") return "100";
    if (instruction=="JNE") return "101";
    if (instruction=="JLE") return "110";
    if (instruction=="JMP") return "111";
    return "bad";
}

static string decimalToBinary(int num) {
    string s = "";
    while (num > 0) {
        int bit = num % 2;
        s = to_string(bit) + s;
        num /= 2;
    }
    s = string(16-s.size(), '0') + s;
    return s;
}
