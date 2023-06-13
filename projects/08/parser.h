#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <string>

enum CommandType {
	C_ARITHMETIC,	// 0
	C_PUSH,			// 1
	C_POP,
	C_LABEL,
	C_GOTO,
	C_IF,
	C_FUNCTION,
	C_RETURN,
	C_CALL,
	C_UNKNOWN		// 9
    };

class Parser {
	public:
		Parser(const std::string& inFn);
		~Parser();
		bool hasMoreLines();
		std::string advance();
		CommandType commandType();
		std::string arg0();
		std::string arg1();
		int arg2();
		void args();
		std::string trim(const std::string& s);
	private:
		void getArgs();
		std::ifstream input_filestream;
		std::string currentLine;
		std::string arg_0;
		std::string arg_1;
		std::string arg_2;
};

#endif // PARSER_H