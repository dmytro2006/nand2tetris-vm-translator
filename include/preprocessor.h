#pragma once
#include <bits/stdc++.h>

class Preprocessor
{
public:
    Preprocessor(const std::string &code);
    std::string preprocess(); // removes comments and blank lines

private:
    std::string code;
};