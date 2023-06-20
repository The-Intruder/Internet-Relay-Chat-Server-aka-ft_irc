/* ---------------------------------------------------------------------------*/
/*      File: handle_MODE.cpp                                                 */
/*      Brief: Mode source file                                               */
/*      Authors:                                                              */
/*       - Mohamed Amine Naimi                                                */
/* -------------------------------------------------------------------------- */

#include "../ircserv.head.hpp"

/* -------------------------------------------------------------------------- */

Channel   *Server::get_channel(size_t pfdsindex, std::string chan_name)
{
    std::map<std::string, Channel>::iterator it = ChannelsMap.find(chan_name);

    if (it == ChannelsMap.end())
    {
        ERR_NOSUCHCHANNEL(pfdsindex, chan_name);
        throw std::runtime_error("get_channel: channel not found");
    }
    return (&(it->second));
}

/* -------------------------------------------------------------------------- */

std::pair<const int, Client>     &Server::get_client(std::string nickname, \
    Channel* channel, size_t pfdsindex)
{
    std::map<int, Client>::iterator it = channel->_joinedUsers.begin();

    while(it != channel->_joinedUsers.end())
    {
        if(it->second.getNICKNAME() == nickname)
            break;
        it++;
    }
    if (it == channel->_joinedUsers.end())
    {
        ERR_USERNOTINCHANNEL(pfdsindex, nickname, channel->_channel_name);
        throw std::runtime_error("ERR_USERNOTINCHANNEL: User not in channel");
    }
    return (*(it));
}

/* -------------------------------------------------------------------------- */

t_pmc  Server::parse_mode_command(std::string &command, size_t pfdsindex, \
    std::string username)
{
    t_pmc                   parsed_command;
    std::string             channel_name;
    std::string::size_type  pos = 0;
    std::string::size_type  pos2 = 0;

    std::string cmd = "MODE";
    pos = command.find(" ");
    if (pos == std::string::npos)
        channel_name = command.substr(0);
    else
        channel_name = command.substr(0, pos++);
    parsed_command.channel = get_channel(pfdsindex, channel_name);
    while (pos < command.length() && command[pos] == ' ')
        ++pos;
    if (pos >= command.length() || pos + 1 >= command.length() \
        || (command[pos] != '+' && command[pos] != '-') \
        || !std::isalpha(command[pos + 1]))
    {
        RPL_CHANNELMODEIS(pfdsindex, channel_name, username, "+-ovbpsitnmlk");
        throw std::runtime_error("ERR_NEEDMOREPARAMS: Needs more parametres");
    }
    parsed_command.mode = command.substr(pos, 2);
    pos = command.find(" ", pos);
    if (pos == std::string::npos)
        return (parsed_command);
    while (pos < command.length() && command[pos] == ' ')
        ++pos;
    pos2 = command.find(" ", pos);
    if (command[pos] == ':')
        parsed_command.parameter = command.substr(pos + 1, command.length() - (pos + 1));
    else if (pos2 == std::string::npos)
        parsed_command.parameter = command.substr(pos, command.length() - pos);
    else
        parsed_command.parameter = command.substr(pos + 1, pos2 - (pos + 1));
    return (parsed_command);
}

/* -------------------------------------------------------------------------- */

void    Server::set_operator(bool add, size_t pfdsindex, t_pmc &pmc, \
    std::string username)
{
    std::pair<const int,Client> &client = get_client(pmc.parameter, \
        pmc.channel, pfdsindex);

    if (add == true)
        pmc.channel->_admins.insert(client);
    else
        pmc.channel->_admins.erase(client.first);

    for (std::map<int, Client>::iterator it = pmc.channel->_joinedUsers.begin(); \
        it != pmc.channel->_joinedUsers.end(); ++it)
    {
        RPL_CHANNELMODEIS(it->second.getSockfd(), pmc.channel->_channel_name, username, \
            pmc.mode + " " + pmc.parameter);
    }
}

/* -------------------------------------------------------------------------- */

void     Server::set_voiced(bool add, size_t pfdsindex, t_pmc &pmc, \
    std::string username)
{
    std::pair<const int,Client>   &client = get_client(pmc.parameter, \
        pmc.channel, pfdsindex);

    if (add == true)
        pmc.channel->_voicedClients.insert(client);
    else
        pmc.channel->_voicedClients.erase(client.first);

    for (std::map<int, Client>::iterator it = pmc.channel->_joinedUsers.begin(); \
        it != pmc.channel->_joinedUsers.end(); ++it)
    {
        RPL_CHANNELMODEIS(it->second.getSockfd(), pmc.channel->_channel_name, username, \
            pmc.mode + " " + pmc.parameter);
    }
}


/* -------------------------------------------------------------------------- */

void     Server::ban_client(bool add, t_pmc &pmc, size_t pfdsindex, \
    std::string username)
{
    std::pair<const int,Client> &client = get_client(pmc.parameter, \
        pmc.channel, pfdsindex);

    if (add == true)
    {
        pmc.channel->_admins.erase(client.first);
        pmc.channel->_voicedClients.erase(client.first);
        pmc.channel->_bannedUsers.insert(client);
    }
    else
        pmc.channel->_bannedUsers.erase(client.first);

    for (std::map<int, Client>::iterator it = pmc.channel->_joinedUsers.begin(); \
        it != pmc.channel->_joinedUsers.end(); ++it)
    {
        RPL_CHANNELMODEIS(it->second.getSockfd(), pmc.channel->_channel_name, username, \
            pmc.mode + " " + pmc.parameter);
    }
}

/* -------------------------------------------------------------------------- */

void    Server::set_private(bool add, t_pmc &pmc, std::string username)
{
    if (add == true)
        pmc.channel->_modes |= 0x01;
    else
        pmc.channel->_modes &= ~0x01;

    for (std::map<int, Client>::iterator it = pmc.channel->_joinedUsers.begin(); \
        it != pmc.channel->_joinedUsers.end(); ++it)
    {
        RPL_CHANNELMODEIS(it->second.getSockfd(), pmc.channel->_channel_name, username, \
            pmc.mode);
    }
}

/* -------------------------------------------------------------------------- */

void    Server::set_secret(bool add, t_pmc &pmc, std::string username)
{
    if (add == true)
        pmc.channel->_modes |= 0x02;
    else
        pmc.channel->_modes &= ~0x02;
    for (std::map<int, Client>::iterator it = pmc.channel->_joinedUsers.begin(); \
        it != pmc.channel->_joinedUsers.end(); ++it)
    {
        RPL_CHANNELMODEIS(it->second.getSockfd(), pmc.channel->_channel_name, username, \
            pmc.mode);
    }
}

/* -------------------------------------------------------------------------- */

void    Server::set_invite_only(bool add, t_pmc &pmc, std::string username)
{
    if (add == true)
        pmc.channel->_modes |= 0x04;
    else
        pmc.channel->_modes &= ~0x04;

    for (std::map<int, Client>::iterator it = pmc.channel->_joinedUsers.begin(); \
        it != pmc.channel->_joinedUsers.end(); ++it)
    {
        RPL_CHANNELMODEIS(it->second.getSockfd(), pmc.channel->_channel_name, username, \
        pmc.mode);
    }
}

/* -------------------------------------------------------------------------- */

void    Server::set_only_ops_change_topic(bool add, t_pmc &pmc, \
    std::string username)
{
    if (add == true)
        pmc.channel->_modes |= 0x20;
    else
        pmc.channel->_modes &= ~0x20;

    for (std::map<int, Client>::iterator it = pmc.channel->_joinedUsers.begin(); \
        it != pmc.channel->_joinedUsers.end(); ++it)
    {
        RPL_CHANNELMODEIS(it->second.getSockfd(), pmc.channel->_channel_name, username, \
            pmc.mode);
    }
}

/* -------------------------------------------------------------------------- */

void    Server::set_no_outside_messages(bool add, t_pmc &pmc, \
    std::string username)
{
    if (add == true)
        pmc.channel->_modes |= 0x10;
    else
        pmc.channel->_modes &= ~0x10;

    for (std::map<int, Client>::iterator it = pmc.channel->_joinedUsers.begin(); \
        it != pmc.channel->_joinedUsers.end(); ++it)
    {
        RPL_CHANNELMODEIS(it->second.getSockfd(), pmc.channel->_channel_name, username, \
            pmc.mode);
    }
}

/* -------------------------------------------------------------------------- */

void     Server::set_only_voice_and_ops(bool add, t_pmc &pmc, \
    std::string username)
{
    if (add == true)
        pmc.channel->_modes |= 0x08;
    else
        pmc.channel->_modes &= ~0x08;

    for (std::map<int, Client>::iterator it = pmc.channel->_joinedUsers.begin(); \
        it != pmc.channel->_joinedUsers.end(); ++it)
    {
        RPL_CHANNELMODEIS(it->second.getSockfd(), pmc.channel->_channel_name, username, \
            pmc.mode);
    }
}

/* -------------------------------------------------------------------------- */

void   Server::set_client_limit(bool add, t_pmc &pmc, std::string cmd, \
    size_t pfdsindex, std::string username)
{
    if (add == true)
        pmc.channel->_modes |= 0x40;
    else
        pmc.channel->_modes &= ~0x40;
    if (!std::isdigit(pmc.parameter[0]))
    {
        ERR_NEEDMOREPARAMS(pfdsindex, cmd);
        throw std::runtime_error("ERR_NEEDMOREPARAMS: Needs more parametres");
    }
    pmc.channel->_client_limit = std::atoi(pmc.parameter.c_str());

    for (std::map<int, Client>::iterator it = pmc.channel->_joinedUsers.begin(); \
        it != pmc.channel->_joinedUsers.end(); ++it)
    {
        RPL_CHANNELMODEIS(it->second.getSockfd(), pmc.channel->_channel_name, \
            username, pmc.mode + " " + pmc.parameter);
    }
}

/* -------------------------------------------------------------------------- */

void    Server::set_key(bool add, t_pmc &pmc, std::string username, \
    size_t pfdsindex)
{
    if (add == true)
    {
        if (pmc.channel->_channel_pass.empty() == false)
        {
            ERR_KEYSET(pfdsindex, pmc.channel->_channel_name)
            throw std::runtime_error("ERR_KEYSET: Key already set");
        }
        pmc.channel->_channel_pass = pmc.parameter;
    }
    else if (pmc.parameter == pmc.channel->_channel_pass)
        pmc.channel->_channel_pass.clear();
    std::cout << "KEY: " << pmc.channel->_channel_pass << std::endl;

    for (std::map<int, Client>::iterator it = pmc.channel->_joinedUsers.begin(); \
        it != pmc.channel->_joinedUsers.end(); ++it)
    {
        RPL_CHANNELMODEIS(pfdsindex, pmc.channel->_channel_name, username, \
            pmc.mode + " " + pmc.parameter);
    }
}

/* -------------------------------------------------------------------------- */

void    Server::is_operator(Channel& channel, size_t pfdsindex)
{
    std::map<int,Client>::iterator it1 = channel._joinedUsers.find(pfds[pfdsindex].fd);

    if (it1 == channel._joinedUsers.end())
    {
        ERR_NOTONCHANNEL(pfdsindex, channel._channel_name)
        throw std::runtime_error("ERR_NOTONCHANNEL: Not on the channel");
    }
    std::map<int,Client>::iterator it2 = channel._admins.find(pfds[pfdsindex].fd);

    if (it2 == channel._admins.end())
    {
        ERR_CHANOPRIVSNEEDED(pfdsindex, channel._channel_name);
        throw std::runtime_error("ERR_CHANOPRIVSNEEDED: Not an operator");
    }
}

/* -------------------------------------------------------------------------- */

void    Server::execute_mode_command(size_t pfdsindex, \
    std::vector<std::string> &full_cmd)
{
    t_pmc   pmc;
    bool    add;
    std::string cmd = "MODE";

    try
    {
        if (full_cmd.size() == 1)
        {
            ERR_NEEDMOREPARAMS(pfdsindex, cmd);
            throw std::runtime_error("ERR_NEEDMOREPARAMS: Needs more parametres");
        }
        std::map<int, Client>::iterator it = ClientsMap.find(pfds[pfdsindex].fd);
        std::string username = it->second.getUSERNAME();
        pmc = parse_mode_command(full_cmd[1], pfdsindex, username);
        is_operator(*(pmc.channel), pfdsindex);
        add = (pmc.mode[0] == '+');
        if (pmc.mode[1] == 'o')
            set_operator(add, pfdsindex, pmc, username);
        else if (pmc.mode[1] == 'v')
            set_voiced(add, pfdsindex, pmc, username);
        else if (pmc.mode[1] == 'b')
            ban_client(add, pmc, pfdsindex, username);
        else if (pmc.mode[1] == 'p')
            set_private(add, pmc, username);
        else if (pmc.mode[1] == 's')
            set_secret(add, pmc, username);
        else if (pmc.mode[1] == 'i')
            set_invite_only(add, pmc, username);
        else if (pmc.mode[1] == 't')
            set_only_ops_change_topic(add, pmc, username);
        else if (pmc.mode[1] == 'n')
            set_no_outside_messages(add, pmc, username);
        else if (pmc.mode[1] == 'm')
            set_only_voice_and_ops(add, pmc, username);
        else if (pmc.mode[1] == 'l')
            set_client_limit(add, pmc, full_cmd[0], pfdsindex, username);
        else if (pmc.mode[1] == 'k')
            set_key(add, pmc, username, pfdsindex);
        else
        {
            ERR_UNKNOWNMODE(pfdsindex, pmc.mode, pmc.channel->_channel_name)
            throw std::runtime_error("ERR_UNKNOWNMODE: Unknown mode");
        }
    }
    catch(const std::exception& e)
    {
        // std::cerr << e.what() << std::endl;
    }
    
}

// FIXME: message gets sent to the wrong client
// FIXED: mode reply message should get broadcasted to all the clients
// FIXED: invite should put clients in an invite list
// FIXED: invalid channel doesn't output an error msg

// FIXME: PRIVMSG doesn't send msg to public channel
// FIXME: JOIN needs to join client if its on the invite list
