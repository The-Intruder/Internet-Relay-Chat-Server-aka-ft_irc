/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.main.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abellakr <abellakr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 10:54:49 by abellakr          #+#    #+#             */
/*   Updated: 2023/05/07 14:23:54 by abellakr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ircserv.head.hpp"


static bool port_is_valid(const std::string &port)
{
    for (int i = 0; port[i] != '\0'; i++)
        if (!std::isdigit(port[i]))
            return false;
    return true;
}

static int check_for_errors(int ac, char **av)
{
    if (ac != 3)
    {
        std::cerr << "Error: invalid arguments count" << std::endl;
        std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
        return EXIT_FAILURE;
    }
    else if (port_is_valid(av[1]) == false)
    {
        std::cerr << "Error: invalid port number" << std::endl;
        std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
        return EXIT_FAILURE;
    }
    else if (std::string(av[2]).size() < 8)
    {
        std::cerr << "Error: password must be at least 8 characters long" << std::endl;
        std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int main(int ac, char **av)
{
    try{
        if (ac == 2 && (strcmp(av[1], "-h") == 0 || strcmp(av[1], "--help") == 0))
        {
            std::cout << "Usage: " << av[0] << " <port> <password>" << std::endl;
            return EXIT_SUCCESS;
        }
        else if (check_for_errors(ac, av) == EXIT_FAILURE)
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


// TODO:
// look for if the parsing of the cmd
// add list for commands to parse data
// bot  : curl parrot.live system("cal");
// check ftp 
//check limechat

//TODO:
// Authentification in server : PASS , NICK , USER
// add function to handle errors as RFC


//TODO:: drari
// list of commands to be executed form other contributors
// synchronized the error handling function for other contributors
