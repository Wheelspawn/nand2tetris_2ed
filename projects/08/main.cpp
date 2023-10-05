#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>

#include "parser.h"
#include "code_writer.h"

int main(int argc, char *argv[]) {
        
    // input directory
    // a.out path/to/files/        -> files.asm
    
    // input single file
    // a.out path/to/files/file.vm    -> file.asm
    
    if (argc < 2) {
        std::cerr << "no input file specified\n";
        return 1;
    }
    
    if (argc > 2) {
        std::cerr << "too many arguments\n";
        return 1;
    }
    
    std::string input_file_name = argv[1];
    std::filesystem::path inputPath(input_file_name);
    std::string output_file_name;
    
    std::vector<std::string> files;
    
    if (std::filesystem::is_directory(inputPath)) {        // directory (multiple files)
        output_file_name = input_file_name + inputPath.parent_path().filename().string() + ".asm";
        
        for (const auto& entry : std::filesystem::directory_iterator(inputPath)) {
            if (entry.path().extension() == ".vm") {
                files.push_back(entry.path());
            }
        }
    }
    else if (inputPath.extension() == ".vm") {            // single file
        output_file_name = input_file_name.substr(0,input_file_name.size()-3) +".asm";
        files.push_back(input_file_name);
    }
    else {
        std::cerr << "invalid input format\n" << std::endl;
        return 1;
    }
    
    /*
    std::cout << files.size() << std::endl;
    for (int i = 0; i < files.size(); i++) {
        std::cout << files[i] << std::endl;
    } */
    
    // std::cout << "\n" << input_file_name << std::endl;
    // std::cout << output_file_name << std::endl;

    Parser parser(input_file_name);
    CodeWriter codewriter(output_file_name);

    for (int i = 0; i < files.size(); i++)
    {
        codewriter.resetInputFileName(files[i]);

        if (files.size() > 1)
        {
            parser.resetInputFileName(files[i]);
            if (i == 0)
            {
                codewriter.writeInit();
            }
        }
    
        // print out command type, first argument, second argument
        while (parser.hasMoreLines()) {
            parser.advance();

            if (parser.commandType() == C_ARITHMETIC)
            {
                codewriter.writeArithmetic(parser.arg0());
            }
            else if ((parser.commandType() == C_PUSH) || (parser.commandType() == C_POP))
            {
                codewriter.writePushPop(parser.commandType(), parser.arg1(), parser.arg2());
            }
            else if (parser.commandType() == C_LABEL)
            {
                codewriter.writeLabel(parser.arg1());
            }
            else if (parser.commandType() == C_GOTO)
            {
                codewriter.writeGoto(parser.arg1());
            }
            else if (parser.commandType() == C_IF)
            {
                codewriter.writeIf(parser.arg1());
            }
            else if (parser.commandType() == C_FUNCTION)
            {
                codewriter.writeFunction(parser.arg1(), parser.arg2());
            }
            else if (parser.commandType() == C_CALL)
            {
                codewriter.writeCall(parser.arg1(), parser.arg2());
            }
            else if (parser.commandType() == C_RETURN)
            {
                codewriter.writeReturn();
            }
        }
    }
    
    return 0;
}

