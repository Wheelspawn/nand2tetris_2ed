#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include "parser.h"

using namespace std;

Parser::Parser(const string& inputFileName, const string& outputFileName) {
	// constructor
	input_filestream.open(inputFileName);
	output_filestream.open(outputFileName);
	}

Parser::~Parser() {
	// deconstructor
	input_filestream.close();
	output_filestream.close();
}

bool Parser::hasMoreLines() {
	// does the input file have another line available? Return true if true, otherwise false
	return input_filestream.peek() != ifstream::traits_type::eof();
}

string Parser::advance() {
	// advance the input stream by one line and return that line, otherwise return the empty string
	if (getline(input_filestream, currentLine)) {
		return currentLine;
	} else {
		return "";
	}
}

CommandType Parser::commandType() {
	// what is the command type?
	// add, sub, neg: these are arithmetical operations, so C_ARITHMETIC
	// push and pop are C_PUSH and C_POP commands
	// everything else is C_UNKNOWN
	
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

string Parser::arg1() {
	// return the first argument of currentLine
	std::regex arg1_regex(R"(^\s*(\S+))");
	std::smatch match;
	
	if (std::regex_search(currentLine, match, arg1_regex) && match.size() > 1) {
		return match[1].str();
	} else {
		return "";
	}
	
}

string Parser::arg2() {
	// return the second argument of currentLine if it exists, otherwise return empty string
	std::regex arg2_regex(R"(^\s*\S+\s+(\S+))");
	std::smatch match;
	
	if (std::regex_search(currentLine, match, arg2_regex) && match.size() > 1) {
		return match[1].str();
	} else {
		return "";
	}
}

void Parser::allArgs() {
	// (^\s*([a-zA-Z0-9_]*)(\s+)?(\S+)?)
	std::regex arg3_regex(R"(^\s*(\S+)(\s+)?(\S+)?)");
	std::smatch match;
	
	if (std::regex_search(currentLine, match, arg3_regex) && match.size() > 0) {
		cout << match.size() << endl;
		for (int i = 0; i < match.size(); i++)
			cout << match[i].str() << " | ";
		cout << endl;
	}
}
