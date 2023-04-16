#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <string>

enum CommandType {
	C_ARITHMETIC,
	C_PUSH,
	C_POP,
	C_LABEL,
	C_GOTO,
	C_IF,
	C_FUNCTION,
	C_RETURN,
	C_CALL,
	C_UNKNOWN
    };

class Parser {
	public:
		Parser(const std::string& inFn, const std::string& outFn);
		~Parser();
		bool hasMoreLines();
		std::string advance();
		CommandType commandType();
		std::string arg1();
		std::string arg2();
		void args();
		std::string trim(const std::string& s);
	private:
		std::ifstream input_filestream;
		std::string currentLine;
};

#endif // PARSER_H