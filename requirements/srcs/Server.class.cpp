/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abellakr <abellakr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 18:31:53 by abellakr          #+#    #+#             */
/*   Updated: 2023/05/05 19:02:06 by abellakr         ###   ########.fr       */
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
    std::cout << "waiting for connections port : " << this->PORT << std::endl;  
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
    // SaveClients(newsockfd, ClientAddr.sin_addr.s_addr);
    // std::cout << "new connection established from client IP:" << inet_ntoa(ClientAddr.sin_addr) << " sockfd:" << newsockfd<< std::endl;
}

void Server::HandleConnections(size_t i)
{
    char buffer[MAX_INPUT + 1] = {0};
    int valread = read(pfds[i].fd, buffer, sizeof(buffer));
    if(valread < 0)
            throw std::runtime_error("read failed");
    else if(valread > 0)
    {
        // std::cout << buffer << std::endl;
        for(size_t j = 1; j < pfds.size(); j++)
        {
            if((pfds[j].revents & POLLOUT) && j != i)
            {
                int valwrite = write(pfds[j].fd, buffer, sizeof(buffer));
                if(valwrite < 0)
                    throw std::runtime_error("write failed");
            }                          
        }
    }
}

void SaveClients(int newsockfd, unsigned int IP)
{
    Client new_client(newsockfd, IP);
    // std::cout << new_client.abdellah << std::endl;
//     std::pair<int,Client> Mypair; // pair to be inserted into the map of clients

//     Mypair.first = newsockfd;
//     Mypair.second = new_client;
//     ClientsMap.insert(Mypair); 
}

Server::~Server()
{
    
}


