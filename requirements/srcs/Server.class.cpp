/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abellakr <abellakr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 18:31:53 by abellakr          #+#    #+#             */
/*   Updated: 2023/05/07 01:18:20 by abellakr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../ircserv.head.hpp"

Server::Server(int PORT, std::string PASSWORD) : PORT(PORT) , PASSWORD(PASSWORD)
{
    SetupServer();
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
                HandleConnections(i);
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
    else if(valread > 0)
    {
        // std::cout << buffer << std::endl; // print buffer in server side
        std::string data = buffer; //
        size_t fi = data.find(" ", 0);
        if(fi == std::string::npos)
            writemessagetoclients(pfdsindex, "461 * ERR_NEEDMOREPARAMS\n", sizeof("461 * ERR_NEEDMOREPARAMS\n"));
        MS.clear();
        MS.push_back(data.substr(0 , fi));
        MS.push_back(data.substr(fi + 1, data.length() - fi - 2));
        if(Authentication(pfdsindex) == true)
        {
            // the client is authenticated to the server
            // -------------------------------------------------- broadcast
            for(size_t j = 1; j < pfds.size(); j++)
                if((pfds[j].revents & POLLOUT) && j != pfdsindex)
                    writemessagetoclients(j, buffer, sizeof(buffer));
            // ---------------------------------------------------------- broadcast
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
    if((MS[0] == "PASS" || MS[0] == "pass" ) && tmp.getVP() == false)
        if((pfds[pfdsindex].revents & POLLOUT) && MS[1] == PASSWORD)
        {
            writemessagetoclients(pfdsindex, "pass valid", 10);
            tmp.setVP(true);
        }  
    else if ((MS[0] == "PASS" || MS[0] == "pass" ) && tmp.getVP() == true)
            writemessagetoclients(pfdsindex, "462 * ERR_ALREADYREGISTRED", sizeof("462 * ERR_ALREADYREGISTRED"));
    return false;
}

Server::~Server()
{
    
}


// write function to send message to the client

void Server::writemessagetoclients(size_t pfdsindex, std::string message, int messagelen)
{
    int valwrite = write(pfds[pfdsindex].fd, message.c_str() ,messagelen);
    if(valwrite < 0)
        throw std::runtime_error("write failed");
}