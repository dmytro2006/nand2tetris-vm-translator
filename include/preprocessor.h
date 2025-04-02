#pragma once
#include <bits/stdc++.h>

/**
 * Preprocesses the source code
 */
class Preprocessor {
public:
    Preprocessor();

    static std::string preprocess(const std::string &input); // removes comments and blank lines
};
