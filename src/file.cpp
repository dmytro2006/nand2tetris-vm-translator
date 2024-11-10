#include "file.h"

File::File(std::string name, file_type type)
{
    this->path = std::filesystem::path(name);
    this->type = type;
}

File::~File()
{
    file.close();
}

bool File::open()
{
    if (type == file_type::INPUT)
    {
        this->file.open(path, std::ios_base::in);
        return file.good();
    }
    if (type == file_type::OUTPUT)
    {
        this->file.open(path, std::ios_base::out);
        return file.good();
    }
    return 0;
}

void File::close()
{
    this->file.close();
}

bool File::remove()
{
    return std::filesystem::remove(path);
}

std::string File::read()
{
    std::stringstream ss;
    ss << this->file.rdbuf();
    return ss.str();
}

void File::write(std::string output)
{
    this->file << output;
}

std::string File::getNameWithoutExtension()
{
    std::string path_string = path.string();
    path_string = path_string.substr(0, path_string.find_last_of("."));
    return path_string.substr(path_string.find_last_of("\\") + 1);
}