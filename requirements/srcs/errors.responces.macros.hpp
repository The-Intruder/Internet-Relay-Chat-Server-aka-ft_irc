/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.responces.macros.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abellakr <abellakr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 13:19:28 by abellakr          #+#    #+#             */
/*   Updated: 2023/05/10 18:49:31 by abellakr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_RESPONCES_MACROS_HPP
#define ERRORS_RESPONCES_MACROS_HPP


#define ERR_NEEDMOREPARAMS(pfdsindex, cmd) {std::string message = ":IrcTheThreeMusketeers 461 " + cmd + " :Not enough parameters\n"; writemessagetoclients(pfdsindex, message);}
#define ERR_ALREADYREGISTRED(pfdsindex) {std::string message = ":IrcTheThreeMusketeers 462 :You may not reregister\n";  writemessagetoclients(pfdsindex, message);}
#define ERR_NONICKNAMEGIVEN(pfdsindex) {std::string message = ":IrcTheThreeMusketeers 431 ::No nickname given\n"; writemessagetoclients(pfdsindex, message);}
#define ERR_NICKNAMEINUSE(pfdsindex, nickname) {std::string message = ":IrcTheThreeMusketeers 433 " + nickname + " :Nickname is already in use\n"; writemessagetoclients(pfdsindex, message);}
#define RPL_WELCOME(pfdsindex, nickname, username){std::string message = ":IrcTheThreeMusketeers 001 " + nickname + " !" + username + " @localhost Welcome to the Internet Relay Network\n"; writemessagetoclients(pfdsindex, message);}
#define RPL_YOURHOST(pfdsindex){std::string message = ":IrcTheThreeMusketeers 002 Your host is IrcTheThreeMusketeers, running version 1.0\n"; writemessagetoclients(pfdsindex, message);}
#define RPL_CREATED(pfdsindex, timeinfo){std::string message = ":IrcTheThreeMusketeers 003 This server was created " + timeinfo + "\n";writemessagetoclients(pfdsindex, message);}
#define ERR_CMDNOTFOUND(pfdsindex){std::string message = ":IrcTheThreeMusketeers 461 command not found\n";writemessagetoclients(pfdsindex, message);}
#define ERR_ERRONEUSNICKNAME(pfdsindex, nickname){std::string message = ":IrcTheThreeMusketeers 432 " + nickname + " :Erroneus nickname\n";writemessagetoclients(pfdsindex, message);}


#endif