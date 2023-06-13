#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include "parser.h"
#include "code_writer.h"

int main(int argc, char *argv[]) {
    
	// the goal of this program is to read stack machine code and turn it into assembly code.
	// for more info, see Chapter 7 of The Elements of Computing Systems: Building a Modern
	// Computer from First Principles by Noam Nisan and Schimon Shocken. this is just the implementation
	// of the Parser module described in that chapter, the CodeWriter will be developed later.
	
	// input files are going to look like this:
	
	//   push constant 112
	//   sub
	//   neg
	//   and
	//   push constant 82
	//   ...
	
	// and for the sake of demonstrating the incomplete project in main we print:
	// the command type (check commandType() in parser.cpp for an explanation)
	// the first argument (push, sub, neg, and, ...), second argument if it exists (constant), as strings
	
	// on the command line, call
	
	//   a.exe file.txt
	
    if (argc < 2) {
        std::cerr << "no input file specified\n";
        return 1;
    }

    std::string input_file_name = argv[1];
    std::string output_file_name = input_file_name.substr(0,input_file_name.size()-3) +".asm";
	
    Parser parser(input_file_name);
	CodeWriter codewriter(output_file_name);
	
	// print out command type, first argument, second argument
	while (parser.hasMoreLines()) {
		parser.advance();
		
		if (parser.commandType() == C_ARITHMETIC)
		{
			codewriter.writeArithmetic(parser.arg0());
		}
		else
		{
			codewriter.writePushPop(parser.commandType(), parser.arg1(), parser.arg2());
		}
	}
	
	return 0;
}
