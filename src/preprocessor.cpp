#include "preprocessor.h"

Preprocessor::Preprocessor(const std::string &code)
{
    this->code = code;
}

std::string Preprocessor::preprocess()
{
    std::string output;
    std::string buffer;
    std::stringstream stream(this->code);
    while (!stream.eof())
    {
        std::getline(stream, buffer);
        if (buffer.empty())
            continue;
        output += buffer.substr(0, buffer.find("//")) + '\n';
    }
    return output;
}