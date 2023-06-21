/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Misc.functions.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haitkadi <hssain.aitkadir@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 15:42:31 by haitkadi          #+#    #+#             */
/*   Updated: 2023/06/05 15:42:32 by haitkadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ircserv.head.hpp"

/* -------------------------------------------------------------------------- */

void    stringTrim(std::string &str, const char *to_trim){
    str.erase(0, str.find_first_not_of(to_trim));
    str.erase(str.find_last_not_of(to_trim)+1);
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */
void    writeMessageToClient_fd(int fd, std::string message)
{
    int valwrite = write(fd, message.c_str() ,message.length());
    if(valwrite < 0)
        throw std::runtime_error("write failed");
}


/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void Server::removeChannel(std::string chName){
    std::map<std::string, Channel>::iterator channelIt = \
    this->ChannelsMap.find(chName);
    if(channelIt != this->ChannelsMap.end() && channelIt->second._joinedUsers.empty()){
        this->ChannelsMap.erase(channelIt);
    }
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */
void    Server::removeClientFromChans(int fd){
    std::map<std::string, Channel>::iterator chan = this->ChannelsMap.begin();
    for(; chan != this->ChannelsMap.end(); chan++){
        if (chan->second.isClientOnChan(fd)){
            chan->second.removeUser(fd);
            if (chan->second.empty()){
                removeChannel(chan->second.getChannelName());
            }
        }
    }
}

/* -------------------------------------------------------------------------- */