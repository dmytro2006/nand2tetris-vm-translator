//
// token.h
// Token structure fot the translator
// Created by dmytro2006 on 03/04/2025.
//

#ifndef TOKEN_H
#define TOKEN_H
#include <string>

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

#endif //TOKEN_H
