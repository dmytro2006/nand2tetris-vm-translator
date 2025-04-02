#include "file.h"

File::File(const std::string &name, const file_type &type) {
    this->path = std::filesystem::path(name);
    this->type = type;
}

File::File(const std::filesystem::path &path, const file_type &type) {
    this->path = path;
    this->type = type;
}

File::~File() {
    file.close();
}

bool File::open() {
    if (type == file_type::INPUT) {
        this->file.open(path, std::ios_base::in);
        return file.good();
    }
    if (type == file_type::OUTPUT) {
        this->file.open(path, std::ios_base::out);
        return file.good();
    }
    return false;
}

void File::close() {
    this->file.close();
}

bool File::remove() const {
    return std::filesystem::remove(path);
}

std::string File::read() const {
    std::stringstream ss;
    ss << this->file.rdbuf();
    return ss.str();
}

void File::write(const std::string &output) {
    this->file << output;
}

std::string File::getNameWithoutExtension() const {
    std::string path_string = path.string();
    path_string = path_string.substr(0, path_string.find_last_of('.'));
    return path_string.substr(path_string.find_last_of("\\/") + 1);
}
