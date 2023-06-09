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
# define  IRCSERV_HEAD_HPP

/* ------------------------------- LIBRARIES -------------------------------- */

# include <map>
# include <vector>
# include <string>
# include <time.h>
# include <utility>
# include <sstream>
# include <cstring>
# include <cstdlib>
# include <fcntl.h>
# include <netdb.h>
# include <limits.h>
# include <unistd.h>
# include <iostream>
# include <sys/time.h>
# include <sys/poll.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include "srcs/errors.responces.macros.hpp"

// error replies macros
// #define ERR_NEEDMOREPARAMS 461
// #define ERR_ALREADYREGISTRED 462

/* -------------------------------- STRUCTS --------------------------------- */

typedef struct s_parsedModeCommand
{
    std::string    channel_name;
    std::string    mode;
    std::string    parameter;
}   t_parsedModeCommand;

typedef struct s_parsedInviteCommand
{
    std::string    nickname;
    std::string    channel_name;
}   t_parsedInviteCommand;

/* -------------------------------- CLASSES --------------------------------- */

class Client
{
    private:
        __uint32_t      IP;             // IP adress of the client
        std::string     NICKNAME;
        std::string     USERNAME;
        std::string     REALNAME;
        bool            VP;             // is the password validated
        bool            VN;             // is nickname validated
        bool            VU;             // is USER validated
        bool            Authenticated;  // hadi matkhdmoch biha 
        int             sockfd; 
        bool            firstATH;       // ila biti t3rf wach ithentificated khdm bhada
        std::string     buffer;         // each client buffer
        long            connectedtime;  // time to connect to the server
    public:
        Client(int newsockfd, unsigned int IP);
        Client(const Client& other);
        Client& operator=(const Client& other);
        ~Client();

        __uint32_t      getIP();
        bool            getVP() const;
        bool            getVN() const;
        bool            getVU() const;
        bool            getAuthenticated() const;
        int             getSockfd() const;
        std::string     getNICKNAME() const;
        std::string     getUSERNAME() const;
        std::string     getREALNAME() const;
        bool            getfirstATH() const;
        std::string     getbuffer() const;
        long            gettime() const;

        void    setVP(bool v);
        void    setVN(bool v);
        void    setVU(bool v);
        void    setAuthenticated(bool v);
        void    setSockfd(int v);
        void    setNICKNAME(std::string NICK);
        void    setUSERNAME(std::string USERNAME);
        void    setREALNAME(std::string REALNAME);
        void    setfirstATH(bool v);
        void    setbuffer(std::string arg);
        void    settime(long time);
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

class Channel {
    public:
        std::map<int,Client>            _joinedClients; // Clients that joined specifec channel
        std::map<int,Client>            _admins;        // Admins of a specifec channel
        std::map<int,Client>            _operators;     // Operators of a specifec channel
        std::map<int,Client>            _bannedClients; // Banned Clients
        std::map<int,Client>            _voicedClients; // Voiced Clients under a moderated channel
        std::string                     _channel_name;
        std::string                     _channel_pass;
        std::string                     _topic;
        uint64_t                        _client_limit;
        uint32_t                        _modes;

    public:
        Channel(std::string channelName, std::string channelPass);
        Channel(Channel const &src);
        const Channel &operator=(Channel const &src);
        ~Channel();

        bool        isPrivate() const;
        bool        isSecret() const;
        bool        isInviteOnly() const;
        bool        isOnlyVoiceAndOps() const;
        bool        isNoOutsideMessages() const;
        bool        isOnlyOpsChangeTopic() const;

        void        setChannelName(const std::string &channelName);
        std::string getChannelName() const;

        void        setClientLimit(uint64_t _client_limit);
        void        setClientPass(std::string channel_pass);

        uint64_t    getClientLimit() const;
        std::string getChannelPass() const;
        std::string getChannelTopic() const;

        void        joinChannel(Client &client, std::string &chPass, int fd);
        void        notifyUsers(int fd);
        void        welcomeUser(int fd);

        void    addAdmin(int fd);
        void    addOperator(std::pair<int,Client> client);
        void    addVoiced(std::pair<int,Client> client);
        void    banClient(std::pair<int,Client> client);

};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

class Server
{
    public:
        int                             PORT;           // argument port
        int                             servsockfd;     // socket file descriptor of the server
        std::string                     PASSWORD;       // password of the server
        std::string                     Servtimeinfo;   // server created time
        std::string                     buffertmp;      // this is for ignoring control D behavior 
        struct sockaddr_in              ServAddr;       // socket address of the server
        std::vector<pollfd>             pfds;           // file descriptors to keep eyes on 
        std::vector<std::string>        MS;             // message splited by space
        std::vector<std::string>        MSATH;          // mesaage splited by space for ATH phase
        std::map<std::string,Channel>   ChannelsMap;    // Channels map
        std::map<int,Client>            ClientsMap;     // clients map
 
    public:
        Server(int PORT, std::string PASSTWORD);
        ~Server();

        void    SetupServer(void);
        void    AcceptConnections();
        void    HandleConnections(size_t i);                   // Handle connections
        void    SaveClients(int newsockfd, unsigned int IP);   // save the connected client to the map of clients

        bool    Authentication(size_t pfdsindex);
        void    checkpass(size_t pfdsindex, Client& client);
        void    checknick(size_t pfdsindex, Client& client);
        void    checkuser(size_t pfdsindex, Client& client);
        
        void    writemessagetoclients(size_t pfdsindex, std::string message); // pfdsindex is the fd socket of the client to send data to *
        void    splitargs();
        void    getDateTime();
        long    ft_gettime(void);
        void    executecommand(size_t pfdsindex);
        void    bot(size_t pfdsindex);
        void    HandleJOIN(size_t pfdsindex, std::string args);
        void    addChannel(int fd, std::string chName, std::string chPass);
        void    RemoveChannel();

        void    executeModeCommand(size_t pfdsindex, std::vector<std::string> &full_cmd);
        void    executeInviteCommand(size_t pfdsindex, std::vector<std::string> &full_cmd)
};

/* -------------------------------- PROTOTYPES ------------------------------ */

void    writemessagetoclients(int fd, std::string message);

#endif // SERVER_HPP
