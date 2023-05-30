#include "../ircserv.head.hpp"

/* -------------------------------------------------------------------------- */

std::vector<std::vector<std::string> >    PART_splitChannels(std::vector<std::string> buffer){
    std::vector<std::vector<std::string> >  result;
    std::vector<std::string>                channels;
    std::vector<std::string>                msg;

    if(buffer.empty())
        throw std::runtime_error("ERR_NEEDMOREPARAMS");
    std::cout << "debug" << std::endl;
    if (buffer[0].find(",")  != std::string::npos){
        std::string token;
        std::stringstream ss(buffer[0]);
        while(std::getline(ss, token, ',')){
            stringTrim(token, " \r\t\n");
            if (!token.empty())
                channels.push_back(token);
            token.clear();
        }
        if(channels.empty()){
            throw std::runtime_error("ERR_NEEDMOREPARAMS");
        }
        result.push_back(channels);
    } else {
        channels.push_back(buffer[0]);
        result.push_back(channels);
    }
    if (buffer.size() > 1){
        msg.push_back(buffer[1]);
        result.push_back(msg);
    }

    return result;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

std::vector<std::vector<std::string> >    PART_parseArgs(std::string args){
    std::string token;
    std::vector<std::string> buffer;

    stringTrim(args, " \r\t\n");
    std::string::size_type colonPos = args.find(' ');
    if (colonPos != std::string::npos) {
        std::string token1 = args.substr(0, colonPos);
        std::string token2 = args.substr(colonPos + 1);
        stringTrim(token1, " \r\t\n");
        stringTrim(token2, " \r\t\n");
        if (!token1.empty())
            buffer.push_back(token1);
        if (!token2.empty())
            buffer.push_back(token2);
    } else{
        buffer.push_back(args);
    }
    return PART_splitChannels(buffer);
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

// void    Server::PRIVMSG_handdleMSG(std::size_t pfdsindex, std::vector<std::vector<std::string> > args){
//     std::vector<std::string>    receivers = args[0];
//     std::string    msg = args[1][0];
//     for (std::vector<std::string>::iterator it = receivers.begin(); it != receivers.end();it++){
//         if (it->at(0) == '#'){
//             std::map<std::string, IRCChannel>::iterator chanl = this->ChannelsMap.find(*it);
//             if (chanl != this->ChannelsMap.end())
//                 chanl->second.PRIVMSG_messagToChannel(this->pfds[pfdsindex].fd, msg);
//             else
//                 ERR_NOSUCHNICK(pfdsindex, this->ClientsMap.find(this->pfds[pfdsindex].fd)->second.getNICKNAME(), *it);
//         } else {
//             this->PRIVMSG_messagToClient(pfdsindex, *it, msg);
//         }
//     }
// }

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void    Server::PART_Handle(size_t pfdsindex, std::vector<std::string> args){
    if (args.size() > 1) {
        stringTrim(args[1], " \r\t\n");
        if (!args[1].empty()){
            try{
                std::vector<std::vector<std::string> > cleanArgs = PART_parseArgs(args[1]);
                std::cout << "Result size: " << cleanArgs[0].size() << std::endl;
                // this->PRIVMSG_handdleMSG(pfdsindex, cleanArgs);
            }catch(const std::exception& e) {
                std::string err = e.what();
                std::cout << "exception cought: " << err << std::endl;
                if (!err.compare("ERR_NEEDMOREPARAMS")){
                    ERR_NEEDMOREPARAMS(pfdsindex, args[0]);
                }
            }
        } else
            ERR_NEEDMOREPARAMS(pfdsindex, args[0]);
    }else
        ERR_NEEDMOREPARAMS(pfdsindex, args[0]);
}


/* -------------------------------------------------------------------------- */