/* -------------------------------------------------------------------------- */
/*  File: handle_INVITE.cpp                                                   */
/*  Brief: Invite source file                                                 */
/*  Authors:                                                                  */
/*   - Mohamed Amine Naimi                                                    */
/* -------------------------------------------------------------------------- */

#include "../ircserv.head.hpp"

/* -------------------------------------------------------------------------- */

int     Server::get_client_fd(std::string nickname, \
    size_t pfdsindex)
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
        ERR_NOSUCHNICK(pfdsindex, nickname);
        throw std::runtime_error("ERR_NOSUCHNICK: No such nickname");
    }
    return (it->first);
}

/* -------------------------------------------------------------------------- */

t_pic  Server::parse_invite_command(std::string &command, size_t pfdsindex)
{
    std::string::size_type  pos2 = 0;
    std::string::size_type  pos = 0;
    t_pic           parsed_command;
    std::string     nickname;
    std::string     channel_name;

    std::string cmd = "INVITE";
    pos = command.find(" ");
    if (pos == std::string::npos)
    {
        ERR_NEEDMOREPARAMS(pfdsindex, cmd);
        throw std::runtime_error("ERR_NEEDMOREPARAMS: Needs more parametres");
    }
    nickname = command.substr(0, pos++);
    parsed_command.clients_fd = get_client_fd(nickname, pfdsindex);
    while (pos < command.length() && command[pos] == ' ')
        ++pos;
    if (pos >= command.length())
    {
        ERR_NEEDMOREPARAMS(pfdsindex, cmd);
        throw std::runtime_error("ERR_NEEDMOREPARAMS: Needs more parametres");
    }
    pos2 = command.find(" ", pos);
    if (command[pos] == ':')
        channel_name = command.substr(pos + 1, command.length() - (pos + 1));
    else if (pos2 == std::string::npos)
        channel_name = command.substr(pos, command.length() - pos);
    else
        channel_name = command.substr(pos + 1, pos2 - (pos + 1));
    parsed_command.channel = get_channel(pfdsindex, channel_name);
    return (parsed_command);
}

/* -------------------------------------------------------------------------- */

void    Server::is_invite_operator(Channel& channel, size_t pfdsindex)
{
    std::map<int,Client>::iterator it = channel._joinedUsers.find(pfds[pfdsindex].fd);

    if (it == channel._joinedUsers.end())
    {
        ERR_NOTONCHANNEL(pfdsindex, channel._channel_name)
        throw std::runtime_error("ERR_NOTONCHANNEL: Not on the channel");
    }
}

/* -------------------------------------------------------------------------- */

static size_t get_client_count(Channel &channel)
{
    std::map<int,Client>::iterator it = channel._joinedUsers.begin();
    size_t count = 0;
    while (it != channel._joinedUsers.end())
    {
        it++;
        count++;
    }
    return (count);
}

/* -------------------------------------------------------------------------- */

void    Server::execute_invite_command(size_t pfdsindex, \
    std::vector<std::string> &full_cmd)
{
    t_pic pic;

    try
    {
        if (full_cmd.size() == 1)
        {
            ERR_NEEDMOREPARAMS(pfdsindex, full_cmd[0]);
            throw std::runtime_error("ERR_NEEDMOREPARAMS");
        }  
        pic = parse_invite_command(full_cmd[1], pfdsindex);
        is_invite_operator(*pic.channel, pfdsindex);
        if (get_client_count(*pic.channel) + 1 >= pic.channel->_client_limit)
        {
            ERR_CHANNELISFULL(pfdsindex, pic.channel->_channel_name);
            throw std::runtime_error(" ");
        }
        std::map<int,Client>::iterator it = pic.channel->_joinedUsers.find(pic.clients_fd);
        if (it != pic.channel->_joinedUsers.end())
        {
            ERR_USERONCHANNEL(pfdsindex, it->second.getNICKNAME(), pic.channel->_channel_name)
            throw std::runtime_error(" ");
        }
        it = ClientsMap.find(pic.clients_fd);
        pic.channel->_invitedClients.insert(std::make_pair(it->first, it->second));
        writeMessageToClient_fd(it->second.getSockfd(), ":" + it->second.getNICKNAME() \
            + "!" + it->second.getUSERNAME() + "@" + Hostname + " INVITE " + \
            it->second.getNICKNAME() + " " + pic.channel->_channel_name + "\r\n");
    }
    
    catch(const std::exception& e)
    {
        // std::cerr << e.what() << std::endl;
    }
}
