/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handdle_KICK.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haitkadi <hssain.aitkadir@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 15:41:53 by haitkadi          #+#    #+#             */
/*   Updated: 2023/06/05 15:41:54 by haitkadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ircserv.head.hpp"


/* -------------------------------------------------------------------------- */

std::vector<std::string>    KICK_parseArgs(std::string args){
    std::vector<std::string>  result;
    std::istringstream iss(args);
    std::vector<std::string> tokens;
    std::string token;

    while (iss >> token) {
        tokens.push_back(token);
    }
    if (tokens.size() < 2)
        throw std::runtime_error("ERR_NEEDMOREPARAMS");
    std::string channel = tokens[0];
    std::string user = tokens[1];
    std::string comment;
    if (tokens.size() > 2) {
        std::ostringstream oss;
        for (size_t i = 2; i < tokens.size(); ++i) {
            oss << tokens[i];
            if (i < tokens.size() - 1) {
                oss << " ";
            }
        }
        comment = oss.str();
    }
    result.push_back(channel);
    result.push_back(user);
    stringTrim(comment, ": ");
    if (!comment.empty())
        result.push_back(comment);
    return result;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void    Server::KICK_trigger(std::size_t pfdsindex, std::vector<std::string> args){
    std::string    chName;
    std::string    client;
    std::string    comment = "";
    if (args.size() == 3)
        comment = args[2];
    std::stringstream channels(args[0]);
    while(std::getline(channels, chName, ',')){
        std::map<std::string, IRCChannel>::iterator channel = this->ChannelsMap.find(chName);
        if(channel != this->ChannelsMap.end()){
            std::stringstream clients(args[1]);
            while(std::getline(clients, client, ',')){
                channel->second.kickFromChan(this->pfds[pfdsindex].fd, client, comment);
            }
        }else
            ERR_NOSUCHCHANNEL(pfdsindex, chName);
    }
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void    Server::KICK_Handle(size_t pfdsindex, std::vector<std::string> args){
    if (args.size() > 1) {
        stringTrim(args[1], " \r\t\n");
        if (!args[1].empty()){
            try{
                std::vector<std::string> kickArgs = KICK_parseArgs(args[1]);
                this->KICK_trigger(pfdsindex, kickArgs);
            }catch(const std::exception& e) {
                std::string err = e.what();
                if (!err.compare("ERR_NEEDMOREPARAMS")){
                    ERR_NEEDMOREPARAMS(pfdsindex, args[0]);
                }
            }
        } else
            ERR_NEEDMOREPARAMS(pfdsindex, args[0]);
    }else
        ERR_NEEDMOREPARAMS(pfdsindex, args[0]);
}

/* -------------------------------------------------------------------------- */