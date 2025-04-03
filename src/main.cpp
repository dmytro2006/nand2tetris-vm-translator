#include <bits/stdc++.h>
#include "preprocessor.h"
#include "tokenizer.h"
#include "file.h"
#include "compiler.h"

using namespace std;


int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "ERROR: Usage\ntranslator.exe <file>" << endl;
        return 1;
    }
    if (filesystem::is_directory(string(argv[1]))) {
        string output_str = Compiler::initialization_code;
        for (const auto &file: filesystem::directory_iterator(string(argv[1]))) {
            if (file.path().extension() == ".vm") {
                File input(file.path(), file_type::INPUT);
                if (!input.open()) {
                    cerr << "ERROR while opening input file " << file.path().string() << endl;
                    return 1;
                }
                Tokenizer tokenizer(Preprocessor::preprocess(input.read()));
                tokenizer.tokenize();
                string className = input.getNameWithoutExtension();
                Compiler compiler(tokenizer.get_tokens(), className);
                if (compiler.compile())
                    return 0;
                output_str += compiler.getOutput();
            }
        }
        auto outputName = string(argv[1]);
        if (outputName.ends_with("/") || outputName.ends_with("\\"))
            outputName = outputName.substr(0, outputName.find_last_of("/\\"));
        outputName = outputName.substr(outputName.find_last_of("\\/") + 1);
        File output(outputName + ".asm", file_type::OUTPUT);
        if (!output.open()) {
            cerr << "ERROR while opening output file" << endl;
            return 1;
        }
        output.write(output_str);
    } else {
        File input(string(argv[1]), file_type::INPUT);
        if (!input.open()) {
            cerr << "ERROR while opening input file" << endl;
            return 1;
        }
        Tokenizer tokenizer(Preprocessor::preprocess(input.read()));
        tokenizer.tokenize();
        string className = input.getNameWithoutExtension();
        Compiler compiler(tokenizer.get_tokens(), className);
        if (compiler.compile())
            return 0;
        File output(className + ".asm", file_type::OUTPUT);
        if (!output.open()) {
            cerr << "ERROR while opening output file" << endl;
            return 1;
        }
        output.write(compiler.getOutput());
    }
    return 0;
}
