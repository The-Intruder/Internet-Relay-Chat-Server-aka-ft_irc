/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abellakr <abellakr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 14:38:54 by abellakr          #+#    #+#             */
/*   Updated: 2023/06/05 19:02:28 by abellakr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ircserv.head.hpp"

void Server::quit(size_t pfdsindex)
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
}