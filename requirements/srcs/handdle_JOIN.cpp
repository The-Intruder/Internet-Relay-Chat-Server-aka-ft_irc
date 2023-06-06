/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handdle_JOIN.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haitkadi <hssain.aitkadir@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 15:41:17 by haitkadi          #+#    #+#             */
/*   Updated: 2023/06/05 15:41:23 by haitkadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ircserv.head.hpp"

/* -------------------------------------------------------------------------- */

std::vector<std::string>    parseLine(std::string line){
    std::stringstream           ss(line);
    std::vector<std::string>    tokens;
    std::string                 token;

    while(std::getline(ss, token, ',')){
        tokens.push_back(token);
        token.clear();
    }
    return tokens;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

bool containsCTLG(const std::string& str) {
    for (std::string::size_type i = 0; i < str.length(); ++i) {
        if (static_cast<int>(str[i]) == 7)
            return true;
    }
    return false;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

bool checkChannelName(std::string &chName){
    if (containsCTLG(chName) || (chName.find(": ,") != std::string::npos) \
        || (chName.length() < 2 || chName.length() > 200) \
        || (chName.at(0) != '#')){
        return true;
    }
    return false;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

std::pair<std::vector<std::string>, std::vector<std::string> >   \
parse_JOIN(std::string args){
    std::stringstream           ss(args);
    std::vector<std::string>    channels;
    std::vector<std::string>    keys;
    std::string                 line;
    bool                        isChnl;

    isChnl = true;
    while(std::getline(ss, line, ' ')){
        if(isChnl){
            channels = parseLine(line);
            isChnl = !isChnl;
        }else
            keys = parseLine(line);
    }
    if (channels.size() > 0 && keys.size() > 0)
        return std::make_pair(channels, keys);
    else
        return std::make_pair(channels, std::vector<std::string>());
}

/* -------------------------------------------------------------------------- */

void    Server::addChannel(int fd, std::string chName, std::string chPass){
    IRCChannel channel(chName, chPass);
    channel.joinChannel(this->ClientsMap.find(fd)->second, chPass, fd);
    channel.addAdmin(fd);
    this->ChannelsMap.insert(std::make_pair(chName, channel));
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void Server::HandleJOIN(size_t pfdsindex, std::vector<std::string> args){
    if (args.size() < 2) {
        std::string cmd = "JOIN";
        ERR_NEEDMOREPARAMS(pfdsindex, cmd);
    } else if(args[1].size() < 2) {
        ERR_INVALIDCHNLNAME(pfdsindex, args[1]);
    } else {
        std::pair<std::vector<std::string>, std::vector<std::string> > parsedArgs = \
        parse_JOIN(args[1]);
        for (std::size_t i=0;i<parsedArgs.first.size();i++){
            std::string chPass = i < parsedArgs.second.size() ? parsedArgs.second[i] : "";
            std::map<std::string, IRCChannel>::iterator channel = this->ChannelsMap.find(parsedArgs.first[i]);
            if (checkChannelName(parsedArgs.first[i])){
                ERR_INVALIDCHNLNAME(pfdsindex, parsedArgs.first[i]);
            } else if (channel == this->ChannelsMap.end()){
                this->addChannel(this->pfds[pfdsindex].fd, parsedArgs.first[i], chPass);
            } else if (channel != this->ChannelsMap.end()) {
                channel->second.joinChannel(this->ClientsMap.find(this->pfds[pfdsindex].fd)->second, chPass, this->pfds[pfdsindex].fd);
            }
        }
    }
}

/* -------------------------------------------------------------------------- */



