#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include "parser.h"

Parser::Parser(const std::string& inputFileName, const std::string& outputFileName) {
	// constructor
	input_filestream.open(inputFileName);
	}

Parser::~Parser() {
	// deconstructor
	input_filestream.close();
}

bool Parser::hasMoreLines() {
	// does the input file have another line available? Return true if true, otherwise false
	return input_filestream.peek() != std::ifstream::traits_type::eof();
}

std::string Parser::advance() {
	// advance the input stream by one line and return that line, otherwise return the empty string
	if (std::getline(input_filestream, currentLine)) {
		currentLine = trim(currentLine);
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

	std::regex arithmetic_regex(R"(^(add|sub|neg)\s*(\r\n|\r|\n)?)");
	std::regex push_regex(R"(^push\s+\S+\s+\d+\s*(\r\n|\r|\n)?)");
	std::regex pop_regex(R"(^pop\s+\S+\s+\d+\s*(\r\n|\r|\n)?)");

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

std::string Parser::arg1() {
	// return the first argument of currentLine
	std::regex arg1_regex(R"(^\s*(\S+))");
	std::smatch match;

	if (std::regex_search(currentLine, match, arg1_regex) && match.size() > 1) {
		return match[1].str();
	} else {
		return "";
	}

}

std::string Parser::arg2() {
	// return the second argument of currentLine if it exists, otherwise return empty string
	std::regex arg2_regex(R"(^\s*\S+\s+(\S+))");
	std::smatch match;

	if (std::regex_search(currentLine, match, arg2_regex) && match.size() > 1) {
		return match[1].str();
	} else {
		return "";
	}
}

void Parser::args() {
	// (^\s*([a-zA-Z0-9_]*)(\s+)?(\S+)?)
	// (^\s*(\S+)(\s+)?(\S+)?)")
	std::regex arg_regex(R"(^(\S+)(\s+)?(\S+)?)");
	std::smatch match;

	if (std::regex_search(currentLine, match, arg_regex) && match.size() > 0) {
		// std::cout << match.size() << std::endl;
		for (int i = 0; i < match.size(); i++)
			std::cout << match[i].str() << " | ";
		std::cout << std::endl;
	}
}

std::string Parser::trim(const std::string& s) {
    std::size_t end = s.find_last_not_of(" \r\n\t\f\v");
    std::string s_trimmed = (end == std::string::npos) ? "" : s.substr(0, end + 1);

	std::size_t commentPos = s_trimmed.find("//");
	
	// std::cout << s << std::endl;
	// std::cout << s_trimmed.substr(0, commentPos) << std::endl;
	
    if (commentPos != std::string::npos) {
        return s_trimmed.substr(0, commentPos);
    }
    return s_trimmed;
}
