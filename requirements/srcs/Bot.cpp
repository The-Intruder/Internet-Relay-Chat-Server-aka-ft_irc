/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abellakr <abellakr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 12:00:02 by abellakr          #+#    #+#             */
/*   Updated: 2023/06/07 00:10:03 by abellakr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ircserv.head.hpp"

void Server::bot(size_t pfdsindex)
{
    if(MS.size() != 2)
    {
        ERR_FLAGNOTFOUND(pfdsindex);
        return;
    }
    if(MS[1] == "-help")
    {
        writeMessageToClient(pfdsindex, ":IrcTheThreeMusketeers NOTICE limechat :use -logtime to get your logtime at the server\n");
        writeMessageToClient(pfdsindex, ":IrcTheThreeMusketeers NOTICE limechat :use -commands to get commands to use in this server\n");
    }
    else if(MS[1] == "-commands")
    {
        writeMessageToClient(pfdsindex, ":IrcTheThreeMusketeers NOTICE limechat :MODE: <channel/user> <mode> - sets channel or user modes.\n");
        writeMessageToClient(pfdsindex, ":IrcTheThreeMusketeers NOTICE limechat :QUIT: <message> - disconnects from the server with an optional message.\n");
        writeMessageToClient(pfdsindex, ":IrcTheThreeMusketeers NOTICE limechat :JOIN: <channel> [<key>] - joins a channel with an optional password.\n");
        writeMessageToClient(pfdsindex, ":IrcTheThreeMusketeers NOTICE limechat :PART: <channel> [<message>] - leaves a channel with an optional part message.\n");
        writeMessageToClient(pfdsindex, ":IrcTheThreeMusketeers NOTICE limechat :TOPIC: <channel> [<topic>] - sets or queries the topic of a channel.\n");
        writeMessageToClient(pfdsindex, ":IrcTheThreeMusketeers NOTICE limechat :NAMES: [<channel>] - lists the users in a channel.\n");
        writeMessageToClient(pfdsindex, ":IrcTheThreeMusketeers NOTICE limechat :INVITE: <user> <channel> - invites a user to a channel.\n");
        writeMessageToClient(pfdsindex, ":IrcTheThreeMusketeers NOTICE limechat :KICK: <channel> <user> [<comment>] - kicks a user from a channel with an optional comment.\n");
        writeMessageToClient(pfdsindex, ":IrcTheThreeMusketeers NOTICE limechat :PRIVMSG: <receiver> <text> - sends a private message to a user or a message to a channel.\n");
        writeMessageToClient(pfdsindex, ":IrcTheThreeMusketeers NOTICE limechat :NOTICE: <receiver> <text> - sends a notice to a user or a channel.\n");
    }
    else if(MS[1] == "-logtime")
    {
        std::map<int,Client>::iterator it = ClientsMap.find(pfds[pfdsindex].fd);
        Client& tmp = it->second;
        long Time = ft_gettime() - tmp.gettime();
        long hours = Time / 3600;
        long minutes = (Time % 3600) / 60;
        long seconds = (Time % 3600) % 60;
        std::stringstream ss;
        ss << hours << "H :" << minutes << "M :" << seconds << "S";
        writeMessageToClient(pfdsindex, ":IrcTheThreeMusketeers NOTICE limechat :Your logtime on the server is " + ss.str() +  "\n");
    }
    else
    {
        ERR_FLAGNOTFOUND(pfdsindex);
    }
}