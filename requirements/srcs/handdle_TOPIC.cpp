

#include "../ircserv.head.hpp"

/* -------------------------------------------------------------------------- */

std::vector<std::string>    TOPIC_parseArgs(std::string args){
    std::vector<std::string>  result;
    std::istringstream iss(args);
    std::vector<std::string> tokens;
    std::string token;

    while (iss >> token) {
        tokens.push_back(token);
    }
    if (tokens.size() < 1)
        throw std::runtime_error("ERR_NEEDMOREPARAMS");
    std::string channel = tokens[0];
    std::string topic;
    if (tokens.size() > 1) {
        std::ostringstream oss;
        for (size_t i = 1; i < tokens.size(); ++i) {
            oss << tokens[i];
            if (i < tokens.size() - 1) {
                oss << " ";
            }
        }
        topic = oss.str();
    }
    result.push_back(channel);
    stringTrim(topic, ": ");
    if (!topic.empty())
        result.push_back(topic);
    return result;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void    Server::TOPIC_trigger(std::size_t pfdsindex, std::vector<std::string> args){
    std::map<std::string, IRCChannel>::iterator channel = this->ChannelsMap.find(args[0]);
    if(channel != this->ChannelsMap.end()){
        if(channel->second.isClientOnChan(this->pfds[pfdsindex].fd)){
            if (args.size() == 2){
                channel->second.changeTopic(this->pfds[pfdsindex].fd, args[1]);
            } else
                channel->second.sendTopicToClient(this->pfds[pfdsindex].fd);
        } else
            ERR_NOTONCHANNEL(this->pfds[pfdsindex].fd, args[0]);
    }else
        ERR_NOSUCHCHANNEL(pfdsindex, args[0]);
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void    Server::TOPIC_Handle(size_t pfdsindex, std::vector<std::string> args){
    if (args.size() > 1) {
        stringTrim(args[1], " :\r\t\n");
        if (!args[1].empty()){
            try{
                std::vector<std::string> topicArgs = TOPIC_parseArgs(args[1]);
                if (topicArgs.size() > 0)
                    this->TOPIC_trigger(pfdsindex, topicArgs);
            }catch(const std::exception& e) {
                std::string err = e.what();
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