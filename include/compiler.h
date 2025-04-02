#pragma once
#include <bits/stdc++.h>
#include "tokenizer.h"

class Compiler
{
public:
    Compiler(std::vector<Tokenizer::token> tokens, std::string className);
    int compile();
    std::string &getOutput();
    const std::map<Tokenizer::value, std::string (Compiler::*)()> operator_code{
        {Tokenizer::value::ADD, Compiler::parseAdd},
        {Tokenizer::value::SUB, Compiler::parseSub},
        {Tokenizer::value::NEG, Compiler::parseNeg},
        {Tokenizer::value::EQ, Compiler::parseEq},
        {Tokenizer::value::GT, Compiler::parseGt},
        {Tokenizer::value::LT, Compiler::parseLt},
        {Tokenizer::value::AND, Compiler::parseAnd},
        {Tokenizer::value::OR, Compiler::parseOr},
        {Tokenizer::value::NOT, Compiler::parseNot}};

private:
    std::string command(Tokenizer::value token_val, Tokenizer::value segment, int index);
    std::string oper(Tokenizer::value token_val);
    std::vector<Tokenizer::token> tokens;
    std::string className;
    std::string output;
    inline std::string parseAdd();
    inline std::string parseSub();
    inline std::string parseNeg();
    std::string parseEq();
    std::string parseGt();
    std::string parseLt();
    std::string parseAnd();
    inline std::string parseOr();
    std::string parseNot();
    std::string parsePush(Tokenizer::value segment, int index);
    std::string parsePop(Tokenizer::value segment, int index);
    std::string parseLabel(std::string label);
    std::string parseGoto(std::string label);
    std::string parseIfGoto(std::string label);
    std::string parseFunction(std::string name, int nVars);
    std::string parseCall(std::string name, int nArgs);
    std::string parseReturn();
};