#include "tokenizer.h"
#include <sstream>

Tokenizer::Tokenizer(const std::string &code) {
    this->code = code;
}

void Tokenizer::tokenize() {
    std::stringstream stream(code);
    std::string token;
    while (stream >> token) {
        //if the token is a keyword
        if (keywords.contains(token)) {
            tokens.push_back(keywords.at(token));
            continue;
        }
        //if the token isn't a keyword
        if (const int val = convertStrToInt(token); val == -1) {
            tokens.push_back({TOKEN_TYPE::STRING_LITERAL, token});
        } else {
            tokens.push_back({TOKEN_TYPE::INT_LITERAL, token});
        }
    }
}


std::vector<Token> &Tokenizer::get_tokens() {
    return this->tokens;
}

int Tokenizer::convertStrToInt(const std::string &str) {
    for (const auto ch: str) {
        if (!isInt(ch))
            return -1;
    }
    return stoi(str);
}

inline bool Tokenizer::isInt(const char ch) {
    return ch >= '0' && ch <= '9';
}
