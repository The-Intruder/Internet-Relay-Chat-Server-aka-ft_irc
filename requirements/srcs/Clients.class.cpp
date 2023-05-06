/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clients.class.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abellakr <abellakr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 18:31:30 by abellakr          #+#    #+#             */
/*   Updated: 2023/05/06 19:52:20 by abellakr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ircserv.head.hpp"

Client::Client(int newsockfd, unsigned int IP)
{
    this->sockfd = newsockfd;
    this->IP = IP;
    this->VP = false;
    this->VN = false;
    this->VM = false;
    this->Authenticated = false;
}

Client::~Client()
{
    
}

bool Client::getVP()
{
    return this->VP;
}
bool Client::getVN()
{
    return this->VN;
}

bool Client::getVM()
{
    return  this->VM;
}

bool Client::getAuthenticated()
{
    return this->Authenticated;
}
     
int Client::getSockfd()
{
    return this->sockfd;
}

void Client::setVP(bool v)
{
    this->VP = v;
}
void Client::setVN(bool v)
{
    this->VN = v;
}

void Client::setVM(bool v)
{
    this->VM = v;
}

void Client::setAuthenticated(bool v)
{
    this->Authenticated = v;
}
     
void Client::setSockfd(int v)
{
    this->sockfd = v;
}