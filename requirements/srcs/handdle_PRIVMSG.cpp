#include "../ircserv.head.hpp"

/* -------------------------------------------------------------------------- */

void    stringTrim(std::string &str, const char *to_trim){
    str.erase(0, str.find_first_not_of(to_trim));                                                                                               
    str.erase(str.find_last_not_of(to_trim)+1);
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

std::string    is_duplicate(std::vector<std::string> receivers){
    sort(receivers.begin(), receivers.end());
    for (std::size_t i = 1; i < receivers.size(); i++) {
        if (receivers[i - 1] == receivers[i])
            return receivers[i];
    }
    return std::string();
}

std::vector<std::vector<std::string> >    finalData(std::vector<std::string> buffer){
    std::vector<std::vector<std::string> >  result;
    std::vector<std::string>                receivers;
    std::vector<std::string>                msg;

    if(buffer.size() == 1)
        throw std::runtime_error("ERR_NOTEXTTOSEND");
    if (buffer[0].find(",")  != std::string::npos){
        std::string token;
        std::stringstream ss(buffer[0]);
        while(std::getline(ss, token, ',')){
            stringTrim(token, " \r\t\n");
            if (!token.empty())
                receivers.push_back(token);
            token.clear();
        }
        if(receivers.empty()){
            throw std::runtime_error("ERR_NORECIPIENT");
        }
        std::string duplicatedToken = is_duplicate(receivers);
        if (!duplicatedToken.empty())
            throw std::runtime_error(duplicatedToken);
        result.push_back(receivers);
        msg.push_back(buffer[1]);
        result.push_back(msg);
    } else {
        receivers.push_back(buffer[0]);
        msg.push_back(buffer[1]);
        result.push_back(receivers);
        result.push_back(msg);
    }
    return result;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

std::vector<std::vector<std::string> >    parseArgs(std::string args){
    std::stringstream ss(args);
    std::string token;
    std::vector<std::string> buffer;

    if (args.find(":") != std::string::npos){
        std::string::size_type colonPos = args.find(':');
        if (colonPos != std::string::npos) {
            std::string token1 = args.substr(0, colonPos);
            std::string token2 = args.substr(colonPos + 1);
            stringTrim(token1, " \r\t\n");
            stringTrim(token2, " \r\t\n");
            if (!token1.empty())
                buffer.push_back(token1);
            if (!token2.empty())
                buffer.push_back(token2);
        }
    }else {
        while(std::getline(ss, token, ' ')){
            stringTrim(token, " \r\t\n");
            if (!token.empty())
                buffer.push_back(token);
            token.clear();
            if (buffer.size() == 2)
                break;
        }
    }
    return finalData(buffer);
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */


void    Server::messagToClient(std::size_t pfdsindex, std::string &clientNick, std::string &msg){
    Client &sender =  this->ClientsMap.find(this->pfds[pfdsindex].fd)->second;
    std::string fullMsg = ":" + sender.getNICKNAME() \
    + "!" + sender.getUSERNAME() + "@localhost.ip PRIVMSG " \
    + clientNick + " :" + msg + "\n";

    std::map<int, Client>::iterator it = this->ClientsMap.begin();
    for(; it != this->ClientsMap.end();it++){
        if (it->second.getNICKNAME() == clientNick)
            break;
    }
    if (it != this->ClientsMap.end()){
        writeMessageToClient(it->first, fullMsg);
    } else {
        ERR_NOSUCHNICK(pfdsindex, sender.getNICKNAME(), clientNick);
    }
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void    Server::handdleMSG(std::size_t pfdsindex, std::vector<std::vector<std::string> > args){
    std::vector<std::string>    receivers = args[0];
    std::string    msg = args[1][0];
    for (std::vector<std::string>::iterator it = receivers.begin(); it != receivers.end();it++){
        if (it->at(0) == '#'){
            std::map<std::string, IRCChannel>::iterator chanl = this->ChannelsMap.find(*it);
            if (chanl != this->ChannelsMap.end())
                chanl->second.messagToChannel(this->pfds[pfdsindex].fd, msg);
            else
                ERR_NOSUCHNICK(pfdsindex, this->ClientsMap.find(this->pfds[pfdsindex].fd)->second.getNICKNAME(), *it);
        } else {
            this->messagToClient(pfdsindex, *it, msg);
        }
    }
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void    Server::HandlePRIVMSG(size_t pfdsindex, std::vector<std::string> args){
    std::string nickName = this->ClientsMap.find(pfds[pfdsindex].fd)->second.getNICKNAME();
    if (args.size() > 1) {
        stringTrim(args[1], " \r\t\n");
        if (!args[1].empty()){
            try
            {
                std::vector<std::vector<std::string> > cleanArgs = parseArgs(args[1]);
                this->handdleMSG(pfdsindex, cleanArgs);
            }
            catch(const std::exception& e)
            {
                std::string err = e.what();
                if (!err.compare("ERR_NORECIPIENT")){
                    ERR_NORECIPIENT(pfdsindex, nickName);
                } else if (!err.compare("ERR_NOTEXTTOSEND")){
                    ERR_NOTEXTTOSEND(pfdsindex, nickName)
                } else
                    ERR_TOOMANYTARGETS(pfdsindex, err);
            }
        } else
            ERR_NORECIPIENT(pfdsindex, nickName);
    }else
        ERR_NORECIPIENT(pfdsindex, nickName);
}

/* -------------------------------------------------------------------------- */
