/* ---------------------------------------------------------------------------------------------------------------------------------------------------- */
/*      File: handle_MODE.cpp                                                                                                                           */
/*      Brief: Mode source file                                                                                                                         */
/*      Authors:                                                                                                                                        */
/*       - Mohamed Amine Naimi                                                                                                                          */
/* ---------------------------------------------------------------------------------------------------------------------------------------------------- */

#include "../ircserv.head.hpp"

/* ---------------------------------------------------------------------------------------------------------------------------------------------------- */

static t_parsedModeCommand  parseModeCommand(Server *class_ptr, std::string const &command, size_t pfdsindex)
{
    t_parsedModeCommand    parsedCommand;
    std::string::size_type pos = 0;

    pos = command.find(' ');
    if (pos == std::string::npos)
    {
        ERR_NEEDMOREPARAMS(class_ptr->pfds[pfdsindex].fd, command);
        throw std::runtime_error("Error: mode command");
    }
    parsedCommand.channel_name = command.substr(0, pos);
    std::cout << parsedCommand.channel_name << std::endl;
    while (pos < command.length() && command[pos] == ' ')
        pos++;
    if (pos == std::string::npos || pos >= command.length() || pos + 1 >= command.length())
    {
        ERR_NEEDMOREPARAMS(class_ptr->pfds[pfdsindex].fd, command);
        throw std::runtime_error("Error: mode command");
    }
    if ((command[pos] == '+' || command[pos] == '-') && pos + 1 < command.length())
    {
        parsedCommand.mode = std::string(1, command[pos]);
        if (!std::isalpha(command[pos + 1]))
        {
            ERR_UNKNOWNMODE(class_ptr->pfds[pfdsindex].fd, std::string(1, command[pos + 1]), parsedCommand.channel_name);
            throw std::runtime_error("Error: mode command");
        }
        parsedCommand.mode += std::string(1, command[++pos]);
    }
    while (command[pos] == ' ' && pos < command.length())
        pos++;
    if (pos < command.length())
    {
        if (command[pos] == ':')
            parsedCommand.parameter = command.substr(pos + 1);
        else
            parsedCommand.parameter = command.substr(pos);
    }
    return (parsedCommand);
}

/* ---------------------------------------------------------------------------------------------------------------------------------------------------- */

static std::pair<const int, Client>     &getClient(Server *class_ptr, std::map<int, Client> &ClientsMap, std::string nickname, std::string channel_name, size_t pfdsindex)
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
        ERR_USERNOTINCHANNEL(class_ptr->pfds[pfdsindex].fd, nickname, channel_name);
        throw std::runtime_error("Error: getClient");
    }
    return (*(it));
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static Channel   &getChannel(std::map<std::string, Channel> &Channels, std::string channelName)
{
    std::map<std::string, Channel>::iterator it = Channels.find(channelName);

    if (it == Channels.end())
        throw std::runtime_error("getChannel: channel not found");
    return (it->second);
}

/* ---------------------------------------------------------------------------------------------------------------------------------------------------- */

static void    setOperator(Server *class_ptr, bool add, size_t pfdsindex, t_parsedModeCommand &parsedCommand)
{
    try
    {
        std::pair<const int,Client>   &client = getClient(class_ptr, class_ptr->ClientsMap, parsedCommand.parameter, parsedCommand.channel_name, pfdsindex);
        Channel     &channel = getChannel(class_ptr->ChannelsMap, parsedCommand.channel_name);
        if (add == true)
        {
            channel._joinedUsers.erase(client.first);
            channel._operators.insert(client);
        }
        else
        {
            channel._operators.erase(client.first);
            channel._joinedUsers.insert(client);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    RPL_CHANNELMODEIS(class_ptr->pfds[pfdsindex].fd, parsedCommand.channel_name, parsedCommand.mode, parsedCommand.parameter);
}

/* ---------------------------------------------------------------------------------------------------------------------------------------------------- */

static void     setVoiced(Server *class_ptr, bool add, size_t pfdsindex, t_parsedModeCommand &parsedCommand)
{
    try
    {
        std::pair<const int,Client>   &client = getClient(class_ptr, class_ptr->ClientsMap, parsedCommand.parameter, parsedCommand.channel_name, pfdsindex);
        Channel     &channel = getChannel(class_ptr->ChannelsMap, parsedCommand.channel_name);
        if (add == true)
        {
            channel._joinedUsers.erase(client.first);
            channel._voicedClients.insert(client);
        }
        else
        {
            channel._voicedClients.erase(client.first);
            channel._joinedUsers.insert(client);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    RPL_CHANNELMODEIS(class_ptr->pfds[pfdsindex].fd, parsedCommand.channel_name, parsedCommand.mode, parsedCommand.parameter);
}

/* ---------------------------------------------------------------------------------------------------------------------------------------------------- */

static std::pair<const int, Client>     &getClientFromNicknameHostAddress(Server *class_ptr, std::string nicknameHostAddress, std::string channel_name, size_t pfdsindex)
{
    std::string nickname = nicknameHostAddress.substr(0, nicknameHostAddress.find('!'));
    std::string address = nicknameHostAddress.substr(nicknameHostAddress.find('@') + 1);

    std::pair<const int, Client> &the_client = getClient(class_ptr, class_ptr->ClientsMap, nickname, channel_name, pfdsindex);;
    return (the_client);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static void     banClient(Server *class_ptr, bool add, t_parsedModeCommand &parsedCommand, size_t pfdsindex)
{
    try
    {
        Channel     &channel = getChannel(class_ptr->ChannelsMap, parsedCommand.channel_name);
        std::pair<const int,Client>   &client = getClientFromNicknameHostAddress(class_ptr, parsedCommand.parameter, parsedCommand.channel_name, pfdsindex);
        if (add == true)
        {
            channel._operators.erase(client.first);
            channel._voicedClients.erase(client.first);
            channel._bannedUsers.insert(client);
        }
        else
            channel._bannedUsers.erase(client.first);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    RPL_CHANNELMODEIS(class_ptr->pfds[pfdsindex].fd, parsedCommand.channel_name, parsedCommand.mode, parsedCommand.parameter);
}

/* ---------------------------------------------------------------------------------------------------------------------------------------------------- */

static void    setPrivate(Server *class_ptr, bool add, t_parsedModeCommand &parsedCommand, size_t pfdsindex)
{
    try
    {
        Channel     &channel = getChannel(class_ptr->ChannelsMap, parsedCommand.channel_name);
        if (add == true)
            channel._modes |= 0x01;
        else
            channel._modes &= ~0x01;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    RPL_CHANNELMODEIS(class_ptr->pfds[pfdsindex].fd, parsedCommand.channel_name, parsedCommand.mode, " ");
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static void    setSecret(Server *class_ptr, bool add, t_parsedModeCommand &parsedCommand, size_t pfdsindex)
{
    try
    {
        Channel     &channel = getChannel(class_ptr->ChannelsMap, parsedCommand.channel_name);
        if (add == true)
            channel._modes |= 0x02;
        else
            channel._modes &= ~0x02;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    RPL_CHANNELMODEIS(class_ptr->pfds[pfdsindex].fd, parsedCommand.channel_name, parsedCommand.mode, parsedCommand.parameter);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static void    setInviteOnly(Server *class_ptr, bool add, t_parsedModeCommand &parsedCommand, size_t pfdsindex)
{
    try
    {
        Channel     &channel = getChannel(class_ptr->ChannelsMap, parsedCommand.channel_name);
        if (add == true)
            channel._modes |= 0x04;
        else
            channel._modes &= ~0x04;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    RPL_CHANNELMODEIS(class_ptr->pfds[pfdsindex].fd, parsedCommand.channel_name, parsedCommand.mode, parsedCommand.parameter);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static void    setOnlyOpsChangeTopic(Server *class_ptr, bool add, t_parsedModeCommand &parsedCommand, size_t pfdsindex)
{
    try
    {
        Channel     &channel = getChannel(class_ptr->ChannelsMap, parsedCommand.channel_name);
        if (add == true)
            channel._modes |= 0x20;
        else
            channel._modes &= ~0x20;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    RPL_CHANNELMODEIS(class_ptr->pfds[pfdsindex].fd, parsedCommand.channel_name, parsedCommand.mode, parsedCommand.parameter);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static void    setNoOutsideMessages(Server *class_ptr, bool add, t_parsedModeCommand &parsedCommand, size_t pfdsindex)
{
    try
    {
        Channel     &channel = getChannel(class_ptr->ChannelsMap, parsedCommand.channel_name);
        if (add == true)
            channel._modes |= 0x10;
        else
            channel._modes &= ~0x10;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    RPL_CHANNELMODEIS(class_ptr->pfds[pfdsindex].fd, parsedCommand.channel_name, parsedCommand.mode, parsedCommand.parameter);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static void     setOnlyVoiceAndOps(Server *class_ptr, bool add, t_parsedModeCommand &parsedCommand, size_t pfdsindex)
{
    try
    {
        Channel     &channel = getChannel(class_ptr->ChannelsMap, parsedCommand.channel_name);
        if (add == true)
            channel._modes |= 0x08;
        else
            channel._modes &= ~0x08;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    RPL_CHANNELMODEIS(class_ptr->pfds[pfdsindex].fd, parsedCommand.channel_name, parsedCommand.mode, parsedCommand.parameter);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static void setClientLimit(Server *class_ptr, bool add, t_parsedModeCommand &parsedCommand, size_t pfdsindex, std::string command)
{
    try
    {
        Channel     &channel = getChannel(class_ptr->ChannelsMap, parsedCommand.channel_name);

        if (add == true)
            channel._modes |= 0x40;
        else
            channel._modes &= ~0x40;
        channel._client_limit = std::stoi(parsedCommand.parameter);
    }
    catch(const std::exception& e)
    {
        if (dynamic_cast<const std::invalid_argument*>(&e) != nullptr || dynamic_cast<const std::out_of_range*>(&e) != nullptr)
            ERR_NEEDMOREPARAMS(class_ptr->pfds[pfdsindex].fd, command);
        std::cerr << e.what() << std::endl;
    }
    RPL_CHANNELMODEIS(class_ptr->pfds[pfdsindex].fd, parsedCommand.channel_name, parsedCommand.mode, parsedCommand.parameter);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static void setKey(Server *class_ptr, bool add, t_parsedModeCommand &parsedCommand, size_t pfdsindex)
{
    try
    {
        Channel     &channel = getChannel(class_ptr->ChannelsMap, parsedCommand.channel_name);

        if (add == true)
        {
            if (channel._key.empty() == false)
            {
                ERR_KEYSET(class_ptr->pfds[pfdsindex].fd, channel._channel_name)
                throw std::runtime_error("Error: setKey");
            }
            channel._key = parsedCommand.parameter;
        }
        else
            channel._key = parsedCommand.parameter;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    RPL_CHANNELMODEIS(class_ptr->pfds[pfdsindex].fd, parsedCommand.channel_name, parsedCommand.mode, parsedCommand.parameter);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

void    Server::executeModeCommand(size_t pfdsindex, std::vector<std::string> &full_cmd)
{
    try
    {
        t_parsedModeCommand parsedCommand;
        bool                add;

        if (full_cmd.size() == 1)
        {
            ERR_NEEDMOREPARAMS(this->pfds[pfdsindex].fd, full_cmd[0]);
            throw std::runtime_error("Error: executeModeCommand");
        }   
        parsedCommand = parseModeCommand(this, full_cmd[1], pfdsindex);
        add = (parsedCommand.mode[0] == '+');
        if (parsedCommand.mode[1] == 'o')
            setOperator(this, add, pfdsindex, parsedCommand);
        else if (parsedCommand.mode[1] == 'v')
            setVoiced(this, add, pfdsindex, parsedCommand);
        else if (parsedCommand.mode[1] == 'b')
            banClient(this, add, parsedCommand, pfdsindex);
        else if (parsedCommand.mode[1] == 'p')
            setPrivate(this, add, parsedCommand, pfdsindex);
        else if (parsedCommand.mode[1] == 's')
            setSecret(this, add, parsedCommand, pfdsindex);
        else if (parsedCommand.mode[1] == 'i')
            setInviteOnly(this, add, parsedCommand, pfdsindex);
        else if (parsedCommand.mode[1] == 't')
            setOnlyOpsChangeTopic(this, add, parsedCommand, pfdsindex);
        else if (parsedCommand.mode[1] == 'n')
            setNoOutsideMessages(this, add, parsedCommand, pfdsindex);
        else if (parsedCommand.mode[1] == 'm')
            setOnlyVoiceAndOps(this, add, parsedCommand, pfdsindex);
        else if (parsedCommand.mode[1] == 'l')
            setClientLimit(this, add, parsedCommand, pfdsindex, full_cmd[0]);
        else if (parsedCommand.mode[1] == 'k')
            setKey(this, add, parsedCommand, pfdsindex);
        else
        {
            ERR_UNKNOWNMODE(this->pfds[pfdsindex].fd, std::string(1, parsedCommand.mode[1]), parsedCommand.channel_name)
            throw std::runtime_error("Error: unkownMode");
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

/* ---------------------------------------------------------------------------------------------------------------------------------------------------- */

