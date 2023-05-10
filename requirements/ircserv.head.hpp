/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.head.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Abellakr, Hssain, Mohamed Amine            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 10:55:17                      #+#    #+#             */
/*   Updated: 2023/05/07 14:52:22                     ###   ########.fr       */
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


// error replies macros
#define ERR_NEEDMOREPARAMS 461
#define ERR_ALREADYREGISTRED 462



class Client
{
    private:
        __uint32_t IP; // IP adress of the client
        std::string NICKNAME;
        bool VP; // is the password validated
        bool VN; // is nickname validated
        bool VM; // is message validated
        bool Authenticated; // is the user authenticated
        int sockfd;
    public:
        Client(int newsockfd, unsigned int IP);
        ~Client();
        __uint32_t getIP();
        std::string getNICKNAME();
        bool getVP();
        bool getVN();
        bool getVM();
        bool getAuthenticated();
        int getSockfd();

        void setVP(bool v);
        void setVN(bool v);
        void setVM(bool v);
        void setAuthenticated(bool v);
        void setSockfd(int v);
};

class IRCChannel {
    private:
        std::map<int,Client>            _joinedUsers; // Users that joined specifec channel
        std::map<int,Client>            _admins; // Operators/Admins of a specifec channel
        std::string                     _channel_name;
        std::string                     _channel_pass;
        std::string                     _topic;
        std::string                     _key;
    public:
        uint64_t                        _client_limit;
        uint16_t                        _modes;

    public:
        IRCChannel(std::string channelName, std::string channelPass);
        IRCChannel(IRCChannel const &src);
        const IRCChannel &operator=(IRCChannel const &src);
        ~IRCChannel();

        bool        isPrivate() const;
        bool        isSecret() const;
        bool        isInviteOnly() const;
        bool        isOnlyVoiceAndOps() const;
        bool        isNoOutsideMessages() const;
        bool        isOnlyOpsChangeTopic() const;

        void setChannelName(const std::string &channelName);
        std::string getChannelName() const;

        uint64_t    getClientLimit() const;
        void        setClientLimit(uint64_t _client_limit);
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
 
    public:
        Server(int PORT, std::string PASSTWORD);
        ~Server();
        void SetupServer(void);
        void AcceptConnections();
        void HandleConnections(size_t i); // Handle connections
        void SaveClients(int newsockfd, unsigned int IP); // save the connected client to the map of clients
        bool Authentication(size_t pfdsindex);
        void writemessagetoclients(size_t pfdsindex, std::string message, int messagelen); // pfdsindex is the fd socket of the client to send data to
        void ErrorReplies(int flag, size_t pfdsindex);
};
#endif

