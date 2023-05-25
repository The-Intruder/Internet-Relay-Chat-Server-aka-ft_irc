/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.responces.macros.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abellakr <abellakr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 13:19:28 by abellakr          #+#    #+#             */
/*   Updated: 2023/05/16 22:02:46 by abellakr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_RESPONCES_MACROS_HPP
#define ERRORS_RESPONCES_MACROS_HPP


#define ERR_NEEDMOREPARAMS(pfdsindex, cmd) {std::string message = ":IrcTheThreeMusketeers 461 " + cmd + " :Not enough parameters\n"; writemessagetoclients(pfdsindex, message);}
#define ERR_ALREADYREGISTRED(pfdsindex) {std::string message = ":IrcTheThreeMusketeers 462 :You may not reregister\n";  writemessagetoclients(pfdsindex, message);}
#define ERR_NONICKNAMEGIVEN(pfdsindex) {std::string message = ":IrcTheThreeMusketeers 431 ::No nickname given\n"; writemessagetoclients(pfdsindex, message);}
#define ERR_ERRONEUSNICKNAME(pfdsindex, nickname){std::string message = ":IrcTheThreeMusketeers 432 " + nickname + " :Erroneus nickname\n";writemessagetoclients(pfdsindex, message);}
#define ERR_NICKNAMEINUSE(pfdsindex, nickname) {std::string message = ":IrcTheThreeMusketeers 433 " + nickname + " :Nickname is already in use\n"; writemessagetoclients(pfdsindex, message);}
#define RPL_WELCOME(pfdsindex, nickname, username){std::string message = ":IrcTheThreeMusketeers 001 " + nickname + " !" + username + " @localhost Welcome to the Internet Relay Network\n"; writemessagetoclients(pfdsindex, message);}
#define RPL_YOURHOST(pfdsindex){std::string message = ":IrcTheThreeMusketeers 002 Your host is IrcTheThreeMusketeers, running version 1.0\n"; writemessagetoclients(pfdsindex, message);}
#define RPL_CREATED(pfdsindex, timeinfo){std::string message = ":IrcTheThreeMusketeers 003 This server was created " + timeinfo + "\n";writemessagetoclients(pfdsindex, message);}
#define ERR_CMDNOTFOUND(pfdsindex){std::string message = ":IrcTheThreeMusketeers ERROR :command not found\n";writemessagetoclients(pfdsindex, message);}
#define ERR_FLAGNOTFOUND(pfdsindex){std::string message = ":IrcTheThreeMusketeers ERROR :flag not found use -help\n";writemessagetoclients(pfdsindex, message);}

// JOIN ERRS and RPL

/*
           ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
           ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
           ERR_CHANNELISFULL               ERR_BADCHANMASK
           ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
           RPL_TOPIC
*/

#define ERR_INVALIDCHNLNAME(pfdsindex, chName){std::string message = ":IrcTheThreeMusketeers ERROR :invalid channel name: " + chName + "\n";writemessagetoclients(pfdsindex, message);}
#define ERR_INVITEONLYCHAN(fd, chName){std::string message = ":IrcTheThreeMusketeers 473 " + chName + " :Cannot join channel (+i)\n";writemessagetoclients(fd, message);}
#define ERR_BADCHANNELKEY(fd, chName){std::string message = ":IrcTheThreeMusketeers 475 " + chName + " :Cannot join channel (+k)\n";writemessagetoclients(fd, message);}
#define ERR_CHANNELISFULL(fd, chName){std::string message = ":IrcTheThreeMusketeers 471 " + chName + " :Cannot join channel (+l)\n";writemessagetoclients(fd, message);}
#define RPL_TOPIC(fd, chName, topic){std::string message = ":IrcTheThreeMusketeers 332 " + chName + " : " + topic + "\n"; writemessagetoclients(fd, message);}
#define RPL_NOTOPIC(fd, chName){std::string message = ":IrcTheThreeMusketeers 331 " + chName + " : No topic is set\n"; writemessagetoclients(fd, message);}

#define ERR_NORECIPIENT(fd){std::string message = ":IrcTheThreeMusketeers 411 Hssain :No recipient given (PRIVMSG)\n";writemessagetoclients(fd, message);}
#define ERR_NOTEXTTOSEND(fd){std::string message = ":IrcTheThreeMusketeers 412 Hssain :No text to send\n";writemessagetoclients(fd, message);}


#endif
