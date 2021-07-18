#include <utility/FileSystem.h>

std::string FileSystem::get_path(const std::string &path)
{
    static std::string (*pathBuilder)(std::string const &) = get_path_builder();
    return (*pathBuilder)(path);
}

std::string const &FileSystem::get_path_root()
{
    static char const *envRoot = getenv("LOGL_ROOT_PATH");
    static char const *givenRoot = (envRoot != nullptr ? envRoot : logl_root);
    static std::string root = (givenRoot != nullptr ? givenRoot : "");
    return root;
}

FileSystem::Builder FileSystem::get_path_builder()
{
    if (get_path_root() != "")
    {
        return &FileSystem::get_path_relative_root;
    }
    else
    {
        return &FileSystem::get_path_relative_binary;
    }
}

std::string FileSystem::get_path_relative_root(const std::string &path)
{
    return (get_path_root() + std::string("/") + path);
}

std::string FileSystem::get_path_relative_binary(const std::string &path)
{
    return ("../../../" + path);
}
