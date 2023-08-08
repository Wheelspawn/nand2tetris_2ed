#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include "parser.h"

Parser::Parser(const std::string& inFn) {
	// constructor
	arg_0 = "";
	arg_1 = "";
	arg_2 = "";
	input_filestream.open(inFn);
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
		getArgs();
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

	if ((arg_0 == "add") || (arg_0 == "sub") || (arg_0 == "neg") ||
		(arg_0 == "eq") || (arg_0 == "gt") || (arg_0 == "lt") || (arg_0 == "and")
		|| (arg_0 == "or") || (arg_0 == "not")) {
		return C_ARITHMETIC;
	} else if (arg_0 == "push") {
		return C_PUSH;
	} else if (arg_0 == "pop") {
		return C_POP;
	} else if (arg_0 == "label") {
		return C_LABEL;
	} else if (arg_0 == "goto") {
		return C_GOTO;
	} else if (arg_0 == "if-goto") {
		return C_IF;
	} else if (arg_0 == "function") {
		return C_FUNCTION;
	} else if (arg_0 == "call") {
		return C_CALL;
	} else if (arg_0 == "return") {
		return C_RETURN;
	} else {
		return C_UNKNOWN;
	}
}

std::string Parser::arg0() {
	return arg_0;
}

std::string Parser::arg1() {
	return arg_1;
}

int Parser::arg2() {
	int r;
	
	std::stringstream ss; 
	ss << arg_2;
	ss >> r;
	
	return r;
}

void Parser::getArgs() {
	// (^\s*([a-zA-Z0-9_]*)(\s+)?(\S+)?)
	// (^\s*(\S+)(\s+)?(\S+)?)")
	std::regex arg_regex(R"(^(\S+)(\s+)?(\S+)?(\s+)?(\d+)?)");
	std::smatch match;

	if (std::regex_search(currentLine, match, arg_regex) && match.size() > 0) {
		arg_0 = match[1];
		arg_1 = match[3];
		arg_2 = match[5];
	}
	else
	{
		arg_0 = "";
		arg_1 = "";
		arg_2 = "";
	}
}

std::string Parser::trim(const std::string& s) {
    std::size_t end = s.find_last_not_of(" \r\n\t\f\v");
    std::string s_trimmed = (end == std::string::npos) ? "" : s.substr(0, end + 1);

	std::size_t commentPos = s_trimmed.find("//");
	
    if (commentPos != std::string::npos) {
        return s_trimmed.substr(0, commentPos);
    }
    return s_trimmed;
}
