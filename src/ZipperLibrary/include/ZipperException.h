#include <exception>

class ZipperException : public std::exception
{
private:
    std::string message;

public:
    ZipperException(std::string msg) : message(msg) {}

    const char *what()
    {
        return message.c_str();
    }
};