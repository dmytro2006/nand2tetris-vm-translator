//
// file.h
// All functions for handling files
// Created by dmytro2006 on 10/11/2024.
//

#ifndef FILE_H
#define FILE_H

#include <string>
#include <filesystem>
#include <fstream>

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

    std::string get_name_without_extension() const;

private:
    std::fstream file;
    std::filesystem::path path;
    file_type type;

    void close();
};
#endif // FILE_H
