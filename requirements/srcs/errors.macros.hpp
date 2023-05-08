/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.macros.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abellakr <abellakr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 13:35:09 by abellakr          #+#    #+#             */
/*   Updated: 2023/05/08 23:50:29 by abellakr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_MACROS_HPP
#define ERRORS_MACROS_HPP


#define ERR_NEEDMOREPARAMS(pfdsindex, cmd) {std::string message = ":abdellah 461 " + cmd + " :Not enough parameters\n"; writemessagetoclients(pfdsindex, message);}
#define ERR_ALREADYREGISTRED(pfdsindex) {std::string message = ":abdellah 462 :You may not reregister\n";  writemessagetoclients(pfdsindex, message);}
#define ERR_NONICKNAMEGIVEN(pfdsindex) {std::string message = ":abdellah 431 ::No nickname given\n"; writemessagetoclients(pfdsindex, message);}
#define ERR_NICKNAMEINUSE(pfdsindex, nickname) {std::string message = ":abdellah 433 " + nickname + " :Nickname is already in use\n"; writemessagetoclients(pfdsindex, message);}

#endif