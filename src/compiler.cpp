#include "compiler.h"

// Code for system initialization and calling Sys.init()
std::string Compiler::initialization_code =
        "@256\n"
        "D=A\n"
        "@SP\n"
        "M=D\n"
        "@Sys.init.return\n"
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
        "@5\n"
        "D=A\n"
        "@SP\n"
        "D=M-D\n"
        "@ARG\n"
        "M=D\n"
        "@SP\n"
        "D=M\n"
        "@LCL\n"
        "M=D\n"
        "@Sys.init\n"
        "0;JMP\n"
        "(Sys.init.return)\n";

Compiler::Compiler(const std::vector<Tokenizer::Token> &tokens, const std::string &className) {
    this->tokens = tokens;
    this->className = className;
}

std::string &Compiler::getOutput() {
    return this->output;
}

int Compiler::compile() {
    for (int i = 0; i < tokens.size(); i++) {
        switch (tokens[i].type) {
            case Tokenizer::TOKEN_TYPE::COMMAND:
                if (i + 2 >= tokens.size()) {
                    std::cerr << "ERROR: Not enough arguments for " << (tokens[i].name == Tokenizer::TOKEN_NAME::PUSH
                                                                            ? "PUSH"
                                                                            : "POP") << std::endl;
                    return 1;
                }
                if (tokens[i + 1].type == Tokenizer::TOKEN_TYPE::SEGMENT && tokens[i + 2].type ==
                    Tokenizer::TOKEN_TYPE::INT_LITERAL)
                    output += command(tokens[i].name, tokens[i + 1].name, tokens[i + 2].value_int);
                else {
                    std::cerr << "ERROR: Invalid arguments for " << (tokens[i].name == Tokenizer::TOKEN_NAME::PUSH
                                                                         ? "PUSH"
                                                                         : "POP") << std::endl;
                    return 1;
                }
                break;
            case Tokenizer::TOKEN_TYPE::OPERATOR:
                output += oper(tokens[i].name);
                break;
            case Tokenizer::TOKEN_TYPE::SEGMENT:
            case Tokenizer::TOKEN_TYPE::INT_LITERAL:
            case Tokenizer::TOKEN_TYPE::STRING_LITERAL:
                break;
            case Tokenizer::TOKEN_TYPE::LABEL:
                if (i + 1 >= tokens.size()) {
                    std::cerr << "ERROR: Not enough arguments for LABEL" << std::endl;
                    return 1;
                }
                if (tokens[i + 1].type == Tokenizer::TOKEN_TYPE::STRING_LITERAL)
                    output += parseLabel(tokens[i + 1].value_str);
                else {
                    std::cerr << "ERROR: Invalid arguments for LABEL" << std::endl;
                    return 1;
                }
                break;
            case Tokenizer::TOKEN_TYPE::GOTO:
                if (i + 1 >= tokens.size()) {
                    std::cerr << "ERROR: Not enough arguments for GOTO" << std::endl;
                    return 1;
                }
                if (tokens[i + 1].type == Tokenizer::TOKEN_TYPE::STRING_LITERAL)
                    output += parseGoto(tokens[i + 1].value_str);
                else {
                    std::cerr << "ERROR: Invalid arguments for GOTO" << std::endl;
                    return 1;
                }
                break;
            case Tokenizer::TOKEN_TYPE::IF_GOTO:
                if (i + 1 >= tokens.size()) {
                    std::cerr << "ERROR: Not enough arguments for IF-GOTO" << std::endl;
                    return 1;
                }
                if (tokens[i + 1].type == Tokenizer::TOKEN_TYPE::STRING_LITERAL)
                    output += parseIfGoto(tokens[i + 1].value_str);
                else {
                    std::cerr << "ERROR: Invalid arguments for IF-GOTO" << std::endl;
                    return 1;
                }
                break;
            case Tokenizer::TOKEN_TYPE::FUNCTION:
                if (i + 2 >= tokens.size()) {
                    std::cerr << "ERROR: Not enough arguments for FUNCTION" << std::endl;
                    return 1;
                }
                if (tokens[i + 1].type == Tokenizer::TOKEN_TYPE::STRING_LITERAL && tokens[i + 2].type ==
                    Tokenizer::TOKEN_TYPE::INT_LITERAL)
                    output += parseFunction(tokens[i + 1].value_str, tokens[i + 2].value_int);
                else {
                    std::cerr << "ERROR: Invalid arguments for FUNCTION" << std::endl;
                    return 1;
                }
                break;
            case Tokenizer::TOKEN_TYPE::CALL:
                if (i + 2 >= tokens.size()) {
                    std::cerr << "ERROR: Not enough arguments for CALL" << std::endl;
                    return 1;
                }
                if (tokens[i + 1].type == Tokenizer::TOKEN_TYPE::STRING_LITERAL && tokens[i + 2].type ==
                    Tokenizer::TOKEN_TYPE::INT_LITERAL)
                    output += parseCall(tokens[i + 1].value_str, tokens[i + 2].value_int);
                else {
                    std::cerr << "ERROR: Invalid arguments for CALL" << std::endl;
                    return 1;
                }
                break;
            case Tokenizer::TOKEN_TYPE::RETURN:
                output += parseReturn();
                break;
        }
    }
    return 0;
}

std::string Compiler::command(const Tokenizer::TOKEN_NAME token_val, const Tokenizer::TOKEN_NAME segment,
                              const int index) const {
    switch (token_val) {
        case Tokenizer::TOKEN_NAME::PUSH:
            return parsePush(segment, index);
        case Tokenizer::TOKEN_NAME::POP:
            return parsePop(segment, index);
        default:
            return {};
    }
}

inline std::string Compiler::oper(const Tokenizer::TOKEN_NAME token_val) {
    return (this->*operator_code.at(token_val))();
}

std::string Compiler::parseAdd() const {
    return "@SP\n"
            "M=M-1\n"
            "A=M\n"
            "D=M\n"
            "A=A-1\n"
            "M=D+M\n";
}

std::string Compiler::parseSub() const {
    return "@SP\n"
            "M=M-1\n"
            "A=M\n"
            "D=M\n"
            "A=A-1\n"
            "M=D-M\n"
            "M=-M\n";
}

std::string Compiler::parseNeg() const {
    return "@SP\n"
            "A=M-1\n"
            "M=-M\n";
}

std::string Compiler::parseEq() const {
    static int n;
    n++;
    return "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "D=M\n"
           "A=A-1\n"
           "D=D-M\n"
           "@EQ_" +
           this->className + "_" +
           std::to_string(n) +
           "\n"
           "D;JEQ\n"
           "D=0\n"
           "@END_EQ_" +
           this->className + "_" +
           std::to_string(n) +
           "\n"
           "0;JMP\n"
           "(EQ_" +
           this->className + "_" +
           std::to_string(n) +
           ")\n"
           "D=1\n"
           "(END_EQ_" +
           this->className + "_" +
           std::to_string(n) +
           ")\n"
           "@SP\n"
           "A=M-1\n"
           "M=D\n";
}

std::string Compiler::parseGt() const {
    static int n;
    n++;
    return "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "D=M\n"
           "A=A-1\n"
           "D=D-M\n"
           "@GT" +
           this->className + "_" +
           std::to_string(n) +
           "\n"
           "D;JLT\n"
           "D=0\n"
           "@END_GT" +
           this->className + "_" +
           std::to_string(n) +
           "\n"
           "0;JMP\n"
           "(GT" +
           this->className + "_" +
           std::to_string(n) +
           ")\n"
           "D=1\n"
           "(END_GT" +
           this->className + "_" +
           std::to_string(n) +
           ")\n"
           "@SP\n"
           "A=M-1\n"
           "M=D\n";
}

std::string Compiler::parseLt() const {
    static int n;
    n++;
    return "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "D=M\n"
           "A=A-1\n"
           "D=D-M\n"
           "@LT" +
           this->className + "_" +
           std::to_string(n) +
           "\n"
           "D;JGT\n"
           "D=0\n"
           "@END_LT" +
           this->className + "_" +
           std::to_string(n) +
           "\n"
           "0;JMP\n"
           "(LT" +
           this->className + "_" +
           std::to_string(n) +
           ")\n"
           "D=1\n"
           "(END_LT" +
           this->className + "_" +
           std::to_string(n) +
           ")\n"
           "@SP\n"
           "A=M-1\n"
           "M=D\n";
}

std::string Compiler::parseAnd() const {
    static int n;
    n++;
    return "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "D=M\n"
           "@AND_" +
           this->className + "_" +
           std::to_string(n) +
           "\n"
           "D;JEQ\n"
           "@SP\n"
           "A=M\n"
           "D=M\n"
           "@AND_" +
           this->className + "_" +
           std::to_string(n) +
           "\n"
           "D;JEQ\n"
           "D=1\n"
           "@END_AND_" +
           this->className + "_" +
           std::to_string(n) +
           "\n"
           "(@AND_" +
           this->className + "_" +
           std::to_string(n) +
           ")\n"
           "D=0\n"
           "(END_AND_" +
           this->className + "_" +
           std::to_string(n) +
           ")\n"
           "@SP\n"
           "A=M-1\n"
           "M=D\n";
}

std::string Compiler::parseOr() const {
    return "@SP\n"
            "M=M-1\n"
            "A=M\n"
            "D=M\n"
            "A=A-1\n"
            "M=D|M\n";
}

std::string Compiler::parseNot() const {
    static int n;
    n++;
    return "@SP\n"
           "A=M-1\n"
           "D=M\n"
           "@NOT_" +
           this->className + "_" +
           std::to_string(n) +
           "\n"
           "D;JEQ\n"
           "D=0\n"
           "@END_NOT_" +
           this->className + "_" +
           std::to_string(n) +
           "\n"
           "0;JMP\n"
           "(NOT_" +
           this->className + "_" +
           std::to_string(n) +
           ")\n"
           "D=1\n"
           "(END_NOT_" +
           this->className + "_" +
           std::to_string(n) +
           ")\n"
           "@SP\n"
           "A=M-1\n"
           "M=D\n";
}

std::string Compiler::parsePush(const Tokenizer::TOKEN_NAME segment, const int index) const {
    std::string output;
    switch (segment) {
        case Tokenizer::TOKEN_NAME::CONSTANT:
            output += "@" + std::to_string(index) +
                    '\n' +
                    "D=A\n";
            break;
        case Tokenizer::TOKEN_NAME::LOCAL:
            output += "@" + std::to_string(index) +
                    '\n' +
                    "D=A\n"
                    "@LCL\n"
                    "A=D+M\n"
                    "D=M\n";
            break;
        case Tokenizer::TOKEN_NAME::ARGUMENT:
            output += "@" + std::to_string(index) +
                    '\n' +
                    "D=A\n"
                    "@ARG\n"
                    "A=D+M\n"
                    "D=M\n";
            break;
        case Tokenizer::TOKEN_NAME::STATIC:
            output += "@" + className + "." + std::to_string(index) +
                    '\n' +
                    "D=M\n";
            break;
        case Tokenizer::TOKEN_NAME::THIS:
            output += "@" + std::to_string(index) +
                    '\n' +
                    "D=A\n"
                    "@THIS\n"
                    "A=D+M\n"
                    "D=M\n";
            break;
        case Tokenizer::TOKEN_NAME::THAT:
            output += "@" + std::to_string(index) +
                    '\n' +
                    "D=A\n"
                    "@THAT\n"
                    "A=D+M\n"
                    "D=M\n";
            break;
        case Tokenizer::TOKEN_NAME::POINTER:
            output += "@" + std::to_string(index + 3) +
                    '\n' +
                    "D=M\n";
            break;
        case Tokenizer::TOKEN_NAME::TEMP:
            output += "@" + std::to_string(index + 5) +
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

std::string Compiler::parsePop(const Tokenizer::TOKEN_NAME segment, const int index) const {
    switch (segment) {
        case Tokenizer::TOKEN_NAME::CONSTANT:
            std::cerr << "ERROR: Can't pop to constant" << std::endl;
            return "\n";
        case Tokenizer::TOKEN_NAME::LOCAL:
            return "@LCL\n"
                   "D=M\n"
                   "@" +
                   std::to_string(index) +
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
        case Tokenizer::TOKEN_NAME::ARGUMENT:
            return "@ARG\n"
                   "D=M\n"
                   "@" +
                   std::to_string(index) +
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
        case Tokenizer::TOKEN_NAME::STATIC:
            return "@SP\n"
                   "M=M-1\n"
                   "A=M\n"
                   "D=M\n"
                   "@" +
                   className + "." + std::to_string(index) +
                   '\n' +
                   "M=D\n";
        case Tokenizer::TOKEN_NAME::THIS:
            return "@THIS\n"
                   "D=M\n"
                   "@" +
                   std::to_string(index) +
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
        case Tokenizer::TOKEN_NAME::THAT:
            return "@THAT\n"
                   "D=M\n"
                   "@" +
                   std::to_string(index) +
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
        case Tokenizer::TOKEN_NAME::POINTER:
            return "@SP\n"
                   "M=M-1\n"
                   "A=M\n"
                   "D=M\n"
                   "@" +
                   std::to_string(index + 3) +
                   '\n' +
                   "M=D\n";
        case Tokenizer::TOKEN_NAME::TEMP:
            return "@SP\n"
                   "M=M-1\n"
                   "A=M\n"
                   "D=M\n"
                   "@" +
                   std::to_string(index + 5) +
                   '\n' +
                   "M=D\n";
        default: ;
    }
    return "\n";
}

std::string Compiler::parseLabel(const std::string &label) const {
    return "(" + label + ")\n";
}

std::string Compiler::parseGoto(const std::string &label) const {
    return "@" + label +
           "\n"
           "0;JMP\n";
}

std::string Compiler::parseIfGoto(const std::string &label) const {
    return "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "D=M\n"
           "@" +
           label +
           "\n"
           "D;JNE\n";
}

std::string Compiler::parseFunction(const std::string &name, const int nVars) const {
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

std::string Compiler::parseCall(const std::string &name, int nArgs) const {
    static int n = 0;
    n++;
    std::string output;
    if (nArgs < 1) {
        output += parsePush(Tokenizer::TOKEN_NAME::CONSTANT, 0);
        nArgs = 1;
    }
    output += "@" + className + ".return." + std::to_string(n) +
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
            className + ".return." + std::to_string(n) +
            ")\n";
    return output;
}

std::string Compiler::parseReturn() const {
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
