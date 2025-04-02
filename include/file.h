#pragma once
#include <bits/stdc++.h>

enum class file_type {
    INPUT,
    OUTPUT
};
/**
 * Handles files
 */
class File {
public:
    File(const std::string &name, const file_type &type);
    File(const std::filesystem::path &path, const file_type &type);

    ~File();

    bool open();

    bool remove() const;

    std::string read() const;

    void write(const std::string &output);

    std::string getNameWithoutExtension() const;

private:
    std::fstream file;
    std::filesystem::path path;
    file_type type;

    void close();
};
