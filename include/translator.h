//
// translator.h
// All functions for translating vm code to assembler
// Created by dmytro2006 on 03/04/2025.
//

#ifndef TRANSLATOR_H
#define TRANSLATOR_H
#include <string>
#include <token.h>
#include <vector>
#include <map>

class Translator {
public:
    static std::string initialization_code;

    Translator(const std::string &code, const std::string& class_name): input_code(code), class_name(class_name) {
    };

    ~Translator() = default;

    // Removes comments and blank lines. Returns non-zero value in case of errors
    int preprocess();

    // Generates tokens from the preprocessed source code
    int tokenize();

    // Generates assembler code from tokens
    int translate();

    std::vector<Token> &get_tokens();

    std::string &get_output();

    std::string &get_class_name();

private:
    std::string input_code;
    std::vector<Token> tokens;
    std::string output_code;
    std::string class_name;

    bool is_int(const std::string &str);

    // Parsing functions
    [[nodiscard]] std::string command(TOKEN_NAME name, TOKEN_NAME segment, const std::string &index) const;

    std::string parseOperator(TOKEN_NAME name);

    [[nodiscard]] std::string parseAdd() const;

    [[nodiscard]] std::string parseSub() const;

    [[nodiscard]] std::string parseNeg() const;

    [[nodiscard]] std::string parseEq() const;

    [[nodiscard]] std::string parseGt() const;

    [[nodiscard]] std::string parseLt() const;

    [[nodiscard]] std::string parseAnd() const;

    [[nodiscard]] std::string parseOr() const;

    [[nodiscard]] std::string parseNot() const;

    [[nodiscard]] std::string parsePush(TOKEN_NAME segment, const std::string &index) const;

    [[nodiscard]] std::string parsePop(TOKEN_NAME segment, const std::string &index) const;

    [[nodiscard]] std::string parseLabel(const std::string &label) const;

    [[nodiscard]] std::string parseGoto(const std::string &label) const;

    [[nodiscard]] std::string parseIfGoto(const std::string &label) const;

    [[nodiscard]] std::string parseFunction(const std::string &name, int nVars) const;

    [[nodiscard]] std::string parseCall(const std::string &name, int nArgs) const;

    [[nodiscard]] std::string parseReturn() const;

    const std::map<TOKEN_NAME, std::string(Translator::*)() const> operator_code{
            {TOKEN_NAME::ADD, Translator::parseAdd},
            {TOKEN_NAME::SUB, Translator::parseSub},
            {TOKEN_NAME::NEG, Translator::parseNeg},
            {TOKEN_NAME::EQ, Translator::parseEq},
            {TOKEN_NAME::GT, Translator::parseGt},
            {TOKEN_NAME::LT, Translator::parseLt},
            {TOKEN_NAME::AND, Translator::parseAnd},
            {TOKEN_NAME::OR, Translator::parseOr},
            {TOKEN_NAME::NOT, Translator::parseNot}
    };
    
};

#endif //TRANSLATOR_H
