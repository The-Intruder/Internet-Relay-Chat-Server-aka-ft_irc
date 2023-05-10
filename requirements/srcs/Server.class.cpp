/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abellakr <abellakr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 18:31:53 by abellakr          #+#    #+#             */
/*   Updated: 2023/05/10 15:51:54 by abellakr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../ircserv.head.hpp"

Server::Server(int PORT, std::string PASSWORD) : PORT(PORT) , PASSWORD(PASSWORD)
{
    SetupServer();
    getDateTime();
    while(true)
    {
        int pollv = poll(&pfds[0], pfds.size(), -1);
        if(pollv < 0)
            throw std::runtime_error("poll failed");
        for(size_t i = 0; i < pfds.size(); i++)
        {       
            if(pfds[i].revents & POLLIN && i == 0)
                AcceptConnections();
            else if(pfds[i].revents & POLLIN)
            {
                std::map<int,Client>::iterator it = ClientsMap.find(pfds[i].fd);
                Client& tmp = it->second;
                HandleConnections(i);
                if(tmp.getAuthenticated() == true)
                    tmp.setfirstATH(true);
                
            }
        }
    }
    close(servsockfd);
}

void    Server::SetupServer()
{
   servsockfd = socket(AF_INET, SOCK_STREAM, 0);
   if(servsockfd < 0)
       throw std::runtime_error("cannot create socket");
    int on = 1;
    int rc = setsockopt(servsockfd, SOL_SOCKET, SO_REUSEADDR,  (char *)&on, sizeof(on));
    if(rc < 0)
        throw std::runtime_error("setsockopt failed");
    int fl = fcntl(servsockfd, F_SETFL, O_NONBLOCK);
    if(fl < 0)
        throw std::runtime_error("fcntl failed");
   memset((char *)&ServAddr, 0 , sizeof(ServAddr));
   ServAddr.sin_family = AF_INET;
   ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
   ServAddr.sin_port = htons(this->PORT);
    if(bind(servsockfd, (struct sockaddr *)&ServAddr, sizeof(ServAddr)) < 0)
        throw std::runtime_error("bind failed");
    if(listen(servsockfd, SOMAXCONN) < 0)
        throw std::runtime_error("listen failed");    
    struct pollfd tmpfd;
    tmpfd.fd = servsockfd;
    tmpfd.events = POLLIN;
    pfds.push_back(tmpfd);
    std::cout << "waiting for connections port : " << this->PORT << " | PASS : " << PASSWORD << std::endl;  
}

void Server::AcceptConnections()
{
    struct sockaddr_in ClientAddr;
    int addrlen = sizeof(ClientAddr);
    int newsockfd = accept(servsockfd, (struct sockaddr *)&ClientAddr, (socklen_t *)&addrlen);
    if(newsockfd < 0)
        throw std::runtime_error("accept failed");
    int fl = fcntl(newsockfd, F_SETFL, O_NONBLOCK);
    if(fl < 0)
        throw std::runtime_error("fcntl failed");
    struct pollfd tmpfd;
    tmpfd.fd = newsockfd;
    tmpfd.events = POLLIN | POLLOUT;
    pfds.push_back(tmpfd);
    SaveClients(newsockfd, ClientAddr.sin_addr.s_addr);
}

void Server::HandleConnections(size_t pfdsindex)
{
    char buffer[MAX_INPUT + 1] = {0};
    int valread = read(pfds[pfdsindex].fd, buffer, sizeof(buffer));
    if(valread < 0)
            throw std::runtime_error("read failed");
    // else if(valread == 0)
    // {
    //     std::cout << "client disconnected" << std::endl;   
    //     // remove client formr pollfd vector 
    //     // remove client from map
    //     // remove client from channel
    // }
    else if(valread > 0)
    {
        std::map<int,Client>::iterator it = ClientsMap.find(pfds[pfdsindex].fd);
        Client& tmp = it->second;
        char *cmd = std::strtok(buffer, "\r\n");
        while(cmd != NULL)
        {
            MS.clear();
            std::string data = cmd;
            MS.push_back(data.substr(0, data.find_first_of(" ")));
            if(data.find(" ") != std::string::npos)
                MS.push_back(data.substr(data.find_first_of(" ") + 1));
            if(Authentication(pfdsindex) == true)
            {
                // the client is authenticated to the server
                // here I will parse the commads
                if(tmp.getfirstATH() == true)
                    executecommand(pfdsindex);
                //------------------------------------------------ broadcast
                for(size_t j = 1; j < pfds.size(); j++)
                {
                    if((pfds[j].revents & POLLOUT) && (j != pfdsindex) && tmp.getfirstATH() == true)
                        writemessagetoclients(j, data + "\n");
                }
                //--------------------------------------------------- broadcast
            }
            cmd = std::strtok(NULL, "\r\n");
        }
    }
}

void Server::SaveClients(int newsockfd, unsigned int IP)
{
    Client new_client(newsockfd, IP);
    std::pair<int,Client> Mypair = std::make_pair(newsockfd, new_client);
    ClientsMap.insert(Mypair); 
}


bool Server::Authentication(size_t pfdsindex)
{
    std::map<int,Client>::iterator it = ClientsMap.find(pfds[pfdsindex].fd);
    Client& tmp = it->second;
    splitargs();
    checkpass(pfdsindex, tmp);
    checknick(pfdsindex, tmp);
    checkuser(pfdsindex, tmp);
    if(tmp.getVP() == true &&  tmp.getVU() == true && tmp.getVN() == true)
    {
        if(tmp.getAuthenticated() == false)
        {            
            RPL_WELCOME(pfdsindex, tmp.getNICKNAME(), tmp.getUSERNAME());
            RPL_YOURHOST(pfdsindex);
            RPL_CREATED(pfdsindex, Servtimeinfo);
        }
        tmp.setAuthenticated(true);
        return true;
    }
    return false;
}

void    Server::checkpass(size_t pfdsindex, Client& client)
{
    if((MS[0] == "PASS" || MS[0] == "pass") && client.getVP() == false)
    {
        if(MS.size() < 2)
        {
            ERR_NEEDMOREPARAMS(pfdsindex,MS[0]);
        }
        else if(MS[1] == PASSWORD)
            client.setVP(true);
    }
    else if((MS[0] == "PASS" || MS[0] == "pass" ) && client.getVP() == true && (pfds[pfdsindex].revents & POLLOUT))
    {
        ERR_ALREADYREGISTRED(pfdsindex);
    }
}

void Server::checknick(size_t pfdsindex, Client& client)
{
    if((MS.size() < 2) && (MS[0] == "NICK" || MS[0] == "nick") && client.getVN() == false && client.getVP() == true)
    {
        ERR_NONICKNAMEGIVEN(pfdsindex);
    }
    else if((MS[0] == "NICK" || MS[0] == "nick") && client.getVN() == false && client.getVP() == true)
    {
        std::map<int, Client>::iterator it = ClientsMap.begin();
        while(it != ClientsMap.end())
        {
            if(it->second.getNICKNAME() == MS[1])
                break;
            it++;   
        }
        if(it != ClientsMap.end())
        {
            ERR_NICKNAMEINUSE(pfdsindex, MS[1]);
        }
        else if(it == ClientsMap.end())
        {
            client.setNICKNAME(MS[1]);
            client.setVN(true);
        }
    }
    else if((MS[0] == "NICK" || MS[0] == "nick") && client.getVN() == true && client.getVP() == true)
        client.setNICKNAME(MS[1]);
}

void Server::checkuser(size_t pfdsindex, Client& client)
{
    if((MS.size() < 5) && (MS[0] == "USER" || MS[0] == "user") && client.getVU() == false && client.getVP() == true && client.getVN() == true)
    {
        ERR_NEEDMOREPARAMS(pfdsindex, MS[0]);
    }
    else if((MS[0] == "USER" || MS[0] == "user") && client.getVU() == false && client.getVP() == true && client.getVN() == true)
    {
        std::map<int, Client>::iterator it = ClientsMap.begin();
        while(it != ClientsMap.end())
        {
            if(it->second.getUSERNAME() == MS[1])
                break;
            it++;
        }
        if(it != ClientsMap.end())
        {
            ERR_ALREADYREGISTRED(pfdsindex);
        }
        else if(it == ClientsMap.end())
        {
            client.setUSERNAME(MS[1]);   
            client.setREALNAME(MS[2]);
            client.setVU(true);
        }
    }
}

void Server::writemessagetoclients(size_t pfdsindex, std::string message)
{
    int valwrite = write(pfds[pfdsindex].fd, message.c_str() ,message.length());
    if(valwrite < 0)
        throw std::runtime_error("write failed");
}

Server::~Server()
{
    
}

void Server::splitargs()
{
    if(MS.size() > 1)
    {
        std::string tmp = MS[1];
        MS.pop_back();
        std::istringstream iss(tmp);
        std::string substring;
        while (iss >> substring)
            MS.push_back(substring);
    }
}

void Server::getDateTime()
{
    char ServTime[80];
   time_t RawTime;
   struct tm* TimeInfos;
   time(&RawTime);
   TimeInfos = localtime(&RawTime);
   strftime(ServTime, 80, "%Y-%m-%d %H:%M:%S", TimeInfos);
   Servtimeinfo = ServTime;
}

void Server::executecommand(size_t pfdsindex)
{
    if(MS[0] == "BOT" || MS[0] == "bot") // bot
        std::cout << "bot\n";
    else if(MS[0] == "NICK" || MS[0] == "nick") // nick 
        std::cout << "NICK\n";
    else if(MS[0] == "MODE" || MS[0] == "mode") // mode
        std::cout << "MODE\n";
    else if(MS[0] == "QUIT" || MS[0] == "quit") // quit
        std::cout << "QUIT\n"; 
    else if(MS[0] == "JOIN" || MS[0] == "join") // join
        std::cout << "join\n";
    else if(MS[0] == "PART" || MS[0] == "part") // part
        std::cout << "part\n"; 
    else if(MS[0] == "TOPIC" || MS[0] == "topic") // part 
        std::cout << "topic\n"; 
    else if(MS[0] == "NAMES" || MS[0] == "names") // names
        std::cout << "names\n"; 
    else if(MS[0] == "INVITE" || MS[0] == "invite") // invite
        std::cout << "invite\n";
    else if(MS[0] == "KICK" || MS[0] == "kick") // kick
        std::cout << "kick\n";
    else if(MS[0] == "PRIVMSG" || MS[0] == "privmsg") // privmsg
        std::cout << "privmsg\n";
    else if(MS[0] == "NOTICE" || MS[0] == "notice") // notice
        std::cout << "notice\n"; 
    else // command not found
    {
        if(MS[0] != "PING" && MS[0] != "PONG") // ignore PING AND PONG requests from limechat
            ERR_CMDNOTFOUND(pfdsindex);    
    }
}