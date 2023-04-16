#ifndef CODEWRITER_H
#define CODEWRITER_H

#include <fstream>
#include <string>

#include "parser.h"

class CodeWriter {
	public:
		CodeWriter(const std::string& inputFileName);
		~CodeWriter();
		void writeArithmetic(std::string& command);
		void writePushPop(CommandType command, const std::string& segment, int idx);
	private:
		std::ofstream output_filestream;
};

#endif // CODEWRITER_H