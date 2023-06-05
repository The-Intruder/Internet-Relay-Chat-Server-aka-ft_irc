/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.head.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abellakr <abellakr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 10:55:17 by abellakr          #+#    #+#             */
/*   Updated: 2023/06/05 18:59:37 by abellakr         ###   ########.fr       */
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
 #include "srcs/errors.responces.macros.hpp"
 #include <cstring>
 #include <cstdlib>
#include <time.h>
#include <cstdlib>
#include <sys/time.h>

// error replies macros
// #define ERR_NEEDMOREPARAMS 461
// #define ERR_ALREADYREGISTRED 462

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
        bool Authenticated; // hadi matkhdmoch biha 
        int sockfd; 
        bool firstATH; // ila biti t3rf wach ithentificated khdm bhada
        std::string buffer; // each client buffer
        long connectedtime;// time to connect to the server
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
        std::string getbuffer() const;
        long gettime() const;

        void setVP(bool v);
        void setVN(bool v);
        void setVU(bool v);
        void setAuthenticated(bool v);
        void setSockfd(int v);
        void setNICKNAME(std::string NICK);
        void setUSERNAME(std::string USERNAME);
        void setREALNAME(std::string REALNAME);
        void setfirstATH(bool v);
        void setbuffer(std::string arg);
        void settime(long time);
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
        std::vector<std::string> MSATH; // mesaage splited by space for ATH phase
        std::string Servtimeinfo; // server created time
        /*---------------------- Hssain-Part ------------------ */
        std::map<std::string,IRCChannel>  ChannelsMap; // Channels map
        std::string buffertmp; // this is for ignoring control D behavior 
 
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
        void getDateTime();
        long ft_gettime(void);
        void executecommand(size_t pfdsindex);
        // commands
        void    bot(size_t pfdsindex);
        void    nick(size_t pfdsindex);
        void    quit(size_t pfdsindex);

        /*---------------------- Hssain-Part ------------------ */
        void AddChannel(std::string args);
        void RemoveChannel();
}; 
#endif