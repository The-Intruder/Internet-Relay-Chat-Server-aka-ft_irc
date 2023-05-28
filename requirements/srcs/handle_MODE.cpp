/* -------------------------------------------------------------------------- */
/*  File: handle_MODE.cpp                                                     */
/*  Brief: Mode source file                                                   */
/*  Authors:                                                                  */
/*   - Mohamed Amine Naimi                                                    */
/* -------------------------------------------------------------------------- */

#include "../ircserv.head.hpp"

/* -------------------------------------------------------------------------- */
// FIXME: need to omit he # before the channelName

static t_parsedModeCommand  parseModeCommand(std::string const &command)
{
    t_parsedModeCommand    parsedCommand;
    std::string::size_type pos = 0;

    try
    {
        pos = command.find(' ');
        parsedCommand.channel_name = command.substr(1, pos);
        pos = command.find(' ', pos + 1);
        while (command[pos + 1] == '+' || command[pos + 1] == '-')
        {
            parsedCommand.mode.push_back(command.substr(pos + 1, command.find(' ', pos + 1)));
            pos = command.find(' ', pos + 1);
        }
        pos = command.find(' ', pos + 1);
        parsedCommand.parameter = command.substr(pos + 1);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return (parsedCommand);
}

/* -------------------------------------------------------------------------- */

static std::pair<const int, Client>     &getClient(std::map<int, Client> &ClientsMap, \
    std::string nickname)
{
    std::map<int, Client>::iterator it = ClientsMap.begin();

    while(it != ClientsMap.end())
    {
        if(it->second.getNICKNAME() == nickname)
            break;
        it++;
    }
    if (it == ClientsMap.end())
        throw std::runtime_error("client not found");
    return (*(it));
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static Channel   &getChannel(std::map<std::string, Channel> &Channels, \
    std::string channelName)
{
    std::map<std::string, Channel>::iterator it = Channels.find(channelName);

    if (it == Channels.end())
        throw std::runtime_error("channel not found");
    return (it->second);
}

/* -------------------------------------------------------------------------- */

static void    setOperator(Server *class_ptr, bool add, \
    t_parsedModeCommand &parsedCommand)
{
    try
    {
        std::pair<const int,Client>   &client = getClient(class_ptr->ClientsMap, \
            parsedCommand.parameter);
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

/* -------------------------------------------------------------------------- */

static void     setVoiced(Server *class_ptr, bool add, \
    t_parsedModeCommand &parsedCommand)
{
    try
    {
        std::pair<const int,Client>   &client = getClient(class_ptr->ClientsMap, \
            parsedCommand.parameter);
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

/* -------------------------------------------------------------------------- */

static std::pair<const int, Client>     &getClientFromNicknameHostAddress( \
    Server *class_ptr, std::string nicknameHostAddress)
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

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static void     banClient(Server *class_ptr, bool add, t_parsedModeCommand &parsedCommand)
{
    try
    {
        Channel     &channel = getChannel(class_ptr->ChannelsMap, parsedCommand.channel_name);
        std::pair<const int,Client>   &client = getClientFromNicknameHostAddress(class_ptr, \
            parsedCommand.parameter);
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

/* -------------------------------------------------------------------------- */

static void    setPrivate(Server *class_ptr, bool add, t_parsedModeCommand &parsedCommand)
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

/* -------------------------------------------------------------------------- */

static void    setSecret(Server *class_ptr, bool add, t_parsedModeCommand &parsedCommand)
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

/* -------------------------------------------------------------------------- */

static void    setInviteOnly(Server *class_ptr, bool add, t_parsedModeCommand &parsedCommand)
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

/* -------------------------------------------------------------------------- */

static void    setOnlyOpsChangeTopic(Server *class_ptr, bool add, t_parsedModeCommand &parsedCommand)
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

/* -------------------------------------------------------------------------- */

static void    setNoOutsideMessages(Server *class_ptr, bool add, t_parsedModeCommand &parsedCommand)
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

/* -------------------------------------------------------------------------- */

static void     setOnlyVoiceAndOps(Server *class_ptr, bool add, t_parsedModeCommand &parsedCommand)
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

static void setClientLimit(Server *class_ptr, bool add, t_parsedModeCommand &parsedCommand)
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
        std::cerr << e.what() << std::endl;
    }
}

/* -------------------------------------------------------------------------- */

void    Server::executeModeCommand(std::string command)
{
    t_parsedModeCommand parsedCommand = parseModeCommand(command);
    bool                add;

    std::cout << command << std::endl;
    for (std::vector<std::string>::iterator it = parsedCommand.mode.begin(); \
        it != parsedCommand.mode.end(); ++it)
    {
        add = ((*it)[0] == '+');
        for (size_t i = 1; i < (*it).size(); i++)
        {
            if ((*it)[i] == 'o')
                setOperator(this, add, parsedCommand);
            else if ((*it)[i] == 'v')
                setVoiced(this, add, parsedCommand);
            else if ((*it)[i] == 'b')
                banClient(this, add, parsedCommand);
            else if ((*it)[i] == 'p')
                setPrivate(this, add, parsedCommand);
            else if ((*it)[i] == 's')
                setSecret(this, add, parsedCommand);
            else if ((*it)[i] == 'i')
                setInviteOnly(this, add, parsedCommand);
            else if ((*it)[i] == 't')
                setOnlyOpsChangeTopic(this, add, parsedCommand);
            else if ((*it)[i] == 'n')
                setNoOutsideMessages(this, add, parsedCommand);
            else if ((*it)[i] == 'm')
                setOnlyVoiceAndOps(this, add, parsedCommand);
            else if ((*it)[i] == 'l')
                setClientLimit(this, add, parsedCommand);
            else if ((*it)[i] == 'k')
                ;
        }
    }
}

/* -------------------------------------------------------------------------- */