/* -------------------------------------------------------------------------- */
/*  File: Mode.source.cpp                                                     */
/*  Brief: Mode source file                                                   */
/*  Authors:                                                                  */
/*   - Mohamed Amine Naimi                                                    */
/* -------------------------------------------------------------------------- */

#include "Mode.source.hpp"

/* -------------------------------------------------------------------------- */

void    executeModeCommand(std::string command)
{
    t_parsedModeCommand parsedCommand = parseModeCommand(command);
    bool                add;

    for (std::vector<std::string>::iterator it = parsedCommand.mode.begin(); it != parsedCommand.mode.end(); ++it)
    {
        add = ((*it)[0] == '+');
        for (size_t i = 1; i < (*it).size(); i++)
        {
            if (*it[1] == 'o')
                setOperator(this, add, parsedCommand);
            else if (*it[1] == 'v')
                ;
            else if (*it[1] == 'p')
                ;
            else if (*it[1] == 's')
                ;
            else if (*it[1] == 'i')
                ;
            else if (*it[1] == 'm')
                ;
            else if (*it[1] == 'n')
                ;
            else if (*it[1] == 't')
                ;
            else if (*it[1] == 'l')
                ;
        }
    }
}

/* -------------------------------------------------------------------------- */

static std::pair<int, Client>     &getClient(Server *this, std::string nickname)
{
    std::map<int, Client>::iterator it = this->ClientsMap.begin();
    while(it != this->ClientsMap.end())
    {
        if(it->second.getNICKNAME() == nickname)
            break;
        it++;
    }
    if (it == this->ClientsMap.end())
        throw std::runtime_error("client not found");
    return (*(it));
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

static Channel   &getChannel(Server *this, std::string channelName)
{
    std::map<std::string, Channel>::iterator it = this->myMap.find(keyToFind);

    if (it == this->myMap.end())
        throw std::runtime_error("channel not found");
    return (*it);
}

/* -------------------------------------------------------------------------- */

static void    setOperator(Server *this, bool add, t_parsedModeCommand &parsedCommand)
{
    try
    {
        Channel &channel = this->_channels.at(parsedCommand.parameter);
        std::pair<int,Client> &client = getClient(this, parsedCommand.nickname);
        if (add == true)
        {
            clients.erase(client.first);
            operators.insert(client);
        }
        else
        {
            operators.erase(client.first);
            clients.insert(client);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

static void   setPrivateMode(bool add, t_parsedModeCommand &parsedCommand)
{
    if (add == true)
        channel._modes |= 0x01;
    else
        channel._modes &= ~0x01;
}


/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

static void   setSecretMode(bool add, t_parsedModeCommand &parsedCommand)
{
    if (add == true)
        channel._modes |= 0x02;
    else
        channel._modes &= ~0x02;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

static void   setInviteOnlyMode(bool add, t_parsedModeCommand &parsedCommand)
{
    if (add == true)
        channel._modes |= 0x04;
    else
        channel._modes &= ~0x04;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

static void   setModeratedMode(bool add, t_parsedModeCommand &parsedCommand)
{
    if (add == true)
        channel._modes |= 0x08;
    else
        channel._modes &= ~0x08;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

static void   setVoiceAndOpsOnlyMode(bool add, t_parsedModeCommand &parsedCommand)
{
    if (add == true)
        channel._modes |= 0x10;
    else
        channel._modes &= ~0x10;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

static void   setNoOutsideMessagesMode(bool add, t_parsedModeCommand &parsedCommand)
{
    if (add == true)
        channel._modes |= 0x20;
    else
        channel._modes &= ~0x20;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

static void   setOnlyOpsChangeTopicMode(bool add, t_parsedModeCommand &parsedCommand)
{
    if (add == true)
        channel._modes |= 0x40;
    else
        channel._modes &= ~0x40;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

static void   setClientLimitMode(uint64_t limit, bool add, t_parsedModeCommand &parsedCommand)
{
    if (add == true)
        channel._modes |= 0x80;
    else
        channel._modes &= ~0x80;
    channel._client_limit = limit;
}

/* -------------------------------------------------------------------------- */

static t_parsedModeCommand  parseModeCommand(std::string const &command)
{
    t_parsedModeCommand    parsedCommand;
    std::string::size_type pos = 0;

    try
    {
        pos = command.find(' ');
        parsedCommand.channel_name = command.substr(0, pos);
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

