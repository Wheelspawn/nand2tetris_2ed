#include <algorithm>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include "code_writer.h"

CodeWriter::CodeWriter(const std::string& outFn) {
	// constructor
	output_filestream.open(outFn);
	
	filename = std::filesystem::path(outFn).stem().string();

	eq_counter = 0;
	gt_counter = 0;
	lt_counter = 0;
	call_counter = 0;
	}

CodeWriter::~CodeWriter() {
	// deconstructor
	/*
	output_filestream << "(INFINITELOOP)"
					  << std::endl
					  << "@INFINITELOOP"
					  << std::endl
					  << "0;JMP"
					  << std::endl;
					  */
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
	if (segment == "argument") { segmentName = "2"; }
	else if (segment == "local") { segmentName = "1"; }
	else if (segment == "this") { segmentName = "3"; }
	else if (segment == "that") { segmentName = "4"; }

	switch(command) {
		case C_PUSH:
		{	
			// calculate value at segment[index]
			if (segment == "constant") {
				output_filestream << "// push constant " << idx
								  << std::endl;
				output_filestream << "@" << idx
								  << std::endl
								  << "D=A"
								  << std::endl;
			}
			else if (segment == "static")
			{
				output_filestream << "// push static: " << filename << "." << idx
								  << std::endl;
				output_filestream << "@" << filename << "." << idx
								  << std::endl
								  << "D=A"
								  << std::endl
								  << "@" << idx
								  << std::endl
								  << "D=D+A"
								  << std::endl
								  << "A=D"
								  << std::endl
								  << "D=M"
								  << std::endl;
			}
			else if (segment == "pointer" && idx == 0)
			{
				output_filestream << "// push pointer 0"
								  << std::endl;
				output_filestream << "@3"
								  << std::endl
								  << "D=M"
								  << std::endl;
			}
			else if (segment == "pointer" && idx == 1)
			{
				output_filestream << "// push pointer 1"
								  << std::endl;
				output_filestream << "@4"
								  << std::endl
								  << "D=M"
								  << std::endl;
			}
			else if (segment == "temp")
			{
				output_filestream << "// push temp " << idx
								  << std::endl;
				output_filestream << "@5"
								  << std::endl
								  << "D=A"
								  << std::endl
								  << "@" << idx
								  << std::endl
								  << "D=D+A"
								  << std::endl
								  << "A=D"
								  << std::endl
								  << "D=M"
								  << std::endl;
			}
			else {
				output_filestream << "// push " << segmentName << " " << idx
								  << std::endl;
				output_filestream << "@" << segmentName
								  << std::endl
								  << "AD=M"
								  << std::endl
								  << "@" << idx
								  << std::endl
								  << "D=D+A"
								  << std::endl
								  << "A=D"
								  << std::endl
								  << "D=M"
								  << std::endl;
			}

			// push value of segment[index] onto stack
			output_filestream << "@SP"
							  << std::endl
							  << "A=M"
							  << std::endl
							  << "M=D"
							  << std::endl
							  << "@SP"
							  << std::endl
							  << "M=M+1"
							  << std::endl;

			break;
		}
		case C_POP:
		{
            // Calculate the effective address segment[index]
			if (segment == "temp")
			{
				output_filestream << "// pop temp " << idx << std::endl;
				
				output_filestream << "@" << std::to_string(5+idx)
								  << std::endl
								  << "D=A"
								  << std::endl;
			}
			else if (segment == "static")
			{
				output_filestream << "// pop static: " << filename << "." << idx
								  << std::endl;
				
				output_filestream << "@" << filename << "." << idx
								  << std::endl
								  << "D=A"
								  << std::endl;
			}
			else if (segment == "pointer" && idx == 0)
			{
				output_filestream << "// pop pointer 0" << std::endl;
				
				output_filestream << "@3"
								  << std::endl
								  << "D=A"
								  << std::endl;
			}
			else if (segment == "pointer" && idx == 1)
			{
				output_filestream << "// pop pointer 1" << std::endl;
				
				output_filestream << "@4"
								  << std::endl
								  << "D=A"
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
								  << std::endl;
			}

			// store the effective address in RAM[13]
			output_filestream << "@R13"
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
			output_filestream << "// write-label " << label
							  << std::endl;
            output_filestream << "(" + label + ")"
							  << std::endl;
}

void CodeWriter::writeGoto(const std::string& label) {
			output_filestream << "// write-goto " << label
							  << std::endl;
            output_filestream << "@" + label << std::endl
							  << "0;JMP" << std::endl;
}

void CodeWriter::writeIf(const std::string& label) {
			output_filestream << "// write-if " << label
							  << std::endl;
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

void CodeWriter::writeFunction(const std::string& functionName, int nVars) {
	output_filestream << "// function " << functionName
					  << std::endl;
	
	writeLabel(functionName);
	
	// repeat nVars times:
	// push 0
	output_filestream << "// repeat " << std::to_string(nVars) << " times:"
					  << std::endl;

	for (int i = 0; i < nVars; ++i)
	{
		output_filestream << "// push local 0"
						  << std::endl;
		output_filestream << "@0"
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
}

void CodeWriter::writeCall(const std::string& functionName, int nArgs) {
	output_filestream << "// write-call " << functionName
					  << std::endl;

	// push returnAddress
	output_filestream << "// push returnAddress"
					  << std::endl;
	output_filestream << "@" + functionName + "_" + std::to_string(call_counter)
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

	// push LCL
	output_filestream << "// push LCL"
					  << std::endl;
	output_filestream << "@LCL"
					  << std::endl
					  << "D=M"
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

	// push ARG
	output_filestream << "// push ARG"
					  << std::endl;
	output_filestream << "@ARG"
					  << std::endl
					  << "D=M"
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

	// push THIS
	output_filestream << "// push THIS"
					  << std::endl;
	output_filestream << "@THIS"
					  << std::endl
					  << "D=M"
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

	// push THAT
	output_filestream << "// push THAT"
					  << std::endl;
	output_filestream << "@THAT"
					  << std::endl
					  << "D=M"
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
	
	// ARG = SP-5-nArgs
	output_filestream << "// ARG = SP-5-nArgs"
					  << std::endl;
	output_filestream << "@SP"
					  << std::endl
					  << "D=M"
					  << std::endl
					  << "@5"
					  << std::endl
					  << "D=D-A"
					  << std::endl
					  << "@" + std::to_string(nArgs)
					  << std::endl
					  << "D=D-A"
					  << std::endl
					  << "@ARG"
					  << std::endl
					  << "M=D"
					  << std::endl;
	
	// LCL = SP
	output_filestream << "// LCL = SP"
					  << std::endl;
	output_filestream << "@SP"
					  << std::endl
					  << "D=M"
					  << std::endl
					  << "@LCL"
					  << std::endl
					  << "M=D"
					  << std::endl;
	
	// goto f
	output_filestream << "// goto f"
					  << std::endl;
	writeGoto(functionName);
	
	// ( returnAddress )
	output_filestream << "// ( returnAddress )"
					  << std::endl;
	writeLabel(functionName + "_" + std::to_string(call_counter++));
}

void CodeWriter::writeReturn() {
	output_filestream << "// write-return"
					  << std::endl;
	
	/*
	// frame = LCL
	output_filestream << "// frame = LCL"
					  << std::endl;
	output_filestream << "@1"
					  << std::endl
					  << "A=M"
					  << std::endl
					  << "D=M"
					  << std::endl
					  << "@FRAME"
					  << std::endl
					  << "M=D"
					  << std::endl; */
	
	// frame = LCL
	output_filestream << "// frame = LCL"
					  << std::endl;
	output_filestream << "@LCL"
					  << std::endl
					  << "D=M"
					  << std::endl
					  << "@FRAME"
					  << std::endl
					  << "M=D"
					  << std::endl;
					  
	// retAddr = *(frame-5)
	output_filestream << "// retAddr = *(frame-5)"
					  << std::endl;
	output_filestream << "@FRAME"
					  << std::endl
					  << "D=M"
					  << std::endl
					  << "@5"
					  << std::endl
					  << "D=D-A"
					  << std::endl
					  << "A=D"
					  << std::endl
					  << "D=M"
					  << std::endl
					  << "@RETADDR"
					  << std::endl
					  << "M=D"
					  << std::endl;
	
	// *ARG = pop()
	output_filestream << "// *ARG = pop()"
					  << std::endl;
	output_filestream << "@SP"
                      << std::endl
                      << "AM=M-1"
                      << std::endl
                      << "D=M"
					  << std::endl
					  << "@ARG"
					  << std::endl
					  << "A=M"
					  << std::endl
					  << "M=D"
					  << std::endl;

	// SP = ARG+1
	output_filestream << "// SP = ARG+1"
					  << std::endl;
	output_filestream << "@ARG"
					  << std::endl
					  << "D=M+1"
					  << std::endl
					  << "@SP"
					  << std::endl
					  << "M=D"
					  << std::endl;
	
	// restore THAT
	output_filestream << "// restore THAT"
					  << std::endl;
	output_filestream << "@FRAME"
					  << std::endl
					  << "D=M"
					  << std::endl
					  << "@1"
					  << std::endl
					  << "D=D-A"
					  << std::endl
					  << "A=D"
					  << std::endl
					  << "D=M"
					  << std::endl
					  << "@THAT"
					  << std::endl
					  << "M=D"
					  << std::endl;
					  
	// restore THIS
	output_filestream << "// restore THIS"
					  << std::endl;
	output_filestream << "@FRAME"
					  << std::endl
					  << "D=M"
					  << std::endl
					  << "@2"
					  << std::endl
					  << "D=D-A"
					  << std::endl
					  << "A=D"
					  << std::endl
					  << "D=M"
					  << std::endl
					  << "@THIS"
					  << std::endl
					  << "M=D"
					  << std::endl;
	
	// restore ARG
	output_filestream << "// restore ARG"
					  << std::endl;
	output_filestream << "@FRAME"
					  << std::endl
					  << "D=M"
					  << std::endl
					  << "@3"
					  << std::endl
					  << "D=D-A"
					  << std::endl
					  << "A=D"
					  << std::endl
					  << "D=M"
					  << std::endl
					  << "@ARG"
					  << std::endl
					  << "M=D"
					  << std::endl;
	
	// restore LCL
	output_filestream << "// restore LCL"
					  << std::endl;
	output_filestream << "@FRAME"
					  << std::endl
					  << "D=M"
					  << std::endl
					  << "@4"
					  << std::endl
					  << "D=D-A"
					  << std::endl
					  << "A=D"
					  << std::endl
					  << "D=M"
					  << std::endl
					  << "@LCL"
					  << std::endl
					  << "M=D"
					  << std::endl;
	
	// goto retAddr
	output_filestream << "// goto retAddr"
					  << std::endl;
	output_filestream << "@RETADDR"
					  << std::endl
					  << "A=M"
					  << std::endl
					  << "0;JMP"
					  << std::endl;
}

void CodeWriter::writeInit() {
	output_filestream << "Bootstrap code initialization" << std::endl;
	// SP = 256
	output_filestream << "@256" << std::endl;
	output_filestream << "D=A" << std::endl;
	output_filestream << "@SP" << std::endl;
	output_filestream << "M=D" << std::endl;
	// call Sys.init
	writeCall("Sys.init", 0);
}



