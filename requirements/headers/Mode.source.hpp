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
t_parsedModeCommand     &parseModeCommand(std::string command);
void                    setOperatorMode(Channel &channel, Client &client, bool add);
void                    setVoiceMode(Channel &channel, Client &client, bool add);
void                    setPrivateMode(Channel &channel, bool add);
void                    setSecretMode(Channel &channel, bool add);
void                    setInviteOnlyMode(Channel &channel, bool add);
void                    setVoiceAndOpsOnlyMode(Channel &channel, bool add);
void                    setNoOutsideMessagesMode(Channel &channel, bool add);
void                    setOnlyOpsChangeTopicMode(Channel &channel, bool add);
void                    setClientLimitMode(Channel &channel, uint64_t limit, bool add);

#endif // !IRCMODE_SOURCE_HPP
