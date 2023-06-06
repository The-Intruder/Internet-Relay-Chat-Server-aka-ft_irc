/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handdle_NOTICE.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haitkadi <hssain.aitkadir@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 15:42:01 by haitkadi          #+#    #+#             */
/*   Updated: 2023/06/05 15:42:02 by haitkadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ircserv.head.hpp"

/* -------------------------------------------------------------------------- */

void    Server::NOTICE_messagToClient(std::size_t pfdsindex, std::string &clientNick, std::string &msg){
    Client &sender =  this->ClientsMap.find(this->pfds[pfdsindex].fd)->second;
    std::string fullMsg = ":" + sender.getNICKNAME() \
    + "!" + sender.getUSERNAME() + "@localhost.ip NOTICE " \
    + clientNick + " :" + msg + "\n";

    std::map<int, Client>::iterator it = this->ClientsMap.begin();
    for(; it != this->ClientsMap.end();it++){
        if (it->second.getNICKNAME() == clientNick)
            break;
    }
    if (it != this->ClientsMap.end()){
        writeMessageToClient(it->first, fullMsg);
    }
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void    Server::NOTICE_handdleMSG(std::size_t pfdsindex, std::vector<std::vector<std::string> > args){
    std::vector<std::string>    receivers = args[0];
    std::string    msg = args[1][0];
    for (std::vector<std::string>::iterator it = receivers.begin(); it != receivers.end();it++){
        if (it->at(0) == '#'){
            std::map<std::string, IRCChannel>::iterator chanl = this->ChannelsMap.find(*it);
            if (chanl != this->ChannelsMap.end())
                chanl->second.NOTICE_messagToChannel(this->pfds[pfdsindex].fd, msg);
        } else {
            this->NOTICE_messagToClient(pfdsindex, *it, msg);
        }
    }
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void    Server::NOTICE_Handle(size_t pfdsindex, std::vector<std::string> args){
    std::string nickName = this->ClientsMap.find(pfds[pfdsindex].fd)->second.getNICKNAME();
    if (args.size() > 1) {
        stringTrim(args[1], " \r\t\n");
        if (!args[1].empty()){
            try{
                std::vector<std::vector<std::string> > cleanArgs = parseArgs(args[1]);
                this->NOTICE_handdleMSG(pfdsindex, cleanArgs);
            } catch(const std::exception& e){
                (void)e;
            }
        }
    }
}

/* -------------------------------------------------------------------------- */
