/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.head.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abellakr <abellakr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 10:55:17 by abellakr          #+#    #+#             */
/*   Updated: 2023/05/04 16:27:35 by abellakr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  IRCSERV_HEAD_HPP
#define  IRCSERV_HEAD_HPP


#include <iostream>
#include <string>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/poll.h>
 #include <limits.h>
 #include <vector>
 #include <map>

class Client
{
    private:
        // int PORT; // port of the client 
        // std::string  IPADDRESS;// ip address of the client

    public:
};

class Server
{
    private:
        int PORT;
        std::string PASSWORD;
        struct sockaddr_in ServAddr;
        std::map<int,Client>  Clients;

    public:
        Server(int PORT, std::string PASSTWORD);
        ~Server();
};


#endif