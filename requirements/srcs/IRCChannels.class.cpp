/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCChannels.class.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haitkadi <hssain.aitkadir@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 15:42:25 by haitkadi          #+#    #+#             */
/*   Updated: 2023/06/05 15:42:26 by haitkadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../ircserv.head.hpp"

/* -------------------------------------------------------------------------- */

IRCChannel::IRCChannel(std::string channelName, std::string channelPass){
    this->_channel_name = channelName;
    this->_channel_pass = channelPass;
    this->_client_limit = 50;
    this->_modes = 0;
    this->_topic = "";
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

IRCChannel::IRCChannel(IRCChannel const &src){
    _joinedUsers = src._joinedUsers;
    _admins = src._admins;
    _channel_name = src._channel_name;
    _channel_pass = src._channel_pass;
    _topic = src._topic;
    _client_limit = src._client_limit;
    _modes = src._modes;
} 

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

const IRCChannel &IRCChannel::operator=(IRCChannel const &src){
    if (this != &src) {
        _joinedUsers = src._joinedUsers;
        _admins = src._admins;
        _channel_name = src._channel_name;
        _channel_pass = src._channel_pass;
        _topic = src._topic;
        _client_limit = src._client_limit;
        _modes = src._modes;
    }
    return *this;
}


/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

IRCChannel::~IRCChannel() {}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

bool   IRCChannel::isPrivate() const {
    return (this->_modes & 0x01);
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

bool   IRCChannel::isSecret() const {
    return (this->_modes & 0x02);
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

bool   IRCChannel::isInviteOnly() const {
    return (this->_modes & 0x04);
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

bool  IRCChannel::isOnlyVoiceAndOps() const {
    return (this->_modes & 0x08);
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

bool  IRCChannel::isNoOutsideMessages() const {
    return (this->_modes & 0x10);
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

bool  IRCChannel::isOnlyOpsChangeTopic() const {
    return (this->_modes & 0x20);
}

/* -------------------------------------------------------------------------- */

void IRCChannel::setChannelName(const std::string &channelName){
    this->_channel_name = channelName;;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

std::string IRCChannel::getChannelName() const{
    return this->_channel_name;
}


uint64_t  IRCChannel::getClientLimit() const {
    return (this->_client_limit);
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void  IRCChannel::setClientLimit(uint64_t _client_limit) {
    this->_client_limit = _client_limit;
}


std::string IRCChannel::getChannelPass() const{
    return this->_channel_pass;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void        IRCChannel::setClientPass(std::string channel_pass){
    this->_channel_pass = channel_pass;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

std::string IRCChannel::getChannelTopic() const{
    return this->_topic;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

bool    IRCChannel::empty() const{
    return this->_joinedUsers.size() < 1;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void IRCChannel::notifyUsers(int fd){

    std::string notifyusers = ":" + this->_joinedUsers.find(fd)->second.getNICKNAME() \
    + "!" + this->_joinedUsers.find(fd)->second.getUSERNAME() + "@localhost.ip JOIN " \
    + this->getChannelName() + "\n";
    for(std::map<int, Client>::iterator i = this->_joinedUsers.begin(); i != this->_joinedUsers.end();i++){
        writeMessageToClient(i->first, notifyusers);
    }
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void IRCChannel::welcomeUser(int fd){

        std::string nickName = this->_joinedUsers.find(fd)->second.getNICKNAME();
        std::string membersList;
        for(std::map<int, Client>::iterator i = this->_joinedUsers.begin(); i != this->_joinedUsers.end();i++){
            if (this->_admins.find(i->first) != this->_admins.end()){
                membersList += "@" + i->second.getNICKNAME() + " ";
            }else
                membersList += i->second.getNICKNAME() + " ";
        }
        RPL_NAMREPLY(fd, nickName,  this->getChannelName(), membersList);
        RPL_ENDOFNAMES(fd, nickName,  this->getChannelName());

}


/* -------------------------------------------------------------------------- */\

void    IRCChannel::joinChannel(Client &client, std::string &chPass, int fd){
    if (this->getChannelPass() != chPass){
        ERR_BADCHANNELKEY(fd, this->getChannelName());
    } else if (this->isInviteOnly()){
        ERR_INVITEONLYCHAN(fd, this->getChannelName());
    }else if (this->_joinedUsers.size() >= this->getClientLimit()){
        ERR_CHANNELISFULL(fd, this->getChannelName());
    } else if (this->_joinedUsers.find(fd) == this->_joinedUsers.end()){
        this->_joinedUsers.insert(std::make_pair(fd, client));
        this->notifyUsers(fd);
        this->welcomeUser(fd);
    }
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */
void    IRCChannel::addAdmin(int fd){
    this->_admins.insert(std::make_pair(fd, this->_joinedUsers.find(fd)->second));
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */
void    IRCChannel::PRIVMSG_messagToChannel(int fd, std::string &msg){
    std::map<int, Client>::iterator client = this->_joinedUsers.find(fd);
    if (client != this->_joinedUsers.end() && this->_bannedUsers.find(fd) == this->_bannedUsers.end()){
        if (!this->isOnlyVoiceAndOps() || (this->isOnlyVoiceAndOps() && this->_admins.find(fd) != this->_admins.end())){
            std::string fullMsg = ":" + client->second.getNICKNAME() \
            + "!" + client->second.getUSERNAME() + "@localhost.ip PRIVMSG " \
            + this->getChannelName() + " :" + msg + "\n";

            for(std::map<int, Client>::iterator i = this->_joinedUsers.begin(); i != this->_joinedUsers.end();i++){
                if (i->first != fd)
                    writeMessageToClient(i->first, fullMsg);
            }
        } else
            ERR_CANNOTSENDTOCHAN(fd, this->getChannelName());
    } else
        ERR_CANNOTSENDTOCHAN(fd, this->getChannelName());
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */
void    IRCChannel::NOTICE_messagToChannel(int fd, std::string &msg){
    std::map<int, Client>::iterator client = this->_joinedUsers.find(fd);
    if (client != this->_joinedUsers.end() && this->_bannedUsers.find(fd) == this->_bannedUsers.end()){
        if (!this->isOnlyVoiceAndOps() || (this->isOnlyVoiceAndOps() && this->_admins.find(fd) != this->_admins.end())){
            std::string fullMsg = ":" + client->second.getNICKNAME() \
            + "!" + client->second.getUSERNAME() + "@localhost.ip PRIVMSG " \
            + this->getChannelName() + " :" + msg + "\n";

            for(std::map<int, Client>::iterator i = this->_joinedUsers.begin(); i != this->_joinedUsers.end();i++){
                if (i->first != fd)
                    writeMessageToClient(i->first, fullMsg);
            }
        }
    }
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void IRCChannel::sayGoodby(int fd, std::string &msg){
    std::string sayBy = ":" + this->_joinedUsers.find(fd)->second.getNICKNAME() \
    + "!" + this->_joinedUsers.find(fd)->second.getUSERNAME() + "@localhost.ip PART " \
    + this->getChannelName() + " " + msg + "\n";
    for(std::map<int, Client>::iterator i = this->_joinedUsers.begin(); i != this->_joinedUsers.end();i++){
        writeMessageToClient(i->first, sayBy);
    }
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void    IRCChannel::leftChannel(int fd, std::string &msg){
    std::map<int, Client>::iterator clientIt = this->_joinedUsers.find(fd);
    if (clientIt != this->_joinedUsers.end()){
        sayGoodby(fd, msg);
        this->_joinedUsers.erase(clientIt);
        std::map<int, Client>::iterator adminIt = this->_admins.find(fd);
        if (adminIt != this->_admins.end())
            this->_admins.erase(adminIt);
    } else{
        ERR_NOTONCHANNEL(fd, this->getChannelName());
    }
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void    IRCChannel::kickFromChan(int kickerFd, std::string &userToKick, std::string &comment){
    int userToKickFd = 0;
    std::map<int, Client>::iterator clientIt = this->_joinedUsers.begin();
    while (clientIt != this->_joinedUsers.end()){
        if (!clientIt->second.getNICKNAME().compare(userToKick))
            userToKickFd = clientIt->first;
        clientIt++;
    }
    if (!userToKickFd){
         ERR_NOTONCHANNEL(kickerFd, userToKick);
        return;
    }
    if (kickerFd == userToKickFd)
        return;
    if (this->_admins.find(kickerFd) != this->_admins.end()){
        std::string cmnt = ":" + this->_joinedUsers.find(kickerFd)->second.getNICKNAME() \
        + "!" + this->_joinedUsers.find(kickerFd)->second.getUSERNAME() + "@localhost.ip KICK " \
        + this->getChannelName() + " " + userToKick + " " + comment + "\n";
        for(std::map<int, Client>::iterator i = this->_joinedUsers.begin(); i != this->_joinedUsers.end();i++){
            writeMessageToClient(i->first, cmnt);
        }
        this->_joinedUsers.erase(userToKickFd);
    } else
        ERR_CHANOPRIVSNEEDED(kickerFd, this->getChannelName());
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

bool    IRCChannel::isClientOnChan(int fd){
    return this->_joinedUsers.find(fd) != this->_joinedUsers.end();
}


/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */
void    IRCChannel::sendTopicToClient(int fd){
    if(!this->getChannelTopic().empty()){
        RPL_TOPIC(fd, this->getChannelName(), this->getChannelTopic());
    } else
        RPL_NOTOPIC(fd, this->getChannelName());
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void    IRCChannel::changeTopic(int fd, std::string &topic){
    if(this->isOnlyOpsChangeTopic()){
        if(this->_admins.find(fd) != this->_admins.end()){
            this->_topic = topic;
        }else
            ERR_CHANOPRIVSNEEDED(fd, this->getChannelName());
    }else {
        this->_topic = topic;
    }
}
/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void    IRCChannel::removeUser(int fd){
    std::map<int, Client>::iterator clientIt = this->_joinedUsers.find(fd);
    if (clientIt != this->_joinedUsers.end()){
        this->_joinedUsers.erase(clientIt);
        std::map<int, Client>::iterator adminIt = this->_admins.find(fd);
        if (adminIt != this->_admins.end())
            this->_admins.erase(adminIt);
    }
}

/* -------------------------------------------------------------------------- */

