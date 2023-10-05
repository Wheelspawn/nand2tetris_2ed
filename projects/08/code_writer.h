#ifndef CODEWRITER_H
#define CODEWRITER_H

#include <fstream>
#include <string>

#include "parser.h"

class CodeWriter {
	public:
		CodeWriter(const std::string& outputFileName);
		~CodeWriter();
        void resetInputFileName(const std::string& inFn);
		void writeArithmetic(const std::string& command);
		void writePushPop(CommandType command, const std::string& segment, int idx);
		void writeLabel(const std::string& label);
		void writeGoto(const std::string& label);
		void writeIf(const std::string& label);
		void writeFunction(const std::string& functionName, int nVars);
		void writeCall(const std::string& functionName, int nArgs);
		void writeReturn();
		void writeInit();
	private:
		std::ofstream output_filestream;
		std::string filename;
		
		int eq_counter;
		int gt_counter;
		int lt_counter;
		int call_counter;
};

#endif // CODEWRITER_H
