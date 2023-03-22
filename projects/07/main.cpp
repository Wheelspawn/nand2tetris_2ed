#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include "parser.h"
// #include "code_writer.h"

using namespace std;

int main(int argc, char *argv[]) {
    
    if (argc < 2) {
        std::cerr << "no input file specified\n";
        return 1;
    }

    string input_file_name = argv[1];
    string output_file_name = input_file_name +".vm";
	
    Parser parser(input_file_name, output_file_name);
	
	while (parser.hasMoreLines()) {
		cout << parser.advance() << endl;
		cout << parser.commandType() << endl;
	}
	
	return 0;
}
