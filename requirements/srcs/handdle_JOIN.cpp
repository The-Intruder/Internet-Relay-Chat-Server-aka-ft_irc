#include "../ircserv.head.hpp"


std::vector<std::string>    parseLine(std::string line){
    std::stringstream           ss(line);
    std::vector<std::string>    tokens;
    std::string                 token;

    while(std::getline(ss, token, ',')){
        tokens.push_back(token);
        token.clear();
    }
    return tokens;
}


std::pair<std::vector<std::string>, std::vector<std::string> >   parse_JOIN(std::string args){
    std::stringstream           ss(args);
    std::vector<std::string>    channels;
    std::vector<std::string>    keys;
    std::string                 line;
    bool                        isChnl;

    isChnl = true;
    while(std::getline(ss, line, ' ')){
        if(isChnl){
            channels = parseLine(line);
            isChnl = !isChnl;
        }else
            keys = parseLine(line);
    }
    // for testing start
    for(std::size_t i = 0; i < channels.size(); i++){
        std::cout << "CHannel: " << channels[i] << std::endl;
    }
    for(std::size_t i = 0; i < keys.size(); i++){
        std::cout << "Keys: " << keys[i] << std::endl;
    }
    // for testing end
    if (channels.size() > 0 && keys.size() > 0)
        return std::make_pair(channels, keys);
    else
        return std::make_pair(channels, std::vector<std::string>());
}


void    Server::addChannel(size_t pfdsindex, std::string chName, std::string chPass){
    std::cout << pfdsindex << std::endl;
    IRCChannel chnl(chName, chPass);
    chnl.addClient(pfdsindex, this->ClientsMap.find(this->pfds[pfdsindex].fd)->second);
    this->ChannelsMap.insert(std::make_pair(chName, chnl));
}

void Server::RemoveChannel(){
    std::cout << "Not implemented yet" << std::endl;
}

void Server::HandleJOIN(size_t pfdsindex, std::string args){
    std::pair<std::vector<std::string>, std::vector<std::string> > parsedArgs =  parse_JOIN(args);
    for (std::size_t i=0;i<parsedArgs.first.size();i++){
        std::string chPass = i < parsedArgs.second.size() ? parsedArgs.second[i] : "";
        if (this->ChannelsMap.find(parsedArgs.first[i]) == this->ChannelsMap.end()){
            addChannel(pfdsindex, parsedArgs.first[i], chPass);

            std::cout << "channel added!" << std::endl;
        } else {
            std::cout << "Already exist!" << std::endl;
        }
    }

}



/*
TODO: JOIN command

*/