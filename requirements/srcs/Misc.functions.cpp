#include "../ircserv.head.hpp"

/* -------------------------------------------------------------------------- */
void    writeMessageToClient(int fd, std::string message)
{
    int valwrite = write(fd, message.c_str() ,message.length());
    if(valwrite < 0)
        throw std::runtime_error("write failed");
}


/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void Server::removeChannel(std::string chName){
    std::map<std::string, IRCChannel>::iterator channelIt = \
    this->ChannelsMap.find(chName);
    if(channelIt != this->ChannelsMap.end() && channelIt->second.empty()){
        this->ChannelsMap.erase(channelIt);
    }
}