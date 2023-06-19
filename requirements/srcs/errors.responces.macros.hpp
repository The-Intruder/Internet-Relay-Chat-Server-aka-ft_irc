/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.responces.macros.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abellakr <abellakr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 13:19:28 by abellakr          #+#    #+#             */
/*   Updated: 2023/06/19 23:18:17 by abellakr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_RESPONCES_MACROS_HPP
#define ERRORS_RESPONCES_MACROS_HPP

#define ERR_NEEDMOREPARAMS(pfdsindex, cmd) {std::string message = ":IrcTheThreeMusketeers 461 " + cmd + " :Not enough parameters\r\n"; writeMessageToClient(pfdsindex, message);}
#define ERR_ALREADYREGISTRED(pfdsindex) {std::string message = ":IrcTheThreeMusketeers 462 :You may not reregister\r\n";  writeMessageToClient(pfdsindex, message);}
#define ERR_NONICKNAMEGIVEN(pfdsindex) {std::string message = ":IrcTheThreeMusketeers 431 ::No nickname given\r\n"; writeMessageToClient(pfdsindex, message);}
#define ERR_ERRONEUSNICKNAME(pfdsindex, nickname){std::string message = ":IrcTheThreeMusketeers 432 " + nickname + " :Erroneus nickname\r\n";writeMessageToClient(pfdsindex, message);}
#define ERR_NICKNAMEINUSE(pfdsindex, nickname) {std::string message = ":IrcTheThreeMusketeers 433 " + nickname + " :Nickname is already in use\r\n"; writeMessageToClient(pfdsindex, message);}
#define RPL_WELCOME(pfdsindex, nickname, username, Hostname){std::string message = ":IrcTheThreeMusketeers 001 " + nickname + " !" + username + " @" + Hostname + " Welcome to the Internet Relay Network\r\n"; writeMessageToClient(pfdsindex, message);}
#define RPL_YOURHOST(pfdsindex){std::string message = ":IrcTheThreeMusketeers 002 Your host is IrcTheThreeMusketeers, running version 1.0\r\n"; writeMessageToClient(pfdsindex, message);}
#define RPL_CREATED(pfdsindex, timeinfo){std::string message = ":IrcTheThreeMusketeers 003 This server was created " + timeinfo + "\r\n";writeMessageToClient(pfdsindex, message);}
#define ERR_BADARGUMENT(pfdsindex){std::string message = ":IrcTheThreeMusketeers ERROR :bad argument\r\n";writeMessageToClient(pfdsindex, message);}
#define  RPL_MYINFO(pfdsindex){std::string message = ":IrcTheThreeMusket 004 1.0 - - \r\n"; writeMessageToClient(pfdsindex, message);}
#define RPL_MOTD(pfdsindex){std::string message = ":IrcTheThreeMusket 372 - - \r\n";writeMessageToClient(pfdsindex, message);}
#define RPL_ENDOFMOTD(pfdsindex){std::string message = ":IrcTheThree 376 this server is created by Mohammed Amine Naimi, Abdellah Bellakrim ,Hssain Aitkadir.\r\n"; writeMessageToClient(pfdsindex, message);}
#define ERR_CMDNOTFOUND(pfdsindex){std::string message = ":IrcTheThreeMusketeers ERROR :command not found\r\n";writeMessageToClient(pfdsindex, message);}
#define ERR_FLAGNOTFOUND(pfdsindex){std::string message = ":IrcTheThreeMusketeers ERROR :flag not found use -help\r\n";writeMessageToClient(pfdsindex, message);}
// JOIN ERRS and RPL

/*
    ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
    ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
    ERR_CHANNELISFULL               ERR_BADCHANMASK
    ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
    RPL_TOPIC
*/

#define ERR_INVALIDCHNLNAME(pfdsindex, chName){std::string message = ":IrcTheThreeMusketeers ERROR :invalid channel name: " + chName + "\n";writeMessageToClient(pfdsindex, message);}
#define ERR_INVITEONLYCHAN(fd, chName){std::string message = ":IrcTheThreeMusketeers 473 " + chName + " :Cannot join channel (+i)\n";writeMessageToClient_fd(fd, message);}
#define ERR_BADCHANNELKEY(fd, chName){std::string message = ":IrcTheThreeMusketeers 475 " + chName + " :Cannot join channel (+k)\n";writeMessageToClient_fd(fd, message);}
#define ERR_CHANNELISFULL(fd, chName){std::string message = ":IrcTheThreeMusketeers 471 " + chName + " :Cannot join channel (+l)\n";writeMessageToClient_fd(fd, message);}
#define RPL_NAMREPLY(fd, nickName,  chName, members){std::string message = ":IrcTheThreeMusketeers 353 " + nickName + " = " + chName + " :" + members + "\n"; writeMessageToClient_fd(fd, message);}
#define RPL_ENDOFNAMES(fd, nickName,  chName){std::string message = ":IrcTheThreeMusketeers 366 " + nickName + " " + chName + " :End of /NAMES list\n"; writeMessageToClient_fd(fd, message);}



/*
   Numeric Replies:

           ERR_NORECIPIENT                 ERR_NOTEXTTOSEND
           ERR_CANNOTSENDTOCHAN            ERR_NOTOPLEVEL
           ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
           ERR_NOSUCHNICK
           RPL_AWAY
*/


#define ERR_NORECIPIENT(pfdsindex, nickName){std::string message = ":IrcTheThreeMusketeers 411 " + nickName + " :No recipient given (PRIVMSG)\n";writeMessageToClient(pfdsindex, message);}
#define ERR_NOTEXTTOSEND(pfdsindex, nickName){std::string message = ":IrcTheThreeMusketeers 412 " + nickName + " :No text to send\n";writeMessageToClient(pfdsindex, message);}
#define ERR_TOOMANYTARGETS(pfdsindex, err){std::string message = ":IrcTheThreeMusketeers 407 " + err + " :Duplicate recipients. No message delivered\n";writeMessageToClient(pfdsindex, message);}
#define ERR_NOSUCHNICK(pfdsindex, receiver){std::string message = ":IrcTheThreeMusketeers 401 " + receiver + " :No such nick/channel\n";writeMessageToClient(pfdsindex, message);}
#define ERR_CANNOTSENDTOCHAN(fd, chName){std::string message = ":IrcTheThreeMusketeers 404 " + chName + " :Cannot send to channel\n";writeMessageToClient_fd(fd, message);}



/*
           ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
           ERR_NOTONCHANNEL
*/

#define ERR_NOSUCHCHANNEL(pfdsindex, chName){std::string message = ":IrcTheThreeMusketeers 403 " + chName + " :No such channel\n";writeMessageToClient(pfdsindex, message);}
#define ERR_NOTONCHANNEL(fd, chName){std::string message = ":IrcTheThreeMusketeers 442 " + chName + " :You're not on that channel\n";writeMessageToClient_fd(fd, message);}



/*
           ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
           ERR_CHANOPRIVSNEEDED            ERR_NOTONCHANNEL
*/


#define ERR_CHANOPRIVSNEEDED(fd, chName){std::string message = ":IrcTheThreeMusketeers 482 " + chName + " You're not channel operator\n";writeMessageToClient_fd(fd, message);}


/*
           ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
           RPL_NOTOPIC                     RPL_TOPIC
           ERR_CHANOPRIVSNEEDED
*/
#define RPL_TOPIC(fd, chName, topic){std::string message = ":IrcTheThreeMusketeers 332 " + chName + " :" + topic + "\n"; writeMessageToClient_fd(fd, message);}
#define RPL_NOTOPIC(fd, chName){std::string message = ":IrcTheThreeMusketeers 331 " + chName + " :No topic is set\n"; writeMessageToClient_fd(fd, message);}

/* ------------------------------ MODE_ERRORS ------------------------------- */
# define ERR_KEYSET(pfdsindex, chName) {std::string message = ":IrcTheThreeMusketeers 467 " + chName + " :Channel key already set\n"; writeMessageToClient(pfdsindex, message);}
# define ERR_USERNOTINCHANNEL(pfdsindex, nick, chName) {std::string message = ":IrcTheThreeMusketeers 441 " + nick + " " + chName + " :They aren't on that channel\n"; writeMessageToClient(pfdsindex, message);}
# define ERR_UNKNOWNMODE(pfdsindex, theChar, chName) {std::string message = ":IrcTheThreeMusketeers 472 " + theChar + " :is unknown mode char to me for " + chName + "\n"; writeMessageToClient(pfdsindex, message);}

# define RPL_CHANNELMODEIS(pfdsindex, chName, mode, modeParams) {std::string message = ":IrcTheThreeMusketeers 324 " + chName + " " + mode + " " + modeParams + "\n"; writeMessageToClient(pfdsindex, message);}

# define ERR_NOCHANMODES(pfdsindex, chName) {std::string message = ":IrcTheThreeMusketeers 477 " + chName + " :Channel doesn't support modes\n"; writeMessageToClient(pfdsindex, message);}

/*
# define RPL_BANLIST(pfdsindex, chName, banMask) {std::string message = ":IrcTheThreeMusketeers 367 " + chName + " " + banMask + "\n"; writeMessageToClient(pfdsindex, message);}
# define RPL_ENDOFBANLIST(pfdsindex, chName) {std::string message = ":IrcTheThreeMusketeers 368 " + chName + " :End of channel ban list\n"; writeMessageToClient(pfdsindex, message);}
# define RPL_EXCEPTLIST(pfdsindex, chName, exceptionMask) {std::string message = ":IrcTheThreeMusketeers 348 " +  chName + " " + exceptionmask + "\n"; writeMessageToClient(pfdsindex, message);}
# define RPL_ENDOFEXCEPTLIST(pfdsindex, chName) {std::string message = ":IrcTheThreeMusketeers 349 " + chName + " :End of channel exception list\n"; writeMessageToClient(pfdsindex, message);}
# define RPL_INVITELIST(pfdsindex, chName, inviteMask) {std::string message = ":IrcTheThreeMusketeers 461 " + chName + " " + inviteMask + "\n"; writeMessageToClient(pfdsindex, message);}
# define RPL_ENDOFINVITELIST(pfdsindex, chName) {std::string message = ":IrcTheThreeMusketeers 347 " + chName + " :End of channel invite list\n"; writeMessageToClient(pfdsindex, message);}
# define RPL_UNIQOPIS(pfdsindex, chName, nick) {std::string message = ":IrcTheThreeMusketeers 325 " + chName + " " + nick + "\n"; writeMessageToClient(pfdsindex, message);}
*/

# define ERR_USERONCHANNEL(pfdsindex, nickname, chName) {std::string message = ":IrcTheThreeMusketeers 443 " + nickname + " " + chName + ":is already on channel\n"; writeMessageToClient(pfdsindex, message);}
# define RPL_INVITING(pfdsindex, nickname, chName) {std::string message = ":IrcTheThreeMusketeers 341 " + chName + " " + nickname + "\n"; writeMessageToClient(pfdsindex, message);}

#endif




/// 0004, 372 , 376