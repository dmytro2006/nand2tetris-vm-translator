#include "compiler.h"

Compiler::Compiler(std::vector<Tokenizer::token> tokens, std::string className)
{
    this->tokens = tokens;
    this->className = className;
}

std::string &Compiler::getOutput()
{
    return this->output;
}
int Compiler::compile()
{
    for (int i = 0; i < tokens.size(); i++)
    {
        switch (tokens[i].type)
        {
        case Tokenizer::token_type::COMMAND:
            if (i + 2 >= tokens.size())
            {
                std::cerr << "ERROR: Not enough arguments for " << (tokens[i].v == Tokenizer::value::PUSH ? "PUSH" : "POP") << std::endl;
                return 1;
            }
            if (tokens[i + 1].type == Tokenizer::token_type::SEGMENT && tokens[i + 2].type == Tokenizer::token_type::INT_LITERAL)
                output += Compiler::command(tokens[i].v, tokens[i + 1].v, tokens[i + 2].val_int);
            else
            {
                std::cerr << "ERROR: Invalid arguments for " << (tokens[i].v == Tokenizer::value::PUSH ? "PUSH" : "POP") << std::endl;
                return 1;
            }
            break;
        case Tokenizer::token_type::OPERATOR:
            output += Compiler::oper(tokens[i].v);
            break;
        case Tokenizer::token_type::SEGMENT:
            break;
        case Tokenizer::token_type::INT_LITERAL:
            break;
        case Tokenizer::token_type::STRING_LITERAL:
            break;
        case Tokenizer::token_type::LABEL:
            if (i + 1 >= tokens.size())
            {
                std::cerr << "ERROR: Not enough arguments for LABEL" << std::endl;
                return 1;
            }
            if (tokens[i + 1].type == Tokenizer::token_type::STRING_LITERAL)
                output += Compiler::parseLabel(tokens[i + 1].val_str);
            else
            {
                std::cerr << "ERROR: Invalid arguments for LABEL" << std::endl;
                return 1;
            }
            break;
        case Tokenizer::token_type::GOTO:
            if (i + 1 >= tokens.size())
            {
                std::cerr << "ERROR: Not enough arguments for GOTO" << std::endl;
                return 1;
            }
            if (tokens[i + 1].type == Tokenizer::token_type::STRING_LITERAL)
                output += Compiler::parseGoto(tokens[i + 1].val_str);
            else
            {
                std::cerr << "ERROR: Invalid arguments for GOTO" << std::endl;
                return 1;
            }
            break;
        case Tokenizer::token_type::IF_GOTO:
            if (i + 1 >= tokens.size())
            {
                std::cerr << "ERROR: Not enough arguments for IF-GOTO" << std::endl;
                return 1;
            }
            if (tokens[i + 1].type == Tokenizer::token_type::STRING_LITERAL)
                output += Compiler::parseIfGoto(tokens[i + 1].val_str);
            else
            {
                std::cerr << "ERROR: Invalid arguments for IF-GOTO" << std::endl;
                return 1;
            }
            break;
        case Tokenizer::token_type::FUNCTION:
            if (i + 2 >= tokens.size())
            {
                std::cerr << "ERROR: Not enough arguments for FUNCTION" << std::endl;
                return 1;
            }
            if (tokens[i + 1].type == Tokenizer::token_type::STRING_LITERAL && tokens[i + 2].type == Tokenizer::token_type::INT_LITERAL)
                output += Compiler::parseFunction(tokens[i + 1].val_str, tokens[i + 2].val_int);
            else
            {
                std::cerr << "ERROR: Invalid arguments for FUNCTION" << std::endl;
                return 1;
            }
            break;
        case Tokenizer::token_type::CALL:
            if (i + 2 >= tokens.size())
            {
                std::cerr << "ERROR: Not enough arguments for CALL" << std::endl;
                return 1;
            }
            if (tokens[i + 1].type == Tokenizer::token_type::STRING_LITERAL && tokens[i + 2].type == Tokenizer::token_type::INT_LITERAL)
                output += Compiler::parseCall(tokens[i + 1].val_str, tokens[i + 2].val_int);
            else
            {
                std::cerr << "ERROR: Invalid arguments for CALL" << std::endl;
                return 1;
            }
            break;
        case Tokenizer::token_type::RETURN:
            output += parseReturn();
            break;
        }
    }
    return 0;
}

std::string Compiler::command(Tokenizer::value token_val, Tokenizer::value segment, int index)
{
    switch (token_val)
    {
    case Tokenizer::value::PUSH:
        return parsePush(segment, index);
        break;
    case Tokenizer::value::POP:
        return parsePop(segment, index);
        break;
    }
}

inline std::string Compiler::oper(Tokenizer::value token_val)
{
    return (this->*operator_code.at(token_val))();
}

inline std::string Compiler::parseAdd()
{
    return "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "D=M\n"
           "A=A-1\n"
           "M=D+M\n";
}

inline std::string Compiler::parseSub()
{
    return "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "D=M\n"
           "A=A-1\n"
           "M=D-M\n"
           "M=-M\n";
}

inline std::string Compiler::parseNeg()
{
    return "@SP\n"
           "A=M-1\n"
           "M=-M\n";
}

std::string Compiler::parseEq()
{
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
std::string Compiler::parseGt()
{
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
std::string Compiler::parseLt()
{
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
std::string Compiler::parseAnd()
{
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
inline std::string Compiler::parseOr()
{
    return "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "D=M\n"
           "A=A-1\n"
           "M=D|M\n";
}
std::string Compiler::parseNot()
{
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

std::string Compiler::parsePush(Tokenizer::value segment, int index)
{
    std::string output;
    switch (segment)
    {
    case Tokenizer::value::CONSTANT:
        output += "@" + std::to_string(index) +
                  '\n' +
                  "D=A\n";
        break;
    case Tokenizer::value::LOCAL:
        output += "@" + std::to_string(index) +
                  '\n' +
                  "D=A\n"
                  "@LCL\n"
                  "A=D+M\n"
                  "D=M\n";
        break;
    case Tokenizer::value::ARGUMENT:
        output += "@" + std::to_string(index) +
                  '\n' +
                  "D=A\n"
                  "@ARG\n"
                  "A=D+M\n"
                  "D=M\n";
        break;
    case Tokenizer::value::STATIC:
        output += "@" + className + "." + std::to_string(index) +
                  '\n' +
                  "D=M\n";
        break;
    case Tokenizer::value::THIS:
        output += "@" + std::to_string(index) +
                  '\n' +
                  "D=A\n"
                  "@THIS\n"
                  "A=D+M\n"
                  "D=M\n";
        break;
    case Tokenizer::value::THAT:
        output += "@" + std::to_string(index) +
                  '\n' +
                  "D=A\n"
                  "@THAT\n"
                  "A=D+M\n"
                  "D=M\n";
        break;
    case Tokenizer::value::POINTER:
        output += "@" + std::to_string(index + 3) +
                  '\n' +
                  "D=M\n";
        break;
    case Tokenizer::value::TEMP:
        output += "@" + std::to_string(index + 5) +
                  '\n' +
                  "D=M\n";
        break;
    }
    return output + "@SP\n"
                    "M=M+1\n"
                    "A=M-1\n"
                    "M=D\n";
}

std::string Compiler::parsePop(Tokenizer::value segment, int index)
{
    switch (segment)
    {
    case Tokenizer::value::CONSTANT:
        std::cerr << "ERROR: Can't pop to constant" << std::endl;
        return "\n";
        break;
    case Tokenizer::value::LOCAL:
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
        break;
    case Tokenizer::value::ARGUMENT:
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
        ;
        break;
    case Tokenizer::value::STATIC:
        return "@SP\n"
               "M=M-1\n"
               "A=M\n"
               "D=M\n"
               "@" +
               className + "." + std::to_string(index) +
               '\n' +
               "M=D\n";
        break;
    case Tokenizer::value::THIS:
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
        break;
    case Tokenizer::value::THAT:
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
        break;
    case Tokenizer::value::POINTER:
        return "@SP\n"
               "M=M-1\n"
               "A=M\n"
               "D=M\n"
               "@" +
               std::to_string(index + 3) +
               '\n' +
               "M=D\n";
        break;
    case Tokenizer::value::TEMP:
        return "@SP\n"
               "M=M-1\n"
               "A=M\n"
               "D=M\n"
               "@" +
               std::to_string(index + 5) +
               '\n' +
               "M=D\n";
        break;
    }
    return "\n";
}

std::string Compiler::parseLabel(std::string label)
{
    return "(" + label + ")\n";
}

std::string Compiler::parseGoto(std::string label)
{
    return "@" + label +
           "\n"
           "0;JMP\n";
}

std::string Compiler::parseIfGoto(std::string label)
{
    return "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "D=M\n"
           "@" +
           label +
           "\n"
           "D;JNE\n";
}
std::string Compiler::parseFunction(std::string name, int nVars)
{
    std::string output = "(" + name +
                         ")\n";
    if (nVars > 0)
    {
        output += "@SP\n"
                  "A=M\n";
        for (int i = 0; i < nVars - 1; i++)
        {
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
std::string Compiler::parseCall(std::string name, int nArgs)
{
    static int n = 0;
    n++;
    std::string output;
    if (nArgs < 1)
    {
        output += parsePush(Tokenizer::value::CONSTANT, 0);
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
std::string Compiler::parseReturn()
{
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