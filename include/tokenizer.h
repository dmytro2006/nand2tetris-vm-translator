#pragma once
#include <string>
#include <map>
#include <vector>
#include "token.h"

/**
 * Generates tokens from the preprocessed source code
 */
class Tokenizer {
public:


    Tokenizer(const std::string &code);

    /* Tokenizes the source code */
    void tokenize();

    /* Returns vector of tokens */
    std::vector<Token> &get_tokens();

private:
    std::string code;
    std::vector<Token> tokens;

    int convertStrToInt(const std::string &str);

    inline bool isInt(char ch);
};
