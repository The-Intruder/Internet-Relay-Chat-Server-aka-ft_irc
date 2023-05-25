/* -------------------------------------------------------------------------- */
/*  File: Mode.source.cpp                                                     */
/*  Brief: Mode source file                                                   */
/*  Authors:                                                                  */
/*   - Mohamed Amine Naimi                                                    */
/* -------------------------------------------------------------------------- */

#include "Mode.source.hpp"

/* -------------------------------------------------------------------------- */

void    executeModeCommand(Server &server, std::string command)
{
    t_parsedModeCommand parsedCommand = parseModeCommand(command);
    bool                add;

    for (std::vector<std::string>::iterator it = parsedCommand.mode.begin(); it != parsedCommand.mode.end(); ++it)
    {
        add = ((*it)[0] == '+');
        for (size_t i = 1; i < (*it).size(); i++)
        {
            if (*it[1] == 'o')
                setOperator(server, add, parsedCommand);
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

static void    setOperator(Server &server, bool add, t_parsedModeCommand &parsedCommand)
{
    std::pair<int,Client>   client = server.getClient(parameter);
    std::map<int,Client>    operators = channel.getOperators();
    Channel                 &channel = server.getChannel(parsedCommand.channel_name);

    if (add == true)
    {
        clients.erase(parsedCommand.parameter);
        operators.insert(std::pair<int,Client>(parsedCommand.parameter,client));
    }
    else
        server._clients[parsedCommand.parameter]._modes &= ~0x01;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

static void   setPrivateMode(Channel &channel, bool add, t_parsedModeCommand &parsedCommand)
{
    if (add == true)
        channel._modes |= 0x01;
    else
        channel._modes &= ~0x01;
}


/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

static void   setSecretMode(Channel &channel, bool add, t_parsedModeCommand &parsedCommand)
{
    if (add == true)
        channel._modes |= 0x02;
    else
        channel._modes &= ~0x02;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

static void   setInviteOnlyMode(Channel &channel, bool add, t_parsedModeCommand &parsedCommand)
{
    if (add == true)
        channel._modes |= 0x04;
    else
        channel._modes &= ~0x04;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

static void   setModeratedMode(Channel &channel, bool add, t_parsedModeCommand &parsedCommand)
{
    if (add == true)
        channel._modes |= 0x08;
    else
        channel._modes &= ~0x08;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

static void   setVoiceAndOpsOnlyMode(Channel &channel, bool add, t_parsedModeCommand &parsedCommand)
{
    if (add == true)
        channel._modes |= 0x10;
    else
        channel._modes &= ~0x10;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

static void   setNoOutsideMessagesMode(Channel &channel, bool add, t_parsedModeCommand &parsedCommand)
{
    if (add == true)
        channel._modes |= 0x20;
    else
        channel._modes &= ~0x20;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

static void   setOnlyOpsChangeTopicMode(Channel &channel, bool add, t_parsedModeCommand &parsedCommand)
{
    if (add == true)
        channel._modes |= 0x40;
    else
        channel._modes &= ~0x40;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

static void   setClientLimitMode(Channel &channel, uint64_t limit, bool add, t_parsedModeCommand &parsedCommand)
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

