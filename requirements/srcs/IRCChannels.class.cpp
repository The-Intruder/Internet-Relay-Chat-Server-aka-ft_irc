/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channels.class.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haitkadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 19:01:05 by haitkadi          #+#    #+#             */
/*   Updated: 2023/05/07 19:01:10 by haitkadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ircserv.head.hpp"

/* -------------------------------------------------------------------------- */

IRCChannel::IRCChannel(std::string channelName, std::string channelPass){
    this->_channel_name = channelName;
    this->_channel_pass = channelPass;
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

uint64_t  IRCChannel::getClientLimit() const {
    return (this->_client_limit);
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void  IRCChannel::setClientLimit(uint64_t _client_limit) {
    this->_client_limit = _client_limit;
}

/* -------------------------------------------------------------------------- */