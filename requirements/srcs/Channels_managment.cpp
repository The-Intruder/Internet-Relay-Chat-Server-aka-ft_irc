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
    for(std::size_t i = 0; i < channels.size(); i++){
        std::cout << "CHannel: " << channels[i] << std::endl;
    }
    for(std::size_t i = 0; i < keys.size(); i++){
        std::cout << "Keys: " << keys[i] << std::endl;
    }
    if (channels.size() > 0 && keys.size() > 0)
        return std::make_pair(channels, keys);
    else
        return std::make_pair(channels, std::vector<std::string>());
}

/*
TODO: fix getting the argument from 'abdellah part'

*/


void Server::AddChannel(std::string args){

    // std::cout << "---- this one: " << args << std::endl;
    parse_JOIN(args);
    if (this->ChannelsMap.find(args) == this->ChannelsMap.end()){
        IRCChannel chnl(args, "pass");
        this->ChannelsMap.insert(std::make_pair(args, chnl));

        std::cout << "channel added!" << std::endl;
    } else {
        std::cout << "Already exist!" << std::endl;
    }

}

void Server::RemoveChannel(){
    std::cout << "Not implemented yet" << std::endl;
}