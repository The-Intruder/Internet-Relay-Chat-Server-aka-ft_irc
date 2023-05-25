#include "../ircserv.head.hpp"

/* -------------------------------------------------------------------------- */

void    stringTrim(std::string &str, const char *to_trim){
    str.erase(0, str.find_first_not_of(to_trim));                                                                                               
    str.erase(str.find_last_not_of(to_trim)+1);
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

bool checkSyntax(int fd, std::string &args){
    // std::string cmd = "PRIVMSG";
    if (args.empty()){
        ERR_NORECIPIENT(fd);
        return false;
    }
    // else if (args.find(":") == std::string::npos){
    //     ERR_NOTEXTTOSEND(fd);
    //     return false;
    // }
    return true;
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

// haitkadi,one,two :stop making excuses

std::vector<std::string>    parseReceivers(int fd, std::string args){
    (void)fd;
    std::string receivers = args.substr(0, args.find_first_of(" :"));
    std::cout<< "this one: [" <<receivers << "]"<<std::endl;
    // if (receivers.empty()){
    //     ERR_NORECIPIENT(fd);
    // }
    // std::cout << receivers << std::endl;
    return std::vector<std::string>();
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

void    Server::HandlePRIVMSG(size_t pfdsindex, std::string args){
        std::cout << "________________(" << args << ")___________" << std::endl;
    stringTrim(args, " \r\t\n");
    if (checkSyntax(this->pfds[pfdsindex].fd, args)){
        std::cout << "PRIVMSG syntax ok: __(" << args << ")__" << std::endl;
        std::vector<std::string> receivers = parseReceivers(this->pfds[pfdsindex].fd, args);
    } else {
        std::cout << "PRIVMSG syntax Not ok: **(" << args << ")**" << std::endl;
    }
}

/* -------------------------------------------------------------------------- */
