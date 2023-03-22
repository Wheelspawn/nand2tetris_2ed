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

using namespace std;

class Parser {
	public:
		Parser(const string& inFn, const string& outFn);
		~Parser();
		bool hasMoreLines();
		string advance();
		CommandType commandType();
	private:
		ifstream input_filestream;
		ofstream output_filestream;
		string currentLine;
};

#endif // PARSER_H