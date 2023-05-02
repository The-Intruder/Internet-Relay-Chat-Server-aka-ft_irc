/* -------------------------------------------------------------------------- */
/*  File: iIRCServer.class.hpp                                                */
/*  Brief: IRC server header file                                             */
/*  Authors:                                                                  */
/*   - Mohamed Amine Naimi                                                    */
/*   - Hssain Ait Kadir                                                       */
/*   - Abdellah Bellakrim                                                     */
/*  Version: 1.0                                                              */
/*  License: GPL-3.0                                                          */
/* -------------------------------------------------------------------------- */

#pragma once
#ifndef IRCSERVER_CLASS_HPP

/* ---------------------------------- INCLUDES ------------------------------ */

# include <sys/socket.h>
# include <iostream>
# include <string>
# include <unistd.h>
# include <fcntl.h>
# include <netdb.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <csignal>
# include <sys/stat.h>
# include <poll.h>

/* ---------------------------------- CLASSES ------------------------------- */

class IRCServer {
    public:
        std::string     server_pass;
        size_t          server_port;
    public:
        IRCServer();
        IRCServer(std::string server_pass, size_t server_port);
        IRCServer(IRCServer const &src);
        const IRCServer &operator=(IRCServer const &src);
        ~IRCServer();
};


#endif // !IRCSERVER_CLASS_HPP