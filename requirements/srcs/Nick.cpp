/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abellakr <abellakr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 22:24:11 by abellakr          #+#    #+#             */
/*   Updated: 2023/06/05 14:39:02 by abellakr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ircserv.head.hpp"

void Server::nick(size_t pfdsindex)
{
    if(MS.size() != 2)
    {
        ERR_NEEDMOREPARAMS(pfdsindex, MS[0]);
        return;   
    }
    else
    {
        if((MS[1][0] >='a' && MS[1][0] <= 'z') || (MS[1][0] >= 'A' && MS[1][0] <= 'Z'))
        {

            std::map<int,Client>::iterator tmp = ClientsMap.find(pfds[pfdsindex].fd);
            Client& client = tmp->second;
            //------
            std::map<int, Client>::iterator it = ClientsMap.begin();
            while(it != ClientsMap.end())
            {
                if(it->second.getNICKNAME() == MS[1])
                    break;
                it++;   
            }
            if(it != ClientsMap.end())
            {
                ERR_NICKNAMEINUSE(pfdsindex, MS[1]);
                return;
            }
            else if(it == ClientsMap.end())
                client.setNICKNAME(MS[1]);
        }
        else
        {
            ERR_ERRONEUSNICKNAME(pfdsindex, MS[1]);
        }  
    }
}
