/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.head.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Abellakr, Hssain, Mohamed Amine            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 10:55:17 by abellakr          #+#    #+#             */
/*   Updated: 2023/05/07 14:52:22 by abellakr         ###   ########.fr       */
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
        Client(const Client& other);
        Client& operator=(const Client& other);
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
        std::map<int,Client>            _bannedUsers; // Banned Users
        std::string                     _channel_name;
        std::string                     _channel_pass;
        std::string                     _topic;
    public:
        uint64_t                        _client_limit;
        uint32_t                        _modes;

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
        std::string getChannelPass() const;
        void        setClientPass(std::string channel_pass);
        std::string getChannelTopic() const;
        void        addAdmin(int fd);
        void        joinChannel(Client &client, std::string &chPass, int fd);
        void        notifyUsers(int fd);
        void        welcomeUser(int fd);
        void        PRIVMSG_messagToChannel(int fd, std::string &msg);
        void        NOTICE_messagToChannel(int fd, std::string &msg);
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
        std::vector<std::string> MS; // message splited by space
        std::vector<std::string> MSATH; // mesaage splited by space for ATH phase
        std::string Servtimeinfo; // server created time
        std::string buffertmp; // this is for ignoring control D behavior 
        /*---------------------- Hssain-Part ------------------ */
        std::map<std::string,IRCChannel>  ChannelsMap; // Channels map
 
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
        void bot(size_t pfdsindex);
        /*----------- Anything related to channels and messages ------------- */
        // JOIN
        void HandleJOIN(size_t pfdsindex, std::vector<std::string> args);
        void addChannel(int fd, std::string chName, std::string chPass);
        // PRIVMSG
        void PRIVMSG_Handle(size_t pfdsindex, std::vector<std::string> args);
        void PRIVMSG_handdleMSG(std::size_t pfdsindex, std::vector<std::vector<std::string> > args);
        void PRIVMSG_messagToClient(std::size_t pfdsindex, std::string &clientNick, std::string &msg);
        // NOTICE
        void NOTICE_Handle(size_t pfdsindex, std::vector<std::string> args);
        void NOTICE_handdleMSG(std::size_t pfdsindex, std::vector<std::vector<std::string> > args);
        void NOTICE_messagToClient(std::size_t pfdsindex, std::string &clientNick, std::string &msg);
};

/* -------------------------------------------------------------------------- */
// helper functions
void    writeMessageToClient(int fd, std::string message);



/* -------------------------------------------------------------------------- */
// PRIVMSG_NOTICE_utils
void                                    stringTrim(std::string &str, const char *to_trim);
std::string                             is_duplicate(std::vector<std::string> receivers);
std::vector<std::vector<std::string> >  finalData(std::vector<std::string> buffer);
std::vector<std::vector<std::string> >  parseArgs(std::string args);

#endif