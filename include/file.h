#pragma once
#include <bits/stdc++.h>

enum class file_type
{
    INPUT,
    OUTPUT
};

class File
{
public:
    File(std::string name, file_type type);
    ~File();
    bool open();
    bool remove();
    std::string read();
    void write(std::string output);
    std::string getNameWithoutExtension();

private:
    std::fstream file;
    std::filesystem::path path;
    file_type type;
    void close();
};