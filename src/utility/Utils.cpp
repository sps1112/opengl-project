#include <utility/Utils.h>

void log_message(const char *message)
{
    std::cout << message << "\n";
}

void log_message(std::string message)
{
    log_message(message.c_str());
}

void log_message(const float n)
{
    log_message(std::to_string(n));
}
