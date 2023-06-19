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
class Channel;

typedef struct s_pmc
{
    Channel*        channel;
    std::string     mode;
    std::string     parameter;
}   t_pmc;

typedef struct s_pic
{
    int         clients_fd;
    Channel*    channel;
}   t_pic;

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
        std::map<int,Client>            _joinedUsers; // Clients that joined specifec channel
        std::map<int,Client>            _admins;        // Admins of a specifec channel
        std::map<int,Client>            _operators;     // Operators of a specifec channel
        std::map<int,Client>            _bannedUsers; // Banned Clients
        std::map<int,Client>            _voicedClients; // Voiced Clients under a moderated channel
        std::string                     _channel_name;
        std::string                     _channel_pass;
        std::string                     _topic;
        uint64_t                        _client_limit;
        uint32_t                        _modes;
        std::string                     _key;

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
        bool        empty() const;
        void        addAdmin(int fd);
        void        joinChannel(Client &client, std::string &chPass, int fd);
        void        notifyUsers(int fd);
        void        welcomeUser(int fd);
        void        PRIVMSG_messagToChannel(int fd, std::string &msg);
        void        NOTICE_messagToChannel(int fd, std::string &msg);
        void        leftChannel(int fd, std::string &msg);
        void        sayGoodby(int fd, std::string &msg);
        void        kickFromChan(int kickerFd, std::string &userToKick, std::string &comment);
        bool        isClientOnChan(int fd);
        void        sendTopicToClient(int fd);
        void        changeTopic(int fd, std::string &topic);
        void        removeUser(int fd);
        void        addOperator(std::pair<int,Client> client);
        void        addVoiced(std::pair<int,Client> client);
        void        banClient(std::pair<int,Client> client);
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

class Server
{
    public:
        int PORT; // argument port
        std::string PASSWORD; // password of the server
        int servsockfd; // socket file descriptor of the server0
        struct sockaddr_in ServAddr; // socket address of the server
        std::vector<pollfd> pfds; // file descriptors to keep eyes on 
        std::map<int,Client>  ClientsMap; // clients map
        std::vector<std::string> MS; // message splited by space
        std::vector<std::string> MSATH; // mesaage splited by space for ATH phase
        std::string Servtimeinfo; // server created time
        std::string buffertmp; // this is for ignoring control D behavior 
        /*---------------------- Hssain-Part ------------------ */
        std::map<std::string,Channel>  ChannelsMap; // Channels map
 
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
        
        void writeMessageToClient(size_t pfdsindex, std::string message); // pfdsindex is the fd socket of the client to send data to *
        void splitargs();
        void getDateTime();
        long ft_gettime(void);
        void executecommand(size_t pfdsindex);
        // commands
        void    bot(size_t pfdsindex);
        void    nick(size_t pfdsindex);
        void    quit(size_t pfdsindex);

        /*---------------------- Hssain-Part ------------------ */
        /*----------- Anything related to channels and messages ------------- */
        void HandleJOIN(size_t pfdsindex, std::string args);
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
        // PART
        void PART_Handle(size_t pfdsindex, std::vector<std::string> args);
        void PART_trigger(std::size_t pfdsindex, std::vector<std::vector<std::string> > args);
        // KICK
        void KICK_Handle(size_t pfdsindex, std::vector<std::string> args);
        void KICK_trigger(std::size_t pfdsindex, std::vector<std::string> args);
        // TOPIC
        void TOPIC_Handle(std::size_t pfdsindex, std::vector<std::string> args);
        void TOPIC_trigger(std::size_t pfdsindex, std::vector<std::string> args);
        // Misc
        void removeClientFromChans(int fd);
        void removeChannel(std::string chName);

        /* ---- AMINE'S PART ------------------------------------------------ */
        Channel*                        get_channel(std::string channel_name);
        std::pair<const int, Client>    &get_client(std::string nickname, Channel* channel, size_t pfdsindex);
        struct s_pmc    parse_mode_command(std::string &command, size_t pfdsindex);
        void    set_operator(bool add, size_t pfdsindex, t_pmc &pmc);
        void    set_voiced(bool add, size_t pfdsindex, t_pmc &pmc);
        void    ban_client(bool add, t_pmc &pmc, size_t pfdsindex);
        void    set_private(bool add, t_pmc &pmc, size_t pfdsindex);
        void    set_secret(bool add, t_pmc &pmc, size_t pfdsindex);
        void    set_invite_only(bool add, t_pmc &pmc, size_t pfdsindex);
        void    set_only_ops_change_topic(bool add, t_pmc &pmc, size_t pfdsindex);
        void    set_no_outside_messages(bool add, t_pmc &pmc, size_t pfdsindex);
        void    set_only_voice_and_ops(bool add, t_pmc &pmc, size_t pfdsindex);
        void    set_client_limit(bool add, t_pmc &pmc, size_t pfdsindex, std::string cmd);
        void    set_key(bool add, t_pmc &pmc, size_t pfdsindex);
        void    execute_mode_command(size_t pfdsindex, std::vector<std::string> &full_cmd);
        void    execute_invite_command(size_t pfdsindex, std::vector<std::string> &full_cmd);
        void    is_operator(Channel& channel, size_t pfdindex);
        void    is_invite_operator(Channel& channel, size_t pfdindex);
        int     get_client_fd(std::string nickname, size_t pfdsindex);
        t_pic   parse_invite_command(std::string &command, size_t pfdsindex);
};

/* -------------------------------------------------------------------------- */
// Misc functions
void    writeMessageToClient_fd(int fd, std::string message);



/* -------------------------------------------------------------------------- */
// PRIVMSG_NOTICE_utils
void                                    stringTrim(std::string &str, const char *to_trim);
std::string                             is_duplicate(std::vector<std::string> receivers);
std::vector<std::vector<std::string> >  finalData(std::vector<std::string> buffer);
std::vector<std::vector<std::string> >  parseArgs(std::string args);

#endif // SERVER_HPP
