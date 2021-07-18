#ifndef FILESYSTEM_H
#define FILESYSTEM_H

// Standard Headers
#include <string>
#include <cstdlib>
#include "root_directory.h"

// File System Class
class FileSystem
{
private:
    typedef std::string (*Builder)(const std::string &path);

public:
    // Returns file path from string
    static std::string get_path(const std::string &path);

private:
    static std::string const &get_path_root();
    static Builder get_path_builder();
    static std::string get_path_relative_root(const std::string &path);
    static std::string get_path_relative_binary(const std::string &path);
};

#endif // !FILESYSTEM_H
