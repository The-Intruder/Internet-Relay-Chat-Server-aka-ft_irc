#include "../ircserv.head.hpp"

/* -------------------------------------------------------------------------- */
void    writeMessageToClient(int fd, std::string message)
{
    int valwrite = write(fd, message.c_str() ,message.length());
    if(valwrite < 0)
        throw std::runtime_error("write failed");
}