/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abellakr <abellakr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 13:04:15 by abellakr          #+#    #+#             */
/*   Updated: 2023/05/05 13:12:52 by abellakr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ircserv.head.hpp"

// constractor parametrized of my simple server
Server::Server(int PORT, std::string PASSWORD) : PORT(PORT) , PASSWORD(PASSWORD)
{
    // setup the server
    SetupServer();
    int newsockfd = 0;
    while(true)
    {
        // ********************************************
        // wait for the incoming connection from any socket
        // ********************************************
        int pollv = poll(&pfds[0], pfds.size(), -1);
        if(pollv < 0)
            throw std::runtime_error("poll failed");
        // ********************************************
        // Check for incoming connections on the server socket
        // ********************************************
        for(size_t i = 0; i < pfds.size(); i++)
           {       
                if(pfds[i].revents & POLLIN && i == 0)
                {
                     struct sockaddr_in ClientAddr;
                     int addrlen = sizeof(ClientAddr);
                     // ********************************************
                     // accept the incoming connection 
                     // ********************************************
                     newsockfd = accept(servsockfd, (struct sockaddr *)&ClientAddr, (socklen_t *)&addrlen);
                     if(newsockfd < 0)
                         throw std::runtime_error("accept failed");
                    std::cout << "new connection established from client IP:" << inet_ntoa(ClientAddr.sin_addr) << " sockfd:" << newsockfd<< std::endl;
                    // ********************************************     
                    // set the new connection to non blocking mode 
                    // ********************************************     
                     int fl = fcntl(newsockfd, F_SETFL, O_NONBLOCK);
                     if(fl < 0)
                         throw std::runtime_error("fcntl failed");
                     // ********************************************
                    // add the new connection the vector
                    // ********************************************
                     struct pollfd tmpfd;
                     tmpfd.fd = newsockfd;
                     tmpfd.events = POLLIN | POLLOUT;
                     pfds.push_back(tmpfd);
                }
                // ********************************************     
                // connection phase : send and receive | read and write
                // ********************************************     
                else if(pfds[i].revents & POLLIN)
                {
                     char buffer[MAX_INPUT + 1] = {0};
                     int valread = read(pfds[i].fd, buffer, sizeof(buffer));
                    //  std::cout << valread << std::endl;
                    if(valread < 0)
                         throw std::runtime_error("read failed");
                    else if(valread == 0)
                            continue; 
                    else
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
           }
    } 
    close(newsockfd);
    close(servsockfd); 
    
}

Server::~Server()
{
    
}

// TODO: code split
// setup server : done
// wait for the connection : while(true)
// accept the connection
// handle the connection



void    Server::SetupServer()
{
    // ******************************************
   // create socket
  // ********************************************
   servsockfd = socket(AF_INET, SOCK_STREAM, 0);
   if(servsockfd < 0)
       throw std::runtime_error("cannot create socket");
    // ********************************************
    // allow the socket descriptor to be reuseable
    // ********************************************
    int on = 1;
    int rc = setsockopt(servsockfd, SOL_SOCKET, SO_REUSEADDR,  (char *)&on, sizeof(on));
    if(rc < 0)
        throw std::runtime_error("setsockopt failed");
    // ********************************************
    //  set the socket to be non blocking
    // ********************************************
    int fl = fcntl(servsockfd, F_SETFL, O_NONBLOCK);
    if(fl < 0)
        throw std::runtime_error("fcntl failed");
    // ********************************************
    // before calling bind we need to fill the sockaddr_in
    // ********************************************
   memset((char *)&ServAddr, 0 , sizeof(ServAddr));
   ServAddr.sin_family = AF_INET;
   ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
   ServAddr.sin_port = htons(this->PORT);
    // ********************************************
    // associate a socket with a specific network address and port number
    // ********************************************
    if(bind(servsockfd, (struct sockaddr *)&ServAddr, sizeof(ServAddr)) < 0)
        throw std::runtime_error("bind failed");
    // ********************************************
    // wait for the incoming connection (listen)
    // ********************************************
    if(listen(servsockfd, SOMAXCONN) < 0)
        throw std::runtime_error("listen failed");    
    // ********************************************
    // initialize the pollfd structure
    // ********************************************
    struct pollfd tmpfd;
    tmpfd.fd = servsockfd;
    tmpfd.events = POLLIN;
    pfds.push_back(tmpfd);
    std::cout << "----waiting for the connection in port : " << this->PORT << "----"<< std::endl;  
}