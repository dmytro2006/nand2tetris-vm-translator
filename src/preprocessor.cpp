#include "preprocessor.h"

std::string Preprocessor::preprocess(const std::string &input) {
    std::string output;
    std::string buffer;
    std::stringstream stream(input);
    while (!stream.eof()) {
        std::getline(stream, buffer);
        if (buffer.empty())
            continue;
        output += buffer.substr(0, buffer.find("//")) + '\n';
    }
    return output;
}
