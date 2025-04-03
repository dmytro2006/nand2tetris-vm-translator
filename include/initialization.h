//
// Created by dmytro2006 on 03/04/2025.
//

#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#include "translator.h"

// Code for system initialization and calling Sys.init()
std::string Translator::initialization_code =
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

#endif //INITIALIZATION_H
