/* -------------------------------------------------------------------------- */
/*  File: Mode.source.hpp                                                     */
/*  Brief: Mode header file                                                   */
/*  Authors:                                                                  */
/*   - Mohamed Amine Naimi                                                    */
/* -------------------------------------------------------------------------- */

#pragma once
#ifndef IRCMODE_SOURCE_HPP
# define IRCMODE_SOURCE_HPP

/* ---------------------------------- INCLUDES ------------------------------ */

# include "libraries.includes.hpp"
# include "Server.class.hpp"
# include "Channel.class.hpp"

/* ---------------------------------- PROTOTYPES ---------------------------- */

typedef struct s_parsedModeCommand
{
    std::string                 channel_name;
    std::vector<std::string>    mode;
    std::string                 parameter;
}   t_parsedModeCommand;

void                    executeModeCommand(Server &server, Client &client, std::string command);

#endif // !IRCMODE_SOURCE_HPP
