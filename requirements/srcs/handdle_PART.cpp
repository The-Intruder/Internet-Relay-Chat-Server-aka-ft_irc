/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handdle_PART.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haitkadi <hssain.aitkadir@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 15:42:05 by haitkadi          #+#    #+#             */
/*   Updated: 2023/06/05 15:42:06 by haitkadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ircserv.head.hpp"

/* -------------------------------------------------------------------------- */
std::vector<std::vector<std::string> >    PART_splitChannels(std::vector<std::string> buffer){
    std::vector<std::vector<std::string> >  result;
    std::vector<std::string>                channels;
    std::vector<std::string>                msg;

    if(buffer.empty())
        throw std::runtime_error("ERR_NEEDMOREPARAMS");
    if (buffer[0].find(",")  != std::string::npos){
        std::string token;
        std::stringstream ss(buffer[0]);
        while(std::getline(ss, token, ',')){
            stringTrim(token, " \r\t\n");
            if (!token.empty())
                channels.push_back(token);
            token.clear();
        }
        if(channels.empty()){
            throw std::runtime_error("ERR_NEEDMOREPARAMS");
        }
        result.push_back(channels);
    } else {
        channels.push_back(buffer[0]);
        result.push_back(channels);
    }
    if (buffer.size() > 1){
        msg.push_back(buffer[1]);
        result.push_back(msg);
    }

    return result;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */
std::vector<std::vector<std::string> >    PART_parseArgs(std::string args){
    std::string token;
    std::vector<std::string> buffer;
    std::string::size_type spacePos = args.find(' ');
    if (spacePos != std::string::npos) {
        std::string token1 = args.substr(0, spacePos);
        std::string token2 = args.substr(spacePos + 1);
        stringTrim(token1, " \r\t\n");
        stringTrim(token2, " \r\t\n");
        if (!token1.empty())
            buffer.push_back(token1);
        if (!token2.empty())
            buffer.push_back(token2);
    } else{
        buffer.push_back(args);
    }
    return PART_splitChannels(buffer);
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void    Server::PART_trigger(std::size_t pfdsindex, std::vector<std::vector<std::string> > args){
    std::vector<std::string>    chNames = args[0];
    std::string    msg = "";
    if (args.size() == 2)
        msg = args[1][0];
    for (std::vector<std::string>::iterator it = chNames.begin(); it != chNames.end();it++){
        std::map<std::string, Channel>::iterator channel = this->ChannelsMap.find(*it);
        if(channel != this->ChannelsMap.end()){
            channel->second.leftChannel(this->pfds[pfdsindex].fd, msg);
            this->removeChannel(channel->second.getChannelName());
        }else{
            ERR_NOSUCHCHANNEL(pfdsindex, *it);
        }
    }
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void    Server::PART_Handle(size_t pfdsindex, std::vector<std::string> args){
    if (args.size() > 1) {
        stringTrim(args[1], " :\r\t\n");
        if (!args[1].empty()){
            try{
                std::vector<std::vector<std::string> > cleanArgs = PART_parseArgs(args[1]);
                this->PART_trigger(pfdsindex, cleanArgs);
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