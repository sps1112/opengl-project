#include <Utils.h>

void Log(const char *message)
{
    std::cout << message << "\n";
}

void Log(std::string message)
{
    Log(message.c_str());
}