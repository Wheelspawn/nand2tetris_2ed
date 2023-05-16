#ifndef CODEWRITER_H
#define CODEWRITER_H

#include <fstream>
#include <string>

#include "parser.h"

class CodeWriter {
	public:
		CodeWriter(const std::string& outputFileName);
		~CodeWriter();
		void writeArithmetic(const std::string& command);
		void writePushPop(CommandType command, const std::string& segment, int idx);
	private:
		std::ofstream output_filestream;
		int eq_counter;
		int gt_counter;
};

#endif // CODEWRITER_H