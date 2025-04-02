#pragma once
#include <bits/stdc++.h>

/**
 * Generates tokens from the preprocessed source code
 */
class Tokenizer {
public:
    enum class TOKEN_TYPE {
        COMMAND,
        OPERATOR,
        SEGMENT,
        INT_LITERAL,
        STRING_LITERAL,
        LABEL,
        GOTO,
        IF_GOTO,
        FUNCTION,
        CALL,
        RETURN,
    };

    enum class TOKEN_NAME {
        PUSH,
        POP,
        ADD,
        SUB,
        NEG,
        EQ,
        GT,
        LT,
        AND,
        OR,
        NOT,
        LOCAL,
        ARGUMENT,
        STATIC,
        CONSTANT,
        THIS,
        THAT,
        POINTER,
        TEMP,
    };

    struct Token {
        TOKEN_TYPE type;
        TOKEN_NAME name;
        std::string value_str;
        int value_int;

        Token(const TOKEN_TYPE type, const TOKEN_NAME name) : type(type), name(name), value_int() {
        };

        Token(const TOKEN_TYPE type): type(type), name(), value_int(0) {
        };

        Token(const TOKEN_TYPE type, const std::string &value) : type(type), name(), value_str(value), value_int(0) {
        };

        Token(const TOKEN_TYPE type, const int value) : type(type), name(), value_int(value) {
        };
    };

    const std::map<std::string, Token> keywords = {
        {
            "push",
            {TOKEN_TYPE::COMMAND, TOKEN_NAME::PUSH}
        },
        {
            "pop",
            {TOKEN_TYPE::COMMAND, TOKEN_NAME::POP}
        },
        {
            "add",
            {TOKEN_TYPE::OPERATOR, TOKEN_NAME::ADD}
        },
        {
            "sub",
            {TOKEN_TYPE::OPERATOR, TOKEN_NAME::SUB}
        },
        {
            "neg",
            {TOKEN_TYPE::OPERATOR, TOKEN_NAME::NEG}
        },
        {
            "eq",
            {TOKEN_TYPE::OPERATOR, TOKEN_NAME::EQ}
        },
        {
            "gt",
            {TOKEN_TYPE::OPERATOR, TOKEN_NAME::GT}
        },
        {
            "lt",
            {TOKEN_TYPE::OPERATOR, TOKEN_NAME::LT}
        },
        {
            "and",
            {TOKEN_TYPE::OPERATOR, TOKEN_NAME::AND}
        },
        {
            "or",
            {TOKEN_TYPE::OPERATOR, TOKEN_NAME::OR}
        },
        {
            "not",
            {TOKEN_TYPE::OPERATOR, TOKEN_NAME::NOT}
        },
        {
            "local",
            {TOKEN_TYPE::SEGMENT, TOKEN_NAME::LOCAL}
        },
        {
            "argument",
            {TOKEN_TYPE::SEGMENT, TOKEN_NAME::ARGUMENT}
        },
        {
            "static",
            {TOKEN_TYPE::SEGMENT, TOKEN_NAME::STATIC}
        },
        {
            "constant",
            {TOKEN_TYPE::SEGMENT, TOKEN_NAME::CONSTANT}
        },
        {
            "this",
            {TOKEN_TYPE::SEGMENT, TOKEN_NAME::THIS}
        },
        {
            "that",
            {TOKEN_TYPE::SEGMENT, TOKEN_NAME::THAT}
        },
        {
            "pointer",
            {TOKEN_TYPE::SEGMENT, TOKEN_NAME::POINTER}
        },
        {
            "temp",
            {TOKEN_TYPE::SEGMENT, TOKEN_NAME::TEMP}
        },
        {
            "label",
            {TOKEN_TYPE::LABEL}
        },
        {
            "goto",
            {TOKEN_TYPE::GOTO}
        },
        {
            "if-goto",
            {TOKEN_TYPE::IF_GOTO}
        },
        {
            "call",
            {TOKEN_TYPE::CALL}
        },
        {
            "function",
            {TOKEN_TYPE::FUNCTION}
        },
        {
            "return",
            {TOKEN_TYPE::RETURN}
        },
    };

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
