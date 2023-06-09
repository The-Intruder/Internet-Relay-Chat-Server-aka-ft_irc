/* ---------------------------------------------------------------------------------------------------------------------------------------------------- */
/*      File: handle_MODE.cpp                                                                                                                           */
/*      Brief: Mode source file                                                                                                                         */
/*      Authors:                                                                                                                                        */
/*       - Mohamed Amine Naimi                                                                                                                          */
/* ---------------------------------------------------------------------------------------------------------------------------------------------------- */

#include "../ircserv.head.hpp"

/* ---------------------------------------------------------------------------------------------------------------------------------------------------- */

static t_parsedModeCommand  parseModeCommand(std::string const &command, size_t pfdsindex)
{
    t_parsedModeCommand    parsedCommand;
    std::string::size_type pos = 0;

    // Check if the channel name is the only thing in the passed command
    pos = command.find(' ');
    if (pos == std::string::npos)
    {
        ERR_NEEDMOREPARAMS(pfdsindex, command[0]);
        throw std::runtime_error("");
    }

    // Parsing the channel_name itself
    if (command[1] == '#')
        parsedCommand.channel_name = command.substr(1, pos - 1);
    else
        parsedCommand.channel_name = command.substr(0, pos);

    // Skipping all the space characters
    while ( pos < command.length() && command[pos] == ' ')
        pos++;

    // check if there is nothing after the channel_name
    pos = command.find(' ', pos);
    if (pos + 1 >= command.length())
    {
        ERR_NEEDMOREPARAMS(pfdsindex, command[0]);
        throw std::runtime_error("");
    }

    if ((command[pos + 1] == '+' || command[pos + 1] == '-') && pos + 2 < command.length())
    {
        parsedCommand.mode += std::string(1, command[++pos]);
        if (!std:isalpha(command[pos + 1]))
        {
            ERR_UNKNOWNMODE(pfdsindex, command[pos + 1], parsedCommand.channel_name);
            throw std::runtime_error("");
        }
        parsedCommand.mode += std::string(1, command[++pos]);
    }

    while (command[pos] == ' ' && pos < command.length())
        pos++;
    if (command[pos] == ':')
        parsedCommand.parameter = command.substr(pos + 1);
    else
    {

        parsedCommand.parameter = command.substr(pos, command.find(' ', pos) - pos);
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

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static Channel   &getChannel(std::map<std::string, Channel> &Channels, std::string channelName)
{
    std::map<std::string, Channel>::iterator it = Channels.find(channelName);

    if (it == Channels.end())
        throw std::runtime_error("channel not found");
    return (it->second);
}

/* ---------------------------------------------------------------------------------------------------------------------------------------------------- */

static void    setOperator(Server *class_ptr, bool add, size_t pfdsindex, t_parsedModeCommand &parsedCommand)
{
    try
    {
        std::pair<const int,Client>   &client = getClient(class_ptr->ClientsMap, parsedCommand.parameter);
        Channel     &channel = getChannel(class_ptr->ChannelsMap, parsedCommand.channel_name);
        if (add == true)
        {
            channel._joinedClients.erase(client.first);
            channel._operators.insert(client);
        }
        else
        {
            channel._operators.erase(client.first);
            channel._joinedClients.insert(client);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

/* ---------------------------------------------------------------------------------------------------------------------------------------------------- */

static void     setVoiced(Server *class_ptr, bool add, , size_t pfdsindex, t_parsedModeCommand &parsedCommand)
{
    try
    {
        std::pair<const int,Client>   &client = getClient(class_ptr->ClientsMap, parsedCommand.parameter);
        Channel     &channel = getChannel(class_ptr->ChannelsMap, parsedCommand.channel_name);
        if (add == true)
        {
            channel._joinedClients.erase(client.first);
            channel._voicedClients.insert(client);
        }
        else
        {
            channel._voicedClients.erase(client.first);
            channel._joinedClients.insert(client);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

/* ---------------------------------------------------------------------------------------------------------------------------------------------------- */

static std::pair<const int, Client>     &getClientFromNicknameHostAddress(Server *class_ptr, std::string nicknameHostAddress)
{
    std::map<int, Client>::iterator it = class_ptr->ClientsMap.begin();
    std::string nickname = nicknameHostAddress.substr(0, nicknameHostAddress.find('!'));
    std::string address = nicknameHostAddress.substr(nicknameHostAddress.find('@') + 1);

    while(it != class_ptr->ClientsMap.end())
    {
        if(it->second.getNICKNAME() == nickname && it->second.getIP() == address)
            break;
        it++;
    }
    if (it == class_ptr->ClientsMap.end())
        throw std::runtime_error("client not found");
    return (*it);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static void     banClient(Server *class_ptr, bool add, t_parsedModeCommand &parsedCommand, size_t pfdsindex)
{
    try
    {
        Channel     &channel = getChannel(class_ptr->ChannelsMap, parsedCommand.channel_name);
        std::pair<const int,Client>   &client = getClientFromNicknameHostAddress(class_ptr, parsedCommand.parameter);
        {
            channel._operators.erase(client.first);
            channel._voicedClients.erase(client.first);
            channel._joinedClients.erase(client.first);
            channel._bannedClients.insert(client);
        }
        else
        {
            channel._bannedClients.erase(client.first);
            channel._joinedClients.insert(client);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
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
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static void setClientLimit(Server *class_ptr, bool add, t_parsedModeCommand &parsedCommand, size_t pfdsindex)
{
    try
    {
        Channel     &channel = getChannel(class_ptr->ChannelsMap, parsedCommand.channel_name);

        if (add == true)
            channel._modes |= 0x40;
        else
            channel._modes &= ~0x40;
        channel._clientLimit = std::stoi(parsedCommand.parameter);
    }
    catch(const std::exception& e)
    {
        if (dynamic_cast<const std::invalid_argument*>(&e) != nullptr || dynamic_cast<const std::out_of_range*>(&e) != nullptr)
            ERR_NEEDMOREPARAMS(pfdsindex, command[0]);
        else
            std::cerr << e.what() << std::endl;
    }
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
                ERR_KEYSET(pfdsindex, channel._channel_name)
                throw std::exception("");
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
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

void    Server::executeModeCommand(size_t pfdsindex, std::vector<std::string> &full_cmd)
{
    t_parsedModeCommand parsedCommand;
    bool                add;

    try
    {
        if (full_cmd.size() == 1)
        {
            ERR_NEEDMOREPARAMS(pfdsindex, "MODE");
            throw std::runtime_error("");
        }
        std::string command = full_cmd[1];    
        parsedCommand = parseModeCommand(command, pfdsindex)
        add = (parsedCommand.mode[0] == '+');
        if (parsedCommand.mode[1] == 'o')            // FIXME: check if user is in channel, if not, send ERR_USERNOTINCHANNEL, and also check if user is operator, NEEDS A PARAMETER
            setOperator(this, add, parsedCommand, pfdsindex);
        else if (parsedCommand.mode[1] == 'v')       // FIXME: check if user is in channel, if not, send ERR_USERNOTINCHANNEL, and also check if user is already voiced, NEEDS A PARAMETER
            setVoiced(this, add, parsedCommand, pfdsindex);
        else if (parsedCommand.mode[1] == 'b')       // FIXME: check if user is in channel, if not, send ERR_USERNOTINCHANNEL, and also check if user is already banned, NEEDS A PARAMETER
            banClient(this, add, parsedCommand, pfdsindex);
        else if (parsedCommand.mode[1] == 'p')       // FIXME: DOESNT NEED A PARAMETER
            setPrivate(this, add, parsedCommand, pfdsindex);
        else if (parsedCommand.mode[1] == 's')       // FIXME: DOESNT NEED A PARAMETER
            setSecret(this, add, parsedCommand, pfdsindex);
        else if (parsedCommand.mode[1] == 'i')       // FIXME: DOESNT NEED A PARAMETER
            setInviteOnly(this, add, parsedCommand, pfdsindex);
        else if (parsedCommand.mode[1] == 't')       // FIXME: DOESNT NEED A PARAMETER
            setOnlyOpsChangeTopic(this, add, parsedCommand, pfdsindex);
        else if (parsedCommand.mode[1] == 'n')       // FIXME: DOESNT NEED A PARAMETER
            setNoOutsideMessages(this, add, parsedCommand, pfdsindex);
        else if (parsedCommand.mode[1] == 'm')       // FIXME: DOESNT NEED A PARAMETER
            setOnlyVoiceAndOps(this, add, parsedCommand, pfdsindex);
        else if (parsedCommand.mode[1] == 'l')       // FIXME: check if the parametre is a number, if not, send ERR_NEEDMOREPARAMS, NEEDS A PARAMETER
            setClientLimit(this, add, parsedCommand, pfdsindex);
        else if (parsedCommand.mode[1] == 'k')
            setKey(this, add, parsedCommand, pfdsindex);
        else
        {
            ERR_UMODEUNKNOWNFLAG(pfdsindex, (*it)[i])
            throw std::exception();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

/* ---------------------------------------------------------------------------------------------------------------------------------------------------- */