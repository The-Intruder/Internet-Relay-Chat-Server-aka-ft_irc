/* -------------------------------------------------------------------------- */
/*  File: IRCServer.class.cpp                                                 */
/*  Brief: IRC server header file                                             */
/*  Authors:                                                                  */
/*   - Mohamed Amine Naimi                                                    */
/*   - Hssain Ait Kadir                                                       */
/*   - Abdellah Bellakrim                                                     */
/*  Version: 1.0                                                              */
/*  License: GPL-3.0                                                          */
/* -------------------------------------------------------------------------- */

#include "IRCServer.class.hpp"

/* -------------------------------------------------------------------------- */

IRCServer::IRCServer() : server_pass(""), server_port(0)
{
    std::cout << "IRCServer default constructor called" << std::endl;
}

/* -------------------------------------------------------------------------- */

IRCServer::IRCServer(std::string server_pass, size_t server_port)
    : server_pass(server_pass), server_port(server_port)
{
    std::cout << "IRCServer constructor called" << std::endl;
}

/* -------------------------------------------------------------------------- */

IRCServer::IRCServer(IRCServer const &src)
    : server_pass(src.server_pass), server_port(src.server_port)
{
    std::cout << "IRCServer copy constructor called" << std::endl;
}

/* -------------------------------------------------------------------------- */

const IRCServer &IRCServer::operator=(IRCServer const &src)
{
    if (this != &src)
    {
        this->server_pass = src.server_pass;
        this->server_port = src.server_port;
    }
    std::cout << "IRCServer assignation operator called" << std::endl;
    return *this;
}

/* -------------------------------------------------------------------------- */

IRCServer::~IRCServer()
{
    std::cout << "IRCServer destructor called" << std::endl;
}
