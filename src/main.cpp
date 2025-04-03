#include "translator.h"
#include "file.h"
#include <iostream>

using namespace std;


int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "ERROR: Usage\ntranslator.exe <file|directory>" << endl;
        return 1;
    }
    // If argument is a directory, translate all files
    if (filesystem::is_directory(string(argv[1]))) {
        string output_str = Translator::initialization_code; // If argument is a directory, system library is needed
        // and initialization code is inserted in the output

        // Iterate through all files
        for (const auto &file: filesystem::directory_iterator(string(argv[1]))) {
            if (file.path().extension() != ".vm") continue; // If not vm code, skip

            File input(file.path(), file_type::INPUT);
            if (!input.open()) {
                cerr << "ERROR while opening input file " << file.path().string() << endl;
                return 1;
            }

            Translator translator(input.read(), input.get_name_without_extension());
            if (translator.preprocess()) { return 1; }
            if (translator.tokenize()) { return 1; }
            if (translator.translate()) { return 1; }
            output_str += translator.get_output(); // Append translated file to the output
        }

        // Extract directory name from the path
        auto outputName = string(argv[1]);
        if (outputName.ends_with("/") || outputName.ends_with("\\"))
            outputName = outputName.substr(0, outputName.find_last_of("/\\"));
        outputName = outputName.substr(outputName.find_last_of("\\/") + 1);

        // Write output to <directory name>.asm
        File output(outputName + ".asm", file_type::OUTPUT);
        if (!output.open()) {
            cerr << "ERROR while opening output file" << endl;
            return 1;
        }
        output.write(output_str);

        return 0;
    }

    // If input is a file, translate the file
    File input(string(argv[1]), file_type::INPUT);
    if (!input.open()) {
        cerr << "ERROR while opening input file" << endl;
        return 1;
    }

    Translator translator(input.read(), input.get_name_without_extension());
    if (translator.preprocess()) { return 1; }
    if (translator.tokenize()) { return 1; }
    if (translator.translate()) { return 1; }

    // Write output to <file name>.asm
    File output(translator.get_class_name() + ".asm", file_type::OUTPUT);
    if (!output.open()) {
        cerr << "ERROR while opening output file" << endl;
        return 1;
    }
    output.write(translator.get_output());

    return 0;
}
