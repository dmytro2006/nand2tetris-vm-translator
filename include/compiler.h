#pragma once
#include <bits/stdc++.h>
#include "tokenizer.h"

/**
 * Generates assembler code from the tokens
 */
class Compiler {
public:
    // Code for system initialization and calling Sys.init()
    static std::string initialization_code;

    Compiler(const std::vector<Token> &tokens, const std::string &className);

    int compile();

    std::string &getOutput();

    const std::map<TOKEN_NAME, std::string(Compiler::*)() const> operator_code{
        {TOKEN_NAME::ADD, Compiler::parseAdd},
        {TOKEN_NAME::SUB, Compiler::parseSub},
        {TOKEN_NAME::NEG, Compiler::parseNeg},
        {TOKEN_NAME::EQ, Compiler::parseEq},
        {TOKEN_NAME::GT, Compiler::parseGt},
        {TOKEN_NAME::LT, Compiler::parseLt},
        {TOKEN_NAME::AND, Compiler::parseAnd},
        {TOKEN_NAME::OR, Compiler::parseOr},
        {TOKEN_NAME::NOT, Compiler::parseNot}
    };

private:
    [[nodiscard]] std::string command(TOKEN_NAME token_val, TOKEN_NAME segment, int index) const;

    std::string oper(TOKEN_NAME token_val);

    std::vector<Token> tokens;
    std::string className;
    std::string output;

    [[nodiscard]] std::string parseAdd() const;

    [[nodiscard]] std::string parseSub() const;

    [[nodiscard]] std::string parseNeg() const;

    [[nodiscard]] std::string parseEq() const;

    [[nodiscard]] std::string parseGt() const;

    [[nodiscard]] std::string parseLt() const;

    [[nodiscard]] std::string parseAnd() const;

    [[nodiscard]] std::string parseOr() const;

    [[nodiscard]] std::string parseNot() const;

    [[nodiscard]] std::string parsePush(TOKEN_NAME segment, int index) const;

    [[nodiscard]] std::string parsePop(TOKEN_NAME segment, int index) const;

    [[nodiscard]] std::string parseLabel(const std::string &label) const;

    [[nodiscard]] std::string parseGoto(const std::string &label) const;

    [[nodiscard]] std::string parseIfGoto(const std::string &label) const;

    [[nodiscard]] std::string parseFunction(const std::string &name, int nVars) const;

    [[nodiscard]] std::string parseCall(const std::string &name, int nArgs) const;

    [[nodiscard]] std::string parseReturn() const;
};
