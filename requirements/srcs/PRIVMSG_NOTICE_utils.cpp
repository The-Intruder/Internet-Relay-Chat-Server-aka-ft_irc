/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG_NOTICE_utils.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haitkadi <hssain.aitkadir@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 15:42:35 by haitkadi          #+#    #+#             */
/*   Updated: 2023/06/05 15:42:36 by haitkadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ircserv.head.hpp"

/* -------------------------------------------------------------------------- */


std::string    is_duplicate(std::vector<std::string> receivers){
    sort(receivers.begin(), receivers.end());
    for (std::size_t i = 1; i < receivers.size(); i++) {
        if (receivers[i - 1] == receivers[i])
            return receivers[i];
    }
    return std::string();
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */

std::vector<std::vector<std::string> >    splitReceivers(std::vector<std::string> buffer){
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
    } else {
        receivers.push_back(buffer[0]);
        result.push_back(receivers);
    }
    msg.push_back(buffer[1]);
    result.push_back(msg);
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
    return splitReceivers(buffer);
}

/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  */