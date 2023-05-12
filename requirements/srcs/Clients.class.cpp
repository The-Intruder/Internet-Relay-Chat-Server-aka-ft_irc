/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clients.class.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abellakr <abellakr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 18:31:30 by abellakr          #+#    #+#             */
/*   Updated: 2023/05/12 07:34:06 by abellakr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ircserv.head.hpp"

Client::Client(int newsockfd, unsigned int IP)
{
    this->sockfd = newsockfd;
    this->IP = IP;
    this->VP = false;
    this->VN = false;
    this->VU = false;
    this->Authenticated = false;
    this->firstATH = false;
}

Client::~Client()
{
    
}


// geters
bool Client::getVP() const
{
    return this->VP;
}
bool Client::getVN() const
{
    return this->VN;
}

bool Client::getVU() const
{
    return  this->VU;
}

bool Client::getAuthenticated() const
{
    return this->Authenticated;
}
     
int Client::getSockfd() const
{
    return this->sockfd;
}


std::string Client::getNICKNAME() const
{
    return this->NICKNAME;
}

std::string Client::getUSERNAME() const
{
    return this->USERNAME;
}

std::string Client::getREALNAME() const
{
    return this->REALNAME;
}

bool Client::getfirstATH() const
{
    return this->firstATH;
}

std::string Client::getbuffer() const
{
    return this->buffer;
}
// seters
void Client::setVP(bool v)
{
    this->VP = v;
}
void Client::setVN(bool v)
{
    this->VN = v;
}

void Client::setVU(bool v)
{
    this->VU = v;
}

void Client::setAuthenticated(bool v)
{
    this->Authenticated = v;
}
     
void Client::setSockfd(int v)
{
    this->sockfd = v;
}

void Client::setNICKNAME(std::string NICK)
{
    this->NICKNAME = NICK;
}

void Client::setUSERNAME(std::string USERNAME)
{
    this->USERNAME = USERNAME;
}

void Client::setREALNAME(std::string REALNAME)
{
    this->REALNAME = REALNAME;
}

void Client::setfirstATH(bool v)
{
    this->firstATH = v;
}

void Client::setbuffer(std::string arg)
{
    this->buffer = arg;
}
