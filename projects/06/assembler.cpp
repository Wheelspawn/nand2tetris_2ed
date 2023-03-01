#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include "assembler.h"

using namespace std;

enum InstructionType {
    A_INSTRUCTION,
    C_INSTRUCTION,
    L_INSTRUCTION,
    NON_INSTRUCTION
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
        line = line.substr(0,line.find_first_of("//"));
        int instruction_type = instructionType(line);

        switch(instruction_type) {
            case L_INSTRUCTION:
                {
                    string l_inst = trim(line, " ()\t\r\n");
                    cout << l_inst << endl;
                    symbol_table.insert(pair<string,int>(l_inst,count));
                    break;
                }
            case A_INSTRUCTION:
            case C_INSTRUCTION:
                count++;
                break;
            default:
                break;
        }
    }

    input_file.close();
    
    // for (map<string,int>::const_iterator it = symbol_table.begin(); it != symbol_table.end(); ++it) {
    //     std::cout << it->first << " " << it->second << "\n";
    // }

    ifstream input_file_2(input_file_name);

    // pass 2
    int free_mem = 16;
    while (std::getline(input_file_2, line)) {
        line = line.substr(0,line.find_first_of("//"));

        int instruction_type = instructionType(line);

        switch(instruction_type) {
            case A_INSTRUCTION:
            {
                string a_inst = trim(line, " @\t\r\n");

                char* p;
                long a_long = strtol(a_inst.c_str(), &p, 10);

                if (!isNumber(a_inst))
                {
                    if (symbol_table.find(a_inst) == symbol_table.end())
                    {
                        symbol_table.insert(pair<string,int>(a_inst,free_mem++));
                        output_file << decimalToBinary(symbol_table.find(a_inst)->second) << "\n";
                    }
                    else
                    {
                        output_file << decimalToBinary(symbol_table.find(a_inst)->second) << "\n";
                    }
                }
                else
                {
                    output_file << decimalToBinary(a_long) << "\n";
                }

                break;
            }
            case C_INSTRUCTION:
            {
                string c_inst = trim(line, " \t\r\n");

                int assignment = c_inst.find('=');
                int semicolon = c_inst.find(';');

                cout << c_inst << endl;
                cout << c_inst.substr(assignment+1) << endl;
                cout << c_inst.substr(0,assignment) << endl;
                cout << c_inst.substr(0,semicolon) << endl;
                cout << c_inst.substr(semicolon+1) << endl;
                cout << endl;

                if (assignment != std::string::npos)
                {
                    output_file << "111" << comp(c_inst.substr(assignment+1)) << dest(c_inst.substr(0,assignment)) << "000" << "\n";
                }
                else if (semicolon != std::string::npos)
                {
                    output_file << "111" << comp(c_inst.substr(0,semicolon)) << "000" << jump(c_inst.substr(semicolon+1)) << "\n";
                }
                break;
            }
        }
    }

    // for (map<string,int>::const_iterator it = symbol_table.begin(); it != symbol_table.end(); ++it) {
    //     std::cout << it->first << " " << it->second << "\n";
    // }

    input_file_2.close();
    output_file.close();

    return 0;
}

static string trimLeft (string instruction, string sep) {
    size_t i = instruction.find_first_not_of(sep);
    return (i == std::string::npos) ? "" : instruction.substr(i,instruction.size()-i);
}

static string trimRight (string instruction, string sep) {
    size_t i = instruction.find_last_not_of(sep);
    return (i == std::string::npos) ? "" : instruction.substr(0,i+1);
}

static string trim (string instruction, string sep) {
    return trimLeft(trimRight(instruction, sep), sep);
}

static int instructionType(string instruction)
{
    if (instruction.find('@') != std::string::npos)
    {
        return A_INSTRUCTION;
    }
    else if (instruction.find('(') !=std::string::npos && instruction.find(')') !=std::string::npos)
    {
        return L_INSTRUCTION;
    }
    else if (instruction.find('=') != std::string::npos || instruction.find(';') !=std::string::npos)
    {
        return C_INSTRUCTION;
    }
    else
    {
        return NON_INSTRUCTION;
    }
}

static string dest(string instruction) {
    if (instruction=="null") return "000";
    if (instruction=="M") return "001";
    if (instruction=="D") return "010";
    if (instruction=="DM" || instruction=="MD") return "011";
    if (instruction=="A") return "100";
    if (instruction=="AM") return "101";
    if (instruction=="AD") return "110";
    if (instruction=="ADM") return "111";
    return "";
}

static string comp(string instruction) {

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

    if (instruction=="D-1") return "0001110";

    if (instruction=="A-1") return "0110010";
    if (instruction=="M-1") return "1110010";

    if (instruction=="D+A") return "0000010";
    if (instruction=="D+M") return "1000010";

    if (instruction=="D-A") return "0010011";
    if (instruction=="D-M") return "1010011";

    if (instruction=="A-D") return "0000111";
    if (instruction=="M-D") return "1000111";

    if (instruction=="D&A") return "0000000";
    if (instruction=="D&M") return "1000000";

    if (instruction=="D|A") return "0010101";
    if (instruction=="D|M") return "1010101";

    return "";
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
    return "";
}

bool isNumber(const string& s)
{
    return !s.empty() && find_if(s.begin(), 
        s.end(), [](unsigned char c) { return !isdigit(c); }) == s.end();
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
