#include <FileSystem.h>

std::string FileSystem::getPath(const std::string &path)
{
    static std::string (*pathBuilder)(std::string const &) = getPathBuilder();
    return (*pathBuilder)(path);
}

std::string const &FileSystem::getRoot()
{
    static char const *envRoot = getenv("LOGL_ROOT_PATH");
    static char const *givenRoot = (envRoot != nullptr ? envRoot : logl_root);
    static std::string root = (givenRoot != nullptr ? givenRoot : "");
    return root;
}

FileSystem::Builder FileSystem::getPathBuilder()
{
    if (getRoot() != "")
    {
        return &FileSystem::getPathRelativeRoot;
    }
    else
    {
        return &FileSystem::getPathRelativeBinary;
    }
}

std::string FileSystem::getPathRelativeRoot(const std::string &path)
{
    return (getRoot() + std::string("/") + path);
}

std::string FileSystem::getPathRelativeBinary(const std::string &path)
{
    return ("../../../" + path);
}
