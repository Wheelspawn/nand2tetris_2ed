#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include "code_writer.h"

CodeWriter::CodeWriter(const std::string& inputFileName) {
	// constructor
	output_filestream.open(inputFileName);
	}

CodeWriter::~CodeWriter() {
	// deconstructor
	output_filestream.close();
}

// RAM[SP++] = 17

// @17
// D=A
// @SP
// A=M
// M=D
// @SP
// M=M+1

// RAM[SP--] = 0

// @SP
// A=M
// M=D
// @SP
// M=0

void CodeWriter::writeArithmetic(std::string& command) {
	
	if (command == "add")
	{
		output_filestream << "@SP"
						  << std::endl
						  << "M=M-1"
						  << std::endl
						  << "A=M"
						  << std::endl
						  << "D=M"			// D=RAM[--SP]
						  << std::endl
						  << "M=0"			// RAM[SP] = 0
						  << std::endl
						  << "@SP"
						  << std::endl
						  << "A=M-1"
						  << std::endl
						  << "M=M+D"		// RAM[SP-1]=RAM[SP-1]+D
						  << std::endl;
	}
	else if (command == "sub")
	{
		return;
	}
	else if (command == "neg")
	{
		return;
	}
	else
	{
		return;
	}
}

void CodeWriter::writePushPop(CommandType command, const std::string& segment, int idx) {
	switch(command) {
		case C_PUSH:
		{
			if (segment == "constant") {
				output_filestream << "@"
						   << idx
						   << std::endl
						   << "D=A"
						   << std::endl
						   << "@SP"
						   << std::endl
						   << "A=M"
						   << std::endl
						   << "M=D"
						   << std::endl
						   << "@SP"
						   << std::endl
						   << "M=M+1"
						   << std::endl;
			}
			break;
		}
		case C_POP:
		{
            // Calculate the effective address segment[index] and store it in RAM[5]
            output_filestream << "@" << segment
                       << std::endl
                       << "D=M"
                       << std::endl
                       << "@" << idx
                       << std::endl
                       << "D=D+A"
                       << std::endl
                       << "@5"
                       << std::endl
                       << "M=D"
                       << std::endl;

            // Pop the top value from the stack and store it in the effective address
            output_filestream << "@SP"
                       << std::endl
                       << "AM=M-1"
                       << std::endl
                       << "D=M"
                       << std::endl
                       << "@5"
                       << std::endl
                       << "A=M"
                       << std::endl
                       << "M=D"
                       << std::endl;
            break;
		}
		default:
			// Invalid CommandType
			break;
	}
}
