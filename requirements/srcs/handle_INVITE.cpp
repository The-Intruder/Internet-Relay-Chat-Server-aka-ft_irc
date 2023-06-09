/* ---------------------------------------------------------------------------------------------------------------------------------------------------- */
/*  File: handle_INVITE.cpp                                                                                                                             */
/*  Brief: Invite source file                                                                                                                           */
/*  Authors:                                                                                                                                            */
/*   - Mohamed Amine Naimi                                                                                                                              */
/* ---------------------------------------------------------------------------------------------------------------------------------------------------- */

#include "../ircserv.head.hpp"

/* ---------------------------------------------------------------------------------------------------------------------------------------------------- */

static t_parsedInviteCommand  parseInviteCommand(std::string const &command, size_t pfdsindex)
{
    t_parsedInviteCommand    parsedCommand;
    std::string::size_type pos = 0;
    std::string::size_type pos2 = 0;

    pos = command.find(' ');
    if (pos == std::string::npos)
    {
        ERR_NEEDMOREPARAMS(pfdsindex, command[0]);
        throw std::runtime_error("");
    }
    parsedCommand.nickname = command.substr(0, pos);
    while ( pos < command.length() && command[pos] == ' ')
        pos++;
    if (pos >= command.length())
    {
        ERR_NEEDMOREPARAMS(pfdsindex, command[0]);
        throw std::runtime_error("");
    }
    pos = command.find(' ', pos);
    if (pos + 1 >= command.length())
    {
        ERR_NEEDMOREPARAMS(pfdsindex, command[0]);
        throw std::runtime_error("");
    }
    if (command[pos + 1] == '#' && pos + 2 < command.length())
    {
        pos2 = command.find(' ', pos)
        if (pos2 == std::string::npos)
            parsedCommand.channel_name = command.substr(pos + 1);
        else
            parsedCommand.channel_name = command.substr(pos + 1, pos2 - pos);
    }
    else
    {
        ERR_NEEDMOREPARAMS(pfdsindex, command[0]);
        throw std::runtime_error("");
    }
    return (parsedCommand);
}

/* ---------------------------------------------------------------------------------------------------------------------------------------------------- */

static std::pair<const int, Client>     &getClient(std::map<int, Client> &ClientsMap, std::string nickname)
{
    std::map<int, Client>::iterator it = ClientsMap.begin();

    while(it != ClientsMap.end())
    {
        if(it->second.getNICKNAME() == nickname)
            break;
        it++;
    }
    if (it == ClientsMap.end())
    {
        ERR_USERNOTINCHANNEL(pfdsindex, nickname, parsedCommand.channel_name);
        throw std::runtime_error("");
    }
    return (*(it));
}

/* ---------------------------------------------------------------------------------------------------------------------------------------------------- */

static Channel   &getChannel(std::map<std::string, Channel> &Channels, std::string channelName)
{
    std::map<std::string, Channel>::iterator it = Channels.find(channelName);

    if (it == Channels.end())
        throw std::runtime_error("channel not found");
    return (it->second);
}

/* ---------------------------------------------------------------------------------------------------------------------------------------------------- */

void    Server::executeInviteCommand(size_t pfdsindex, std::vector<std::string> &full_cmd)
{
    t_parsedInviteCommand parsedCommand;

    try
    {
        if (full_cmd.size() == 1)
        {
            ERR_NEEDMOREPARAMS(pfdsindex, full_cmd[0]);
            throw std::runtime_error("");
        }
        std::string command = full_cmd[1];    
        parsedCommand = parseInviteCommand(command, pfdsindex)
        if (parsedCommand.nickname == "" || parsedCommand.channel_name == "")
        {
            ERR_NEEDMOREPARAMS(pfdsindex, full_cmd[0]);
            throw std::runtime_error("");
        }
        std::pair<const int, Client> client = getClient(ClientsMap, parsedCommand.nickname);
        Channel& channel = getChannel(ChannelsMap, parsedCommand.channel_name);
        // Check if the client is in the channel
        if (channel.getUsers().find(client.first) == channel.getUsers().end())
        {
            ERR_USERNOTINCHANNEL(pfdsindex, parsedCommand.nickname, parsedCommand.channel_name);
            throw std::runtime_error("");
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
