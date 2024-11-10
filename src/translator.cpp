#include <bits/stdc++.h>
#include "preprocessor.h"
#include "tokenizer.h"
#include "file.h"
#include "compiler.h"

using namespace std;

string callInit = "@Sys.init.return\n"
                  "D=A\n"
                  "@SP\n"
                  "M=M+1\n"
                  "A=M-1\n"
                  "M=D\n"
                  "@LCL\n"
                  "D=M\n"
                  "@SP\n"
                  "M=M+1\n"
                  "A=M-1\n"
                  "M=D\n"
                  "@ARG\n"
                  "D=M\n"
                  "@SP\n"
                  "M=M+1\n"
                  "A=M-1\n"
                  "M=D\n"
                  "@THIS\n"
                  "D=M\n"
                  "@SP\n"
                  "M=M+1\n"
                  "A=M-1\n"
                  "M=D\n"
                  "@THAT\n"
                  "D=M\n"
                  "@SP\n"
                  "M=M+1\n"
                  "A=M-1\n"
                  "M=D\n"
                  "@5\n"
                  "D=A\n"
                  "@SP\n"
                  "D=M-D\n"
                  "@ARG\n"
                  "M=D\n"
                  "@SP\n"
                  "D=M\n"
                  "@LCL\n"
                  "M=D\n"
                  "@Sys.init\n"
                  "0;JMP\n"
                  "(Sys.init.return)\n";

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "ERROR: Usage\ntranslator.exe <file>" << endl;
        return 1;
    }
    string output_str;
    if (filesystem::is_directory(string(argv[1])))
    {
        output_str = "@256\n"
                     "D=A\n"
                     "@SP\n"
                     "M=D\n" +
                     callInit;
        for (const auto &file : filesystem::directory_iterator(string(argv[1])))
        {
            if (file.path().extension() == ".vm")
            {
                File input(file.path().string(), file_type::INPUT);
                if (!input.open())
                {
                    cerr << "ERROR while opening input file " << file.path().string() << endl;
                    return 1;
                };
                Preprocessor preprocessor(input.read());
                Tokenizer tokenizer(preprocessor.preprocess());
                tokenizer.tokenize();
                string className = input.getNameWithoutExtension();
                Compiler compiler(tokenizer.get_tokens(), className);
                int compilerExitCode = compiler.compile();
                if (compilerExitCode)
                    return 0;
                output_str += compiler.getOutput();
            }
        }
        string outputName = string(argv[1]);
        if (outputName.ends_with("/") || outputName.ends_with("\\"))
            outputName = outputName.substr(0, outputName.find_last_of("/\\"));
        outputName = outputName.substr(outputName.find_last_of("\\/") + 1);
        File output(outputName + ".asm", file_type::OUTPUT);
        if (!output.open())
        {
            cerr << "ERROR while opening output file" << endl;
            return 1;
        };
        output.write(output_str);
    }
    else
    {
        File input(string(argv[1]), file_type::INPUT);
        if (!input.open())
        {
            cerr << "ERROR while opening input file" << endl;
            return 1;
        };
        Preprocessor preprocessor(input.read());
        Tokenizer tokenizer(preprocessor.preprocess());
        tokenizer.tokenize();
        string className = input.getNameWithoutExtension();
        Compiler compiler(tokenizer.get_tokens(), className);
        int compilerExitCode = compiler.compile();
        if (compilerExitCode)
            return 0;
        File output(className + ".asm", file_type::OUTPUT);
        if (!output.open())
        {
            cerr << "ERROR while opening output file" << endl;
            return 1;
        };
        output_str += compiler.getOutput();
        output.write(output_str);
    }
    return 0;
}