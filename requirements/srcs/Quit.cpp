/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abellakr <abellakr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 14:38:54 by abellakr          #+#    #+#             */
/*   Updated: 2023/06/05 14:39:05 by abellakr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ircserv.head.hpp"

bool Server::quit(size_t pfdsindex)
{
    if(MS.size() != 1)
        return false; 
    else
    {
        // remove client from map
        std::cout << "client disconnected sockfd: " << pfds[pfdsindex].fd << std::endl; 
        std::map<int,Client>::iterator itmap = ClientsMap.find(pfds[pfdsindex].fd);
        ClientsMap.erase(itmap);
        // remove client from pollfd vector
        std::vector<pollfd>::iterator itvec = pfds.begin();
        itvec += pfdsindex;
        close(pfds[pfdsindex].fd);
        pfds.erase(itvec);
        // remove client from channel
        return true;
    }
    return false;
}