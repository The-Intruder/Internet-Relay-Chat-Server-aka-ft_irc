/* -------------------------------------------------------------------------- */
/*  File: Mode.source.cpp                                                     */
/*  Brief: Mode source file                                                   */
/*  Authors:                                                                  */
/*   - Mohamed Amine Naimi                                                    */
/* -------------------------------------------------------------------------- */

#include "../headers/Mode.source.hpp"

/* -------------------------------------------------------------------------- */

void    executeModeCommand(Server &server, std::string command)
{
    t_parsedModeCommand parsedCommand = parseModeCommand(command);
    bool                add;

    for (std::vector<std::string>::iterator it = parsedCommand.mode.begin(); \
        it != parsedCommand.mode.end(); ++it)
    {
        add = (*it)[0] == '+';
        if (*it[1] == 'o')
            setOperatorMode(server.getChannel(parsedCommand.channel_name), client, add);
        else if (*it[1] == 'v')
            setVoiceMode(server.getChannel(parsedCommand.channel_name), client, add);
        else if (*it[1] == 'p')
            setPrivateMode(server.getChannel(parsedCommand.channel_name), add);
        else if (*it[1] == 's')
            setSecretMode(server.getChannel(parsedCommand.channel_name), add);
        else if (*it[1] == 'i')
            setInviteOnlyMode(server.getChannel(parsedCommand.channel_name), add);
        else if (*it[1] == 'm')
            setVoiceAndOpsOnlyMode(server.getChannel(parsedCommand.channel_name), add);
        else if (*it[1] == 'n')
            setNoOutsideMessagesMode(server.getChannel(parsedCommand.channel_name), add);
        else if (*it[1] == 't')
            setOnlyOpsChangeTopicMode(server.getChannel(parsedCommand.channel_name), add);
        else if (*it[1] == 'l')
            setClientLimitMode(server.getChannel(parsedCommand.channel_name), std::stoul(parsedCommand.parameter), add);
    }
}

/* -------------------------------------------------------------------------- */

std::vector<std::string>   getAllTheModesInInput(std::string const &input)
{
    std::vector<std::string>    modes;
    std::string::size_type      pos = 0;

    while ((pos = input.find(' ', pos)) != std::string::npos)
    {
        if (input[pos + 1] == '+')
        {
            for (std::string::size_type i = pos + 2; i < input.size() && input[i] != ' '; ++i)
                modes.push_back("+" + input.substr(i, 1));
        }
        else if (input[pos + 1] == '-')
        {
            for (std::string::size_type i = pos + 2; i < input.size() && input[i] != ' '; ++i)
                modes.push_back("-" + input.substr(i, 1));
        }
        ++pos;
    }
    return (modes);
}

/* -------------------------------------------------------------------------- */

t_parsedModeCommand  parseModeCommand(std::string const &command)
{
    t_parsedModeCommand    parsedCommand;
    std::string::size_type pos = 0;

    try
    {
        pos = command.find(' ');
        parsedCommand.channel_name = command.substr(0, pos);
        pos = command.find(' ', pos + 1);
        parsedCommand.mode = getAllTheModesInInput(command.substr(pos + 1));
        pos = command.find(' ', pos + 1);
        parsedCommand.parameter = command.substr(pos + 1);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return (parsedCommand);
}

/* -------------------------------------------------------------------------- */

void   setPrivateMode(Channel &channel, bool add)
{
    if (add == true)
        channel._modes |= 0x01;
    else
        channel._modes &= ~0x01;
}


/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void   setSecretMode(Channel &channel, bool add)
{
    if (add == true)
        channel._modes |= 0x02;
    else
        channel._modes &= ~0x02;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void   setInviteOnlyMode(Channel &channel, bool add)
{
    if (add == true)
        channel._modes |= 0x04;
    else
        channel._modes &= ~0x04;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void   setModeratedMode(Channel &channel, bool add)
{
    if (add == true)
        channel._modes |= 0x08;
    else
        channel._modes &= ~0x08;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void   setVoiceAndOpsOnlyMode(Channel &channel, bool add)
{
    if (add == true)
        channel._modes |= 0x10;
    else
        channel._modes &= ~0x10;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void   setNoOutsideMessagesMode(Channel &channel, bool add)
{
    if (add == true)
        channel._modes |= 0x20;
    else
        channel._modes &= ~0x20;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void   setOnlyOpsChangeTopicMode(Channel &channel, bool add)
{
    if (add == true)
        channel._modes |= 0x40;
    else
        channel._modes &= ~0x40;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void   setClientLimitMode(Channel &channel, uint64_t limit, bool add)
{
    if (add == true)
        channel._modes |= 0x80;
    else
        channel._modes &= ~0x80;
    channel._client_limit = limit;
}

