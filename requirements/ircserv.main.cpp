/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.main.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abellakr <abellakr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 10:54:49 by abellakr          #+#    #+#             */
/*   Updated: 2023/05/04 17:57:06 by abellakr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ircserv.head.hpp"


int main(int ac, char **av)
{
    try{
        if(ac != 3)
            throw std::invalid_argument("error: invalid argument");
        else
            Server ircserv(std::atoi(av[1]), av[2]);
        
    }
    catch(std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    // system("leaks ircserv"); // check for leaks
}


// TODO: check parising of arguments and it's erros 
// split server constractor into multi functions 
// how to store clients ip adresses if the class and store each client data in the map
// Authentification in server
// bot 
// look for if the parsing of the cmd