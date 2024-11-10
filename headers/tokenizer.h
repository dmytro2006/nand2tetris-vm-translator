#pragma once
#include <bits/stdc++.h>

class Tokenizer
{
public:
    enum class token_type
    {
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

    enum class value
    {
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

    struct token
    {
        token_type type;
        value v;
        std::string val_str;
        int val_int;
    };

    const std::map<std::string, token> m{
        {"push",
         {token_type::COMMAND, value::PUSH}},
        {"pop",
         {token_type::COMMAND, value::POP}},
        {"add",
         {token_type::OPERATOR, value::ADD}},
        {"sub",
         {token_type::OPERATOR, value::SUB}},
        {"neg",
         {token_type::OPERATOR, value::NEG}},
        {"eq",
         {token_type::OPERATOR, value::EQ}},
        {"gt",
         {token_type::OPERATOR, value::GT}},
        {"lt",
         {token_type::OPERATOR, value::LT}},
        {"and",
         {token_type::OPERATOR, value::AND}},
        {"or",
         {token_type::OPERATOR, value::OR}},
        {"not",
         {token_type::OPERATOR, value::NOT}},
        {"local",
         {token_type::SEGMENT, value::LOCAL}},
        {"argument",
         {token_type::SEGMENT, value::ARGUMENT}},
        {"static",
         {token_type::SEGMENT, value::STATIC}},
        {"constant",
         {token_type::SEGMENT, value::CONSTANT}},
        {"this",
         {token_type::SEGMENT, value::THIS}},
        {"that",
         {token_type::SEGMENT, value::THAT}},
        {"pointer",
         {token_type::SEGMENT, value::POINTER}},
        {"temp",
         {token_type::SEGMENT, value::TEMP}},
        {"label",
         {token_type::LABEL}},
        {"goto",
         {token_type::GOTO}},
        {"if-goto",
         {token_type::IF_GOTO}},
        {"call",
         {token_type::CALL}},
        {"function",
         {token_type::FUNCTION}},
        {"return",
         {token_type::RETURN}},
    };

    Tokenizer(std::string code);
    int tokenize();
    std::vector<token> &get_tokens();

private:
    std::string code;
    std::vector<token> tokens;
    int convertStrToInt(std::string str);
    inline bool isInt(char ch);
};