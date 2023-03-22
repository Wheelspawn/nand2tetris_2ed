#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include "parser.h"

using namespace std;

Parser::Parser(const string& inputFileName, const string& outputFileName) {
	input_filestream.open(inputFileName);
	output_filestream.open(outputFileName);
	}

Parser::~Parser() {
	input_filestream.close();
	output_filestream.close();
}

bool Parser::hasMoreLines() {
	return input_filestream.peek() != ifstream::traits_type::eof();
}

string Parser::advance() {
	if (getline(input_filestream, currentLine)) {
		return currentLine;
	} else {
		return "";
	}
}

CommandType Parser::commandType() {
	regex arithmetic_regex(R"(^(add|sub|neg)\s*(\r\n|\r|\n)?)");
	regex push_regex(R"(^push\s+\S+\s+\d+\s*(\r\n|\r|\n)?)");
	regex pop_regex(R"(^pop\s+\S+\s+\d+\s*(\r\n|\r|\n)?)");
	
	if (regex_match(currentLine, arithmetic_regex)) {
		return C_ARITHMETIC;
	} else if (regex_match(currentLine, push_regex)) {
		return C_PUSH;
	} else if (regex_match(currentLine, pop_regex)) {
		return C_POP;
	} else {
		return C_UNKNOWN;
	}
}
