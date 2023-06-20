#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include "code_writer.h"

CodeWriter::CodeWriter(const std::string& outFn) {
	// constructor
	output_filestream.open(outFn);
	
	output_filestream << "@256"
					  << std::endl
					  << "D=A"
					  << std::endl
					  << "@SP"
					  << std::endl
					  << "M=D"
					  << std::endl
					  << std::endl;

	eq_counter = 0;
	gt_counter = 0;
	lt_counter = 0;
	}

CodeWriter::~CodeWriter() {
	// deconstructor
	output_filestream << "(INFINITELOOP)"
					  << std::endl
					  << "@INFINITELOOP"
					  << std::endl
					  << "0;JMP"
					  << std::endl;
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

void CodeWriter::writeArithmetic(const std::string& command) {
	
	output_filestream << "// " << command << std::endl;
	
	if (command == "add")
	{
		output_filestream << "@SP"		<< std::endl
						  << "AM=M-1" 	<< std::endl
						  << "D=M"		<< std::endl	// D=RAM[--SP]
						  << "A=A-1"	<< std::endl
						  << "M=M+D"	<< std::endl;	// RAM[SP-1]=RAM[SP-1]+D
	}
	else if (command == "sub")
	{
		output_filestream << "@SP"		<< std::endl
						  << "AM=M-1" 	<< std::endl
						  << "D=M"		<< std::endl	// D=RAM[--SP]
						  << "A=A-1"	<< std::endl
						  << "M=M-D"	<< std::endl;	// RAM[SP-1]=RAM[SP-1]-D
	}
	else if (command == "neg")
	{
		output_filestream << "@SP"		<< std::endl
						  << "A=M-1" 	<< std::endl
						  << "M=-M"		<< std::endl;	// RAM[SP-1]=-RAM[SP-1]
	}
	else if (command == "eq")
	{
		output_filestream << "@SP"		<< std::endl
						  << "AM=M-1" 	<< std::endl
						  << "D=M"		<< std::endl
						  << "A=A-1"	<< std::endl
						  << "D=M-D"	<< std::endl // should be 0 (true) if SP[i-1]==SP[i-2]
						  << "@EQTRUE_"	<< std::to_string(eq_counter) << std::endl
						  << "D;JEQ"	<< std::endl
						  << "D=-1"		<< std::endl // should be -1 (false) if SP[i-1]!=SP[i-2]
						  << "(EQTRUE_"	<< std::to_string(eq_counter) << ")" << std::endl
						  << "@SP"		<< std::endl
						  << "A=M-1"	<< std::endl
						  << "M=!D"		<< std::endl;
		eq_counter++;
		
	}
	else if (command == "gt")
	{
		output_filestream << "@SP"		<< std::endl
						  << "AM=M-1" 	<< std::endl
						  << "D=M"		<< std::endl	// d=y
						  << "A=A-1"	<< std::endl
						  << "D=M-D"	<< std::endl	// d=x-y
						  << "@GTTRUE_"	<< std::to_string(gt_counter) << std::endl
						  << "D;JGT"	<< std::endl
						  << "D=0"		<< std::endl
						  << "@GTEND_"	<< std::to_string(gt_counter) << std::endl
						  << "0;JMP"	<< std::endl
						  << "(GTTRUE_"	<< std::to_string(gt_counter) << ")" << std::endl
						  << "D=-1"		<< std::endl
						  << "(GTEND_"	<< std::to_string(gt_counter) << ")" << std::endl
						  << "@SP"		<< std::endl
						  << "A=M-1"	<< std::endl
						  << "M=D"		<< std::endl;
		gt_counter++;
	}
	else if (command == "lt")
	{
		output_filestream << "@SP"		<< std::endl
						  << "AM=M-1" 	<< std::endl
						  << "D=M"		<< std::endl
						  << "A=A-1"	<< std::endl
						  << "D=M-D"	<< std::endl
						  << "@LTTRUE_"	<< std::to_string(lt_counter) << std::endl
						  << "D;JLT"	<< std::endl
						  << "D=0"		<< std::endl
						  << "@LTEND_"	<< std::to_string(lt_counter) << std::endl
						  << "0;JMP"	<< std::endl
						  << "(LTTRUE_"	<< std::to_string(lt_counter) << ")" << std::endl
						  << "D=-1"		<< std::endl
						  << "(LTEND_"	<< std::to_string(lt_counter) << ")" << std::endl
						  << "@SP"		<< std::endl
						  << "A=M-1"	<< std::endl
						  << "M=D"		<< std::endl;
		lt_counter++;
	}
	else if (command == "and")
	{
		output_filestream << "@SP"		<< std::endl
						  << "AM=M-1" 	<< std::endl
						  << "D=M"		<< std::endl
						  << "A=A-1"	<< std::endl
						  << "M=D&M"	<< std::endl;
	}
	else if (command == "or")
	{
		output_filestream << "@SP"		<< std::endl
						  << "AM=M-1" 	<< std::endl
						  << "D=M"		<< std::endl
						  << "A=A-1"	<< std::endl
						  << "M=D|M"	<< std::endl;
	}
	else if (command == "not")
	{
		output_filestream << "@SP"		<< std::endl
						  << "A=M-1" 	<< std::endl
						  << "M=!M"		<< std::endl;
	}
	else
	{
		return;
	}
}

void CodeWriter::writePushPop(CommandType command, const std::string& segment, int idx) {
	
	std::string segmentName;
	if (segment == "argument") { segmentName = "ARG"; }
	else if (segment == "local") { segmentName = "LCL"; }
	else if (segment == "this" || (segment == "pointer" && idx == 0)) { segmentName = "THIS"; }
	else if (segment == "that" || (segment == "pointer" && idx == 1)) { segmentName = "THAT"; }
	else if (segment == "temp") { segmentName = "TEMP"; }
	
	std::cout << segment << std::endl;
	std::cout << segmentName << std::endl << std::endl;
	
	switch(command) {
		case C_PUSH:
		{			
			if (segment == "constant") {
				
				output_filestream << "// push constant " << idx << std::endl;
				
				output_filestream << "@" << idx			<< std::endl
								  << "D=A"				<< std::endl
								  << "@SP"				<< std::endl
								  << "A=M"				<< std::endl
								  << "M=D"				<< std::endl
								  << "@SP"				<< std::endl
								  << "M=M+1"			<< std::endl;
			}
			else if (segment == "pointer" && idx == 0)
			{
				
				output_filestream << "// push this" << idx << std::endl;
				
				output_filestream << "@THIS"			<< std::endl
								  << "AD=M"				<< std::endl
								  << "A=D"				<< std::endl
								  << "D=M"				<< std::endl
								  << "@SP"				<< std::endl
								  << "A=M"				<< std::endl
								  << "M=D"				<< std::endl
								  << "@SP"				<< std::endl
								  << "M=M+1"			<< std::endl;
			}
			else if (segment == "pointer" && idx == 1)
			{
				
				output_filestream << "// push that " << idx << std::endl;
				
				output_filestream << "@THAT"			<< std::endl
								  << "AD=M"				<< std::endl
								  << "A=D"				<< std::endl
								  << "D=M"				<< std::endl
								  << "@SP"				<< std::endl
								  << "A=M"				<< std::endl
								  << "M=D"				<< std::endl
								  << "@SP"				<< std::endl
								  << "M=M+1"			<< std::endl;
			}
			else {
				
				output_filestream << "// push " << segmentName << " " << idx << std::endl;
				
				output_filestream << "@" << segmentName	<< std::endl
								  << "AD=M"				<< std::endl
								  << "@" << idx		 	<< std::endl
								  << "D=D+A"			<< std::endl
								  << "A=D"				<< std::endl
								  << "D=M"				<< std::endl
								  << "@SP"				<< std::endl
								  << "A=M"				<< std::endl
								  << "M=D"				<< std::endl
								  << "@SP"				<< std::endl
								  << "M=M+1"			<< std::endl;
			}
			break;
		}
		case C_POP:
		{
            // Calculate the effective address segment[index] and store it in RAM[13]
			if (segment == "temp")
			{
				output_filestream << "// pop temp " << idx << std::endl;
				
				output_filestream << "@" << std::to_string(5+idx)
								  << std::endl
								  << "D=A"
								  << std::endl
								  << "@R13"
								  << std::endl
								  << "M=D"
								  << std::endl;
			}
			else
			{
				output_filestream << "// pop " << segmentName << " " << idx << std::endl;
				
				output_filestream << "@" << segmentName
								  << std::endl
								  << "D=M"
								  << std::endl
								  << "@" << idx
								  << std::endl
								  << "D=D+A"
								  << std::endl
								  << "@R13"
								  << std::endl
								  << "M=D"
								  << std::endl;
			}

            // Pop the top value from the stack and store it in the effective address
            output_filestream << "@SP"
                    		  << std::endl
                    		  << "AM=M-1"
                    		  << std::endl
                    		  << "D=M"
                    		  << std::endl
                    		  << "@R13"
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

void CodeWriter::writeLabel(const std::string& label) {
            output_filestream << "(" + label + ")"
							  << std::endl;
}

void CodeWriter::writeGoto(const std::string& label) {
            output_filestream << "@" + label << std::endl
							  << "0;JMP" << std::endl;
}

void CodeWriter::writeIf(const std::string& label) {
            output_filestream << "@SP"
                    		  << std::endl
                    		  << "AM=M-1"
                    		  << std::endl
                    		  << "D=M"
                    		  << std::endl
							  << "@" + label
							  << std::endl
							  << "D;JNE"
							  << std::endl;
}
