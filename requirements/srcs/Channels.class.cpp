/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channels.class.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haitkadi <hssain.aitkadir@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 15:42:25 by haitkadi          #+#    #+#             */
/*   Updated: 2023/06/05 15:42:26 by haitkadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../ircserv.head.hpp"

/* -------------------------------------------------------------------------- */

Channel::Channel(std::string channelName, std::string channelPass){
    this->_channel_name = channelName;
    this->_channel_pass = channelPass;
    this->_client_limit = 50;
    this->_modes = 0;
    this->_topic = "";
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

Channel::Channel(Channel const &src){
    _joinedUsers = src._joinedUsers;
    _admins = src._admins;
    _channel_name = src._channel_name;
    _channel_pass = src._channel_pass;
    _topic = src._topic;
    _client_limit = src._client_limit;
    _modes = src._modes;
} 

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

const Channel &Channel::operator=(Channel const &src){
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

Channel::~Channel() {}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

bool   Channel::isPrivate() const {
    return (this->_modes & 0x01);
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

bool   Channel::isSecret() const {
    return (this->_modes & 0x02);
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

bool   Channel::isInviteOnly() const {
    return (this->_modes & 0x04);
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

bool  Channel::isOnlyVoiceAndOps() const {
    return (this->_modes & 0x08);
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

bool  Channel::isNoOutsideMessages() const {
    return (this->_modes & 0x10);
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

bool  Channel::isOnlyOpsChangeTopic() const {
    return (this->_modes & 0x20);
}

/* -------------------------------------------------------------------------- */

void Channel::setChannelName(const std::string &channelName){
    this->_channel_name = channelName;;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

std::string Channel::getChannelName() const{
    return this->_channel_name;
}


uint64_t  Channel::getClientLimit() const {
    return (this->_client_limit);
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void  Channel::setClientLimit(uint64_t _client_limit) {
    this->_client_limit = _client_limit;
}


std::string Channel::getChannelPass() const{
    return this->_channel_pass;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void        Channel::setClientPass(std::string channel_pass){
    this->_channel_pass = channel_pass;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

std::string Channel::getChannelTopic() const{
    return this->_topic;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

bool    Channel::empty() const{
    return this->_joinedUsers.size() < 1;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void Channel::notifyUsers(int fd){

    std::string notifyusers = ":" + this->_joinedUsers.find(fd)->second.getNICKNAME() \
    + "!" + this->_joinedUsers.find(fd)->second.getUSERNAME() + "@localhost.ip JOIN " \
    + this->getChannelName() + "\n";
    for(std::map<int, Client>::iterator i = this->_joinedUsers.begin(); i != this->_joinedUsers.end();i++){
        writeMessageToClient_fd(i->first, notifyusers);
    }
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void Channel::welcomeUser(int fd){

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

void    Channel::joinChannel(Client &client, std::string &chPass, int fd){
    if (this->getChannelPass() != chPass){
        ERR_BADCHANNELKEY(fd, this->getChannelName());
    } else if (this->isInviteOnly() || this->isPrivate()){
        ERR_INVITEONLYCHAN(fd, this->getChannelName());
    }else if (this->_joinedUsers.size() >= this->getClientLimit()){
        ERR_CHANNELISFULL(fd, this->getChannelName());
    } else if (this->_joinedUsers.find(fd) == this->_joinedUsers.end()){
        this->_joinedUsers.insert(std::make_pair(fd, client));

    }
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */
void    Channel::addAdmin(int fd){
    this->_admins.insert(std::make_pair(fd, this->_joinedUsers.find(fd)->second));
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */
void    Channel::PRIVMSG_messagToChannel(int fd, std::string &msg){
    std::map<int, Client>::iterator client = this->_joinedUsers.find(fd);
    if (client != this->_joinedUsers.end() && this->_bannedUsers.find(fd) == this->_bannedUsers.end()){
        if (!this->isOnlyVoiceAndOps() || (this->isOnlyVoiceAndOps() && this->_admins.find(fd) != this->_admins.end())){
            std::string fullMsg = ":" + client->second.getNICKNAME() \
            + "!" + client->second.getUSERNAME() + "@localhost.ip PRIVMSG " \
            + this->getChannelName() + " :" + msg + "\n";

            for(std::map<int, Client>::iterator i = this->_joinedUsers.begin(); i != this->_joinedUsers.end();i++){
                if (i->first != fd)
                    writeMessageToClient_fd(i->first, fullMsg);
            }
        } else
            ERR_CANNOTSENDTOCHAN(fd, this->getChannelName());
    } else
        ERR_CANNOTSENDTOCHAN(fd, this->getChannelName());
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */
void    Channel::NOTICE_messagToChannel(int fd, std::string &msg){
    std::map<int, Client>::iterator client = this->_joinedUsers.find(fd);
    if (client != this->_joinedUsers.end() && this->_bannedUsers.find(fd) == this->_bannedUsers.end()){
        if (!this->isOnlyVoiceAndOps() || (this->isOnlyVoiceAndOps() && this->_admins.find(fd) != this->_admins.end())){
            std::string fullMsg = ":" + client->second.getNICKNAME() \
            + "!" + client->second.getUSERNAME() + "@localhost.ip PRIVMSG " \
            + this->getChannelName() + " :" + msg + "\n";

            for(std::map<int, Client>::iterator i = this->_joinedUsers.begin(); i != this->_joinedUsers.end();i++){
                if (i->first != fd)
                    writeMessageToClient_fd(i->first, fullMsg);
            }
        }
    }
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void Channel::sayGoodby(int fd, std::string &msg){
    std::string sayBy = ":" + this->_joinedUsers.find(fd)->second.getNICKNAME() \
    + "!" + this->_joinedUsers.find(fd)->second.getUSERNAME() + "@localhost.ip PART " \
    + this->getChannelName() + " " + msg + "\n";
    for(std::map<int, Client>::iterator i = this->_joinedUsers.begin(); i != this->_joinedUsers.end();i++){
        writeMessageToClient_fd(i->first, sayBy);
    }
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void    Channel::leftChannel(int fd, std::string &msg){
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

void    Channel::kickFromChan(int kickerFd, std::string &userToKick, std::string &comment){
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
            writeMessageToClient_fd(i->first, cmnt);
        }
        this->_joinedUsers.erase(userToKickFd);
    } else
        ERR_CHANOPRIVSNEEDED(kickerFd, this->getChannelName());
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

bool    Channel::isClientOnChan(int fd){
    return this->_joinedUsers.find(fd) != this->_joinedUsers.end();
}


/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */
void    Channel::sendTopicToClient(int fd){
    if(!this->getChannelTopic().empty()){
        RPL_TOPIC(fd, this->getChannelName(), this->getChannelTopic());
    } else
        RPL_NOTOPIC(fd, this->getChannelName());
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void    Channel::changeTopic(int fd, std::string &topic){
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

void    Channel::removeUser(int fd){
    std::map<int, Client>::iterator clientIt = this->_joinedUsers.find(fd);
    if (clientIt != this->_joinedUsers.end()){
        this->_joinedUsers.erase(clientIt);
        std::map<int, Client>::iterator adminIt = this->_admins.find(fd);
        if (adminIt != this->_admins.end())
            this->_admins.erase(adminIt);
    }
}

/* -------------------------------------------------------------------------- */

