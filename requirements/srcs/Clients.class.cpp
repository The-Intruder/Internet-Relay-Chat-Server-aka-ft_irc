/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clients.class.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abellakr <abellakr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 18:31:30 by abellakr          #+#    #+#             */
/*   Updated: 2023/05/05 18:54:32 by abellakr         ###   ########.fr       */
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
    this->authenticated = false;
    this->abdellah = "mk";
}

Client::~Client()
{
    
}