//
// Created by dmytro2006 on 03/04/2025.
//
#include "translator.h"
#include "keywords.h"
#include "initialization.h"
#include <sstream>
#include <iostream>

std::string &Translator::get_class_name() {
    return class_name;
}

std::string &Translator::get_output() {
    return output_code;
}

std::vector<Token> &Translator::get_tokens() {
    return tokens;
}

// Removes comments and blank lines. Returns non-zero value in case of errors
int Translator::preprocess() {
    std::string output;
    std::string buffer;
    std::stringstream stream(input_code);
    while (!stream.eof()) {
        std::getline(stream, buffer);
        if (buffer.empty())
            continue;
        output += buffer.substr(0, buffer.find("//")) + '\n'; // Remove comments
    }
    input_code = output;
    return 0;
}

bool Translator::isInt(const std::string &str) {
    for (const auto ch: str) {
        if (!(ch >= '0' && ch <= '9'))
            return false;
    }
    return true;
}

int Translator::tokenize() {
    std::stringstream stream(input_code);
    std::string token;
    while (stream >> token) {
        //if the token is a keyword
        if (keywords.contains(token)) {
            tokens.push_back(keywords.at(token));
            continue;
        }
        //if the token isn't a keyword
        if (isInt(token)) {
            tokens.push_back({TOKEN_TYPE::STRING_LITERAL, token});
        } else {
            tokens.push_back({TOKEN_TYPE::INT_LITERAL, token});
        }
    }
    return 0;
}

int Translator::translate() {
    for (int i = 0; i < tokens.size(); i++) {
        switch (tokens[i].type) {
            case TOKEN_TYPE::COMMAND:
                if (i + 2 >= tokens.size()) {
                    std::cerr << "ERROR: Not enough arguments for " << (tokens[i].name == TOKEN_NAME::PUSH
                                                                            ? "PUSH"
                                                                            : "POP") << std::endl;
                    return 1;
                }
                if (tokens[i + 1].type == TOKEN_TYPE::SEGMENT && tokens[i + 2].type ==
                    TOKEN_TYPE::INT_LITERAL)
                    output_code += command(tokens[i].name, tokens[i + 1].name, tokens[i + 2].value);
                else {
                    std::cerr << "ERROR: Invalid arguments for " << (tokens[i].name == TOKEN_NAME::PUSH
                                                                         ? "PUSH"
                                                                         : "POP") << std::endl;
                    return 1;
                }
                break;
            case TOKEN_TYPE::OPERATOR:
                output_code += parseOperator(tokens[i].name);
                break;
            case TOKEN_TYPE::SEGMENT:
            case TOKEN_TYPE::INT_LITERAL:
            case TOKEN_TYPE::STRING_LITERAL:
                break;
            case TOKEN_TYPE::LABEL:
                if (i + 1 >= tokens.size()) {
                    std::cerr << "ERROR: Not enough arguments for LABEL" << std::endl;
                    return 1;
                }
                if (tokens[i + 1].type == TOKEN_TYPE::STRING_LITERAL)
                    output_code += parseLabel(tokens[i + 1].value);
                else {
                    std::cerr << "ERROR: Invalid arguments for LABEL" << std::endl;
                    return 1;
                }
                break;
            case TOKEN_TYPE::GOTO:
                if (i + 1 >= tokens.size()) {
                    std::cerr << "ERROR: Not enough arguments for GOTO" << std::endl;
                    return 1;
                }
                if (tokens[i + 1].type == TOKEN_TYPE::STRING_LITERAL)
                    output_code += parseGoto(tokens[i + 1].value);
                else {
                    std::cerr << "ERROR: Invalid arguments for GOTO" << std::endl;
                    return 1;
                }
                break;
            case TOKEN_TYPE::IF_GOTO:
                if (i + 1 >= tokens.size()) {
                    std::cerr << "ERROR: Not enough arguments for IF-GOTO" << std::endl;
                    return 1;
                }
                if (tokens[i + 1].type == TOKEN_TYPE::STRING_LITERAL)
                    output_code += parseIfGoto(tokens[i + 1].value);
                else {
                    std::cerr << "ERROR: Invalid arguments for IF-GOTO" << std::endl;
                    return 1;
                }
                break;
            case TOKEN_TYPE::FUNCTION:
                if (i + 2 >= tokens.size()) {
                    std::cerr << "ERROR: Not enough arguments for FUNCTION" << std::endl;
                    return 1;
                }
                if (tokens[i + 1].type == TOKEN_TYPE::STRING_LITERAL && tokens[i + 2].type ==
                    TOKEN_TYPE::INT_LITERAL)
                    output_code += parseFunction(tokens[i + 1].value, stoi(tokens[i + 2].value));
                else {
                    std::cerr << "ERROR: Invalid arguments for FUNCTION" << std::endl;
                    return 1;
                }
                break;
            case TOKEN_TYPE::CALL:
                if (i + 2 >= tokens.size()) {
                    std::cerr << "ERROR: Not enough arguments for CALL" << std::endl;
                    return 1;
                }
                if (tokens[i + 1].type == TOKEN_TYPE::STRING_LITERAL && tokens[i + 2].type ==
                    TOKEN_TYPE::INT_LITERAL)
                    output_code += parseCall(tokens[i + 1].value, stoi(tokens[i + 2].value));
                else {
                    std::cerr << "ERROR: Invalid arguments for CALL" << std::endl;
                    return 1;
                }
                break;
            case TOKEN_TYPE::RETURN:
                output_code += parseReturn();
                break;
        }
    }
    return 0;
}

std::string Translator::command(const TOKEN_NAME token_val, const TOKEN_NAME segment,
                              const std::string& index) const {
    switch (token_val) {
        case TOKEN_NAME::PUSH:
            return parsePush(segment, index);
        case TOKEN_NAME::POP:
            return parsePop(segment, index);
        default:
            return {};
    }
}

inline std::string Translator::parseOperator(const TOKEN_NAME token_val) {
    return (this->*operator_code.at(token_val))();
}

std::string Translator::parseAdd() const {
    return "@SP\n"
            "M=M-1\n"
            "A=M\n"
            "D=M\n"
            "A=A-1\n"
            "M=D+M\n";
}

std::string Translator::parseSub() const {
    return "@SP\n"
            "M=M-1\n"
            "A=M\n"
            "D=M\n"
            "A=A-1\n"
            "M=D-M\n"
            "M=-M\n";
}

std::string Translator::parseNeg() const {
    return "@SP\n"
            "A=M-1\n"
            "M=-M\n";
}

std::string Translator::parseEq() const {
    static int n;
    n++;
    return "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "D=M\n"
           "A=A-1\n"
           "D=D-M\n"
           "@EQ_" +
           class_name + "_" +
           std::to_string(n) +
           "\n"
           "D;JEQ\n"
           "D=0\n"
           "@END_EQ_" +
           class_name + "_" +
           std::to_string(n) +
           "\n"
           "0;JMP\n"
           "(EQ_" +
           class_name + "_" +
           std::to_string(n) +
           ")\n"
           "D=1\n"
           "(END_EQ_" +
           class_name + "_" +
           std::to_string(n) +
           ")\n"
           "@SP\n"
           "A=M-1\n"
           "M=D\n";
}

std::string Translator::parseGt() const {
    static int n;
    n++;
    return "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "D=M\n"
           "A=A-1\n"
           "D=D-M\n"
           "@GT" +
           class_name + "_" +
           std::to_string(n) +
           "\n"
           "D;JLT\n"
           "D=0\n"
           "@END_GT" +
           class_name + "_" +
           std::to_string(n) +
           "\n"
           "0;JMP\n"
           "(GT" +
           class_name + "_" +
           std::to_string(n) +
           ")\n"
           "D=1\n"
           "(END_GT" +
           class_name + "_" +
           std::to_string(n) +
           ")\n"
           "@SP\n"
           "A=M-1\n"
           "M=D\n";
}

std::string Translator::parseLt() const {
    static int n;
    n++;
    return "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "D=M\n"
           "A=A-1\n"
           "D=D-M\n"
           "@LT" +
           class_name + "_" +
           std::to_string(n) +
           "\n"
           "D;JGT\n"
           "D=0\n"
           "@END_LT" +
           class_name + "_" +
           std::to_string(n) +
           "\n"
           "0;JMP\n"
           "(LT" +
           class_name + "_" +
           std::to_string(n) +
           ")\n"
           "D=1\n"
           "(END_LT" +
           class_name + "_" +
           std::to_string(n) +
           ")\n"
           "@SP\n"
           "A=M-1\n"
           "M=D\n";
}

std::string Translator::parseAnd() const {
    static int n;
    n++;
    return "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "D=M\n"
           "@AND_" +
           class_name + "_" +
           std::to_string(n) +
           "\n"
           "D;JEQ\n"
           "@SP\n"
           "A=M\n"
           "D=M\n"
           "@AND_" +
           class_name + "_" +
           std::to_string(n) +
           "\n"
           "D;JEQ\n"
           "D=1\n"
           "@END_AND_" +
           class_name + "_" +
           std::to_string(n) +
           "\n"
           "(@AND_" +
           class_name + "_" +
           std::to_string(n) +
           ")\n"
           "D=0\n"
           "(END_AND_" +
           class_name + "_" +
           std::to_string(n) +
           ")\n"
           "@SP\n"
           "A=M-1\n"
           "M=D\n";
}

std::string Translator::parseOr() const {
    return "@SP\n"
            "M=M-1\n"
            "A=M\n"
            "D=M\n"
            "A=A-1\n"
            "M=D|M\n";
}

std::string Translator::parseNot() const {
    static int n;
    n++;
    return "@SP\n"
           "A=M-1\n"
           "D=M\n"
           "@NOT_" +
           class_name + "_" +
           std::to_string(n) +
           "\n"
           "D;JEQ\n"
           "D=0\n"
           "@END_NOT_" +
           class_name + "_" +
           std::to_string(n) +
           "\n"
           "0;JMP\n"
           "(NOT_" +
           class_name + "_" +
           std::to_string(n) +
           ")\n"
           "D=1\n"
           "(END_NOT_" +
           class_name + "_" +
           std::to_string(n) +
           ")\n"
           "@SP\n"
           "A=M-1\n"
           "M=D\n";
}

std::string Translator::parsePush(const TOKEN_NAME segment, const std::string& index) const {
    std::string output;
    switch (segment) {
        case TOKEN_NAME::CONSTANT:
            output += "@" + index +
                    '\n' +
                    "D=A\n";
            break;
        case TOKEN_NAME::LOCAL:
            output += "@" + index +
                    '\n' +
                    "D=A\n"
                    "@LCL\n"
                    "A=D+M\n"
                    "D=M\n";
            break;
        case TOKEN_NAME::ARGUMENT:
            output += "@" + index +
                    '\n' +
                    "D=A\n"
                    "@ARG\n"
                    "A=D+M\n"
                    "D=M\n";
            break;
        case TOKEN_NAME::STATIC:
            output += "@" + class_name + "." + index +
                    '\n' +
                    "D=M\n";
            break;
        case TOKEN_NAME::THIS:
            output += "@" + index +
                    '\n' +
                    "D=A\n"
                    "@THIS\n"
                    "A=D+M\n"
                    "D=M\n";
            break;
        case TOKEN_NAME::THAT:
            output += "@" +index +
                    '\n' +
                    "D=A\n"
                    "@THAT\n"
                    "A=D+M\n"
                    "D=M\n";
            break;
        case TOKEN_NAME::POINTER:
            output += "@" + std::to_string(stoi(index) + 3) +
                    '\n' +
                    "D=M\n";
            break;
        case TOKEN_NAME::TEMP:
            output += "@" + std::to_string(stoi(index) + 5) +
                    '\n' +
                    "D=M\n";
            break;
        default: ;
    }
    return output + "@SP\n"
           "M=M+1\n"
           "A=M-1\n"
           "M=D\n";
}

std::string Translator::parsePop(const TOKEN_NAME segment, const std::string& index) const {
    switch (segment) {
        case TOKEN_NAME::CONSTANT:
            std::cerr << "ERROR: Can't pop to constant" << std::endl;
            return "\n";
        case TOKEN_NAME::LOCAL:
            return "@LCL\n"
                   "D=M\n"
                   "@" +
                   index +
                   '\n' +
                   "D=D+A\n"
                   "@SP\n"
                   "M=M-1\n"
                   "A=M+1\n"
                   "M=D\n"
                   "A=A-1\n"
                   "D=M\n"
                   "A=A+1\n"
                   "A=M\n"
                   "M=D\n";
        case TOKEN_NAME::ARGUMENT:
            return "@ARG\n"
                   "D=M\n"
                   "@" +
                   index +
                   '\n' +
                   "D=D+A\n"
                   "@SP\n"
                   "M=M-1\n"
                   "A=M+1\n"
                   "M=D\n"
                   "A=A-1\n"
                   "D=M\n"
                   "A=A+1\n"
                   "A=M\n"
                   "M=D\n";
        case TOKEN_NAME::STATIC:
            return "@SP\n"
                   "M=M-1\n"
                   "A=M\n"
                   "D=M\n"
                   "@" +
                   class_name + "." + index +
                   '\n' +
                   "M=D\n";
        case TOKEN_NAME::THIS:
            return "@THIS\n"
                   "D=M\n"
                   "@" +
                   index +
                   '\n' +
                   "D=D+A\n"
                   "@SP\n"
                   "M=M-1\n"
                   "A=M+1\n"
                   "M=D\n"
                   "A=A-1\n"
                   "D=M\n"
                   "A=A+1\n"
                   "A=M\n"
                   "M=D\n";
        case TOKEN_NAME::THAT:
            return "@THAT\n"
                   "D=M\n"
                   "@" +
                   index +
                   '\n' +
                   "D=D+A\n"
                   "@SP\n"
                   "M=M-1\n"
                   "A=M+1\n"
                   "M=D\n"
                   "A=A-1\n"
                   "D=M\n"
                   "A=A+1\n"
                   "A=M\n"
                   "M=D\n";
        case TOKEN_NAME::POINTER:
            return "@SP\n"
                   "M=M-1\n"
                   "A=M\n"
                   "D=M\n"
                   "@" +
                   std::to_string(stoi(index) + 3) +
                   '\n' +
                   "M=D\n";
        case TOKEN_NAME::TEMP:
            return "@SP\n"
                   "M=M-1\n"
                   "A=M\n"
                   "D=M\n"
                   "@" +
                   std::to_string(stoi(index) + 5) +
                   '\n' +
                   "M=D\n";
        default: ;
    }
    return "\n";
}

std::string Translator::parseLabel(const std::string &label) const {
    return "(" + label + ")\n";
}

std::string Translator::parseGoto(const std::string &label) const {
    return "@" + label +
           "\n"
           "0;JMP\n";
}

std::string Translator::parseIfGoto(const std::string &label) const {
    return "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "D=M\n"
           "@" +
           label +
           "\n"
           "D;JNE\n";
}

std::string Translator::parseFunction(const std::string &name, const int nVars) const {
    std::string output = "(" + name +
                         ")\n";
    if (nVars > 0) {
        output += "@SP\n"
                "A=M\n";
        for (int i = 0; i < nVars - 1; i++) {
            output += "M=0\n"
                    "A=A+1\n";
        }
        output += "M=0\n"
                "@" +
                std::to_string(nVars) +
                "\n"
                "D=A\n"
                "@SP\n"
                "M=D+M\n";
    }
    return output;
}

std::string Translator::parseCall(const std::string &name, int nArgs) const {
    static int n = 0;
    n++;
    std::string output;
    if (nArgs < 1) {
        output += parsePush(TOKEN_NAME::CONSTANT, "0");
        nArgs = 1;
    }
    output += "@" + class_name + ".return." + std::to_string(n) +
            "\n"
            "D=A\n"
            "@SP\n"
            "M=M+1\n"
            "A=M-1\n"
            "M=D\n"
            "@LCL\n"
            "D=M\n"
            "@SP\n"
            "M=M+1\n"
            "A=M-1\n"
            "M=D\n"
            "@ARG\n"
            "D=M\n"
            "@SP\n"
            "M=M+1\n"
            "A=M-1\n"
            "M=D\n"
            "@THIS\n"
            "D=M\n"
            "@SP\n"
            "M=M+1\n"
            "A=M-1\n"
            "M=D\n"
            "@THAT\n"
            "D=M\n"
            "@SP\n"
            "M=M+1\n"
            "A=M-1\n"
            "M=D\n"
            "@" +
            std::to_string(5 + nArgs) +
            "\n"
            "D=A\n"
            "@SP\n"
            "D=M-D\n"
            "@ARG\n"
            "M=D\n"
            "@SP\n"
            "D=M\n"
            "@LCL\n"
            "M=D\n"
            "@" +
            name +
            "\n"
            "0;JMP\n"
            "(" +
            class_name + ".return." + std::to_string(n) +
            ")\n";
    return output;
}

std::string Translator::parseReturn() const {
    return "@SP\n"
            "A=M-1\n"
            "D=M\n"
            "@ARG\n"
            "A=M\n"
            "M=D\n"
            "D=A+1\n"
            "@SP\n"
            "M=D\n"
            "@5\n"
            "D=A\n"
            "@LCL\n"
            "A=M-D\n"
            "D=M\n"
            "@SP\n"
            "A=M\n"
            "M=D\n"
            "@LCL\n"
            "A=M-1\n"
            "D=M\n"
            "@THAT\n"
            "M=D\n"
            "@2\n"
            "D=A\n"
            "@LCL\n"
            "A=M-D\n"
            "D=M\n"
            "@THIS\n"
            "M=D\n"
            "@3\n"
            "D=A\n"
            "@LCL\n"
            "A=M-D\n"
            "D=M\n"
            "@ARG\n"
            "M=D\n"
            "@4\n"
            "D=A\n"
            "@LCL\n"
            "A=M-D\n"
            "D=M\n"
            "@LCL\n"
            "M=D\n"
            "@SP\n"
            "A=M\n"
            "A=M\n"
            "0;JMP\n";
}