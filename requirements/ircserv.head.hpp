/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.head.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abellakr <abellakr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 10:55:17 by abellakr          #+#    #+#             */
/*   Updated: 2023/05/08 21:44:12 by abellakr         ###   ########.fr       */
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
 #include <arpa/inet.h>
 #include <utility>
 #include <string>
 #include <sstream>
 #include "srcs/errors.macros.hpp"



class Client
{
    private:
        __uint32_t IP; // IP adress of the client
        std::string NICKNAME;
        std::string USERNAME;
        std::string REALNAME;
        bool VP; // is the password validated
        bool VN; // is nickname validated
        bool VU; // is USER validated
        bool Authenticated; // is the user authenticated
        int sockfd; 
        bool firstATH;
    public:
        Client(int newsockfd, unsigned int IP);
        ~Client();
        __uint32_t getIP();
        bool getVP() const;
        bool getVN() const;
        bool getVU() const;
        bool getAuthenticated() const;
        int getSockfd() const;
        std::string getNICKNAME() const;
        std::string getUSERNAME() const;
        std::string getREALNAME() const;
        bool getfirstATH() const;

        void setVP(bool v);
        void setVN(bool v);
        void setVU(bool v);
        void setAuthenticated(bool v);
        void setSockfd(int v);
        void setNICKNAME(std::string NICK);
        void setUSERNAME(std::string USERNAME);
        void setREALNAME(std::string REALNAME);
        void setfirstATH(bool v);
};

class Server
{
    private:
        int PORT; // argument port
        std::string PASSWORD; // password of the server
        int servsockfd; // socket file descriptor of the server
        struct sockaddr_in ServAddr; // socket address of the server
        std::vector<pollfd> pfds; // file descriptors to keep eyes on 
        std::map<int,Client>  ClientsMap; // clients map
        std::vector<std::string> MS; // mesaage splited by space
        std::vector<std::string> NickNames;
 
    public:
        Server(int PORT, std::string PASSTWORD);
        ~Server();
        void SetupServer(void);
        void AcceptConnections();
        void HandleConnections(size_t i); // Handle connections
        void SaveClients(int newsockfd, unsigned int IP); // save the connected client to the map of clients

        bool Authentication(size_t pfdsindex);
        void checkpass(size_t pfdsindex, Client& client); 
        void checknick(size_t pfdsindex, Client& client); 
        void checkuser(size_t pfdsindex, Client& client); 
        
        void writemessagetoclients(size_t pfdsindex, std::string message); // pfdsindex is the fd socket of the client to send data to *
        void splitargs();
};
#endif