/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abellakr <abellakr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 18:31:53 by abellakr          #+#    #+#             */
/*   Updated: 2023/05/17 12:17:08 by abellakr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../ircserv.head.hpp"
// static int counter = 0;
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
    std::cout << "Server started waiting for connections port : " << this->PORT << " | PASS : " << PASSWORD << std::endl;  
    std::cout << "To athenticate to the server set in order:\n1.password: \"pass + <password>\"\n2.nickname: \"nick + <nickname>\"\n3.username: \"user + <username> <hostname> <servername> <realname>\"" << std::endl;
    std::cout << "<-----------------------> waiting ... <------------------------->" << std::endl;
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
    int valread = read(pfds[pfdsindex].fd, buffer, MAX_INPUT);
    std::string bufferobj = buffer;
    if(valread < 0)
        throw std::runtime_error("read failed");
    else if(valread == 0)
    {
        std::cout << "client disconnected sockfd: " << pfds[pfdsindex].fd << std::endl; 
        // remove client from map
        std::map<int,Client>::iterator itmap = ClientsMap.find(pfds[pfdsindex].fd);
        ClientsMap.erase(itmap);
        // remove client from pollfd vector
        std::vector<pollfd>::iterator itvec = pfds.begin();
        itvec += pfdsindex;
        pfds.erase(itvec);
        // remove client from channel
    }
    else if(valread > 0)
    {
        std::map<int,Client>::iterator it = ClientsMap.find(pfds[pfdsindex].fd);
        Client& tmp = it->second;
        size_t pos = 0;
        while((pos = bufferobj.find("\r\n", pos)) != std::string::npos)
        {
            bufferobj.replace(pos, 2, "\n");
            pos += 2;
        }
        if(bufferobj.find_first_of("\n") == std::string::npos)
        {

            tmp.setbuffer(tmp.getbuffer() + bufferobj);
            return;
        }
        else
            tmp.setbuffer(tmp.getbuffer() + bufferobj);        
        std::stringstream stream(tmp.getbuffer());
        std::string token;
        while(std::getline(stream , token, '\n'))
        {
            // std::cout << "-------------  Debuging1 -------------:" << token << std::endl; 
            MS.clear();
            std::string data = token;
            MS.push_back(data.substr(0, data.find_first_of(" ")));
            // for (std::size_t i = 0;i < MS.size();i++){
            //     std::cout << "MS[" << i << "] = " << MS[i] << std::endl;
            // }
            if(data.find(" ") != std::string::npos)
                MS.push_back(data.substr(data.find_first_of(" ") + 1));
            // for (std::size_t i = 0;i < MS.size();i++){
            //     std::cout << "MS[" << i << "] = " << MS[i] << std::endl;
            // }
            // std::cout << "-------------  Debuging2 -------------:" << data << std::endl; 
            if(Authentication(pfdsindex) == true)
            {
                // the client is authenticated to the server
                // here I will parse the commads
                if(tmp.getfirstATH() == true)
                    executecommand(pfdsindex);
                //------------------------------------------------ broadcast
                // for(size_t j = 1; j < pfds.size(); j++)
                // {
                //     if((pfds[j].revents & POLLOUT) && (j != pfdsindex) && tmp.getfirstATH() == true)
                //         writemessagetoclients(j, data + "\n");
                // }
                //--------------------------------------------------- broadcast
            }  
        }
        token.clear();
        tmp.setbuffer("");
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
            std::cout << "client connected sockfd: " << pfds[pfdsindex].fd << std::endl; 
            RPL_WELCOME(pfdsindex, tmp.getNICKNAME(), tmp.getUSERNAME());
            RPL_YOURHOST(pfdsindex);
            RPL_CREATED(pfdsindex, Servtimeinfo);
            tmp.settime(ft_gettime());
        }
        tmp.setAuthenticated(true);
        return true;
    }
    return false;
}

void    Server::checkpass(size_t pfdsindex, Client& client)
{
    if((MSATH[0] == "PASS" || MSATH[0] == "pass") && client.getVP() == false)
    {
        if(MSATH.size() < 2)
        {
            ERR_NEEDMOREPARAMS(pfdsindex,MSATH[0]);
        }
        else if(MSATH[1] == PASSWORD)
            client.setVP(true);
    }
    else if((MSATH[0] == "PASS" || MSATH[0] == "pass" ) && client.getVP() == true && (pfds[pfdsindex].revents & POLLOUT))
    {
        ERR_ALREADYREGISTRED(pfdsindex);
    }
}

void Server::checknick(size_t pfdsindex, Client& client)
{
    if((MSATH.size() < 2) && (MSATH[0] == "NICK" || MSATH[0] == "nick") && client.getVN() == false && client.getVP() == true)
    {
        ERR_NONICKNAMEGIVEN(pfdsindex);
    }
    else if((MSATH[0] == "NICK" || MSATH[0] == "nick") && client.getVN() == false && client.getVP() == true)
    {
        if((MSATH[1][0] > 'a' && MSATH[1][0] < 'z') || (MSATH[1][0] > 'A' && MSATH[1][0] < 'Z'))
        {
            std::map<int, Client>::iterator it = ClientsMap.begin();
            while(it != ClientsMap.end())
            {
                if(it->second.getNICKNAME() == MSATH[1])
                    break;
                it++;   
            }
            if(it != ClientsMap.end())
            {
                ERR_NICKNAMEINUSE(pfdsindex, MSATH[1]);
            }
            else if(it == ClientsMap.end())
            {
                client.setNICKNAME(MSATH[1]);
                client.setVN(true);
            }
        }
        else
        {
            ERR_ERRONEUSNICKNAME(pfdsindex, MSATH[1]);
        }
    }
    else if((MSATH[0] == "NICK" || MSATH[0] == "nick") && client.getVN() == true && client.getVP() == true)
        client.setNICKNAME(MSATH[1]);
}

void Server::checkuser(size_t pfdsindex, Client& client)
{
    if((MSATH.size() < 5) && (MSATH[0] == "USER" || MSATH[0] == "user") && client.getVU() == false && client.getVP() == true && client.getVN() == true)
    {
        ERR_NEEDMOREPARAMS(pfdsindex, MSATH[0]);
    }
    else if((MSATH[0] == "USER" || MSATH[0] == "user") && client.getVU() == false && client.getVP() == true && client.getVN() == true)
    {
        std::map<int, Client>::iterator it = ClientsMap.begin();
        while(it != ClientsMap.end())
        {
            if(it->second.getUSERNAME() == MSATH[1])
                break;
            it++;
        }
        if(it != ClientsMap.end())
        {
            ERR_ALREADYREGISTRED(pfdsindex);
        }
        else if(it == ClientsMap.end())
        {
            client.setUSERNAME(MSATH[1]);   
            client.setREALNAME(MSATH[2]);
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
    MSATH.clear();
    if(MS.size() > 1)
    {
        MSATH.push_back(MS[0]);
        std::string tmp = MS[1];
        std::istringstream iss(tmp);
        std::string substring;
        while (iss >> substring)
            MSATH.push_back(substring);
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
    std::cout << "here:      "<<MS[1] << std::endl;
    if(MS[0] == "BOT" || MS[0] == "bot") // bot
        bot(pfdsindex);
    else if(MS[0] == "NICK" || MS[0] == "nick") // nick 
        std::cout << "NICK\n";
    else if(MS[0] == "MODE" || MS[0] == "mode") // mode
        std::cout << "MODE\n";
    else if(MS[0] == "QUIT" || MS[0] == "quit") // quit
        std::cout << "QUIT\n";
    else if(MS[0] == "JOIN" || MS[0] == "join") // join
        this->AddChannel(MS[1]);
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


long	Server::ft_gettime(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return (current_time.tv_sec);
}