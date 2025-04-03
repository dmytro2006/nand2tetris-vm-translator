//
// Created by dmytro2006 on 03/04/2025.
//

#ifndef KEYWORDS_H
#define KEYWORDS_H

#include <map>
#include "token.h"

const std::map< std::string,  Token> keywords = {
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

#endif //KEYWORDS_H
