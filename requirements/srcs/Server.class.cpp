/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abellakr <abellakr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 18:31:53 by abellakr          #+#    #+#             */
/*   Updated: 2023/05/08 21:04:25 by abellakr         ###   ########.fr       */
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
    std::map<int,Client>::iterator it = ClientsMap.find(pfds[pfdsindex].fd);
    Client& tmp = it->second;
    char buffer[MAX_INPUT + 1] = {0};
    int valread = read(pfds[pfdsindex].fd, buffer, sizeof(buffer));
    if(valread < 0)
            throw std::runtime_error("read failed");
    else if(valread > 0)
    {
        MS.clear();
        std::string data = buffer; 
        data.pop_back();
        // std::cout << "'" << data << "' " << data.length() << std::endl; 
        MS.push_back(data.substr(0, data.find_first_of(" ")));
        if(data.find(" ") != std::string::npos)
            MS.push_back(data.substr(data.find_first_of(" ") + 1));
        if(Authentication(pfdsindex) == true)
        {
            // the client is authenticated to the server
            // here I will parse the commads
            //------------------------------------------------ broadcast
            for(size_t j = 1; j < pfds.size(); j++)
            {
                if((pfds[j].revents & POLLOUT) && (j != pfdsindex) && tmp.getfirstATH() == true)
                    writemessagetoclients(j, data);
            }
            //--------------------------------------------------- broadcast
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
    // check pass
    checkpass(pfdsindex, tmp);
    // check nick
    checknick(pfdsindex, tmp);
    //check user
    checkuser(pfdsindex, tmp);
    if(tmp.getVP() == true &&  tmp.getVU() == true && tmp.getVN() == true)
    {
        if(tmp.getAuthenticated() == false)
        {            
            std::string tmp = "successfully authenticated\n";
            writemessagetoclients(pfdsindex, tmp);
        }
        tmp.setAuthenticated(true);
        return true;
    }
    return false;
}



///////////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////
void Server::checknick(size_t pfdsindex, Client& client)
{
    if(MS.size() < 2)
    {
        ERR_NONICKNAMEGIVEN(pfdsindex);
    }
    else if((MS[0] == "NICK" || MS[0] == "nick") && client.getVN() == false && client.getVP() == true)
    {
        client.setNICKNAME(MS[1]);
        client.setVN(true);
    }
    else if((MS[0] == "NICK" || MS[0] == "nick") && client.getVN() == true && client.getVP() == true)
    {
        client.setNICKNAME(MS[1]);
        client.setVN(true);
        std::string tmp = "WiZ changed his nickname to " + MS[1] + "\n";
         writemessagetoclients(pfdsindex, tmp);
    }
}
//////////////////////////////////////////////////////////////////////
void Server::checkuser(size_t pfdsindex, Client& client)
{
    if((MS[0] == "USER" || MS[0] == "user") && client.getVU() == false && client.getVP() == true && client.getVN() == true)
    {
        client.setUSERNAME(MS[1]);   
        client.setREALNAME(MS[2]);
        std::string tmp = "USER !!";
         writemessagetoclients(pfdsindex, tmp);
        client.setVU(true);   
    }
    // else if((MS[0] == "USER" || MS[0] == "user") && client.getVU() == true && client.getVP() == true && client.getVN() == true)
    // {
        
    // }
}
///////////////////////////////////////////////////////////////////////////


// function to send message to the client
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