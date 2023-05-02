/* -------------------------------------------------------------------------- */
/*  File: ircserv.head.cpp                                                    */
/*  Brief: IRC server header file                                             */
/*  Authors:                                                                  */
/*   - Mohamed Amine Naimi                                                    */
/*   - Hssain Ait Kadir                                                       */
/*   - Abdellah Bellakrim                                                     */
/*  Version: 1.0                                                              */
/*  License: GPL-3.0                                                          */
/* -------------------------------------------------------------------------- */

#include "ircserv.head.hpp"

/* -------------------------------------------------------------------------- */

static bool port_is_valid(const std::string &port)
{
    for (int i = 0; port[i] != '\0'; i++)
        if (!std::isdigit(port[i]))
            return false;
    return true;
}

/* -------------------------------------------------------------------------- */

static int check_for_errors(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Error: invalid arguments count" << std::endl;
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return EXIT_FAILURE;
    }
    else if (port_is_valid(argv[1]) == false)
    {
        std::cerr << "Error: invalid port number" << std::endl;
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return EXIT_FAILURE;
    }
    else if (std::string(argv[2]).size() < 8)
    {
        std::cerr << "Error: password must be at least 8 characters long" << std::endl;
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/* -------------------------------------------------------------------------- */

int main(int argc, char **argv)
{
    if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0))
    {
        std::cout << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return EXIT_SUCCESS;
    }
    else if (check_for_errors(argc, argv) == EXIT_FAILURE)
        return EXIT_FAILURE;

    /* ---------------------------- Parse Data ------------------------------ */

    IRCServer ircserv(argv[2], std::stoi(argv[1]));
    std::cout << "Server password: " << ircserv.server_pass << std::endl;
    std::cout << "Server port: " << ircserv.server_port << std::endl;

    /* --------------------------- Create Socket ---------------------------- */

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        std::cerr << "Failed to create socket" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Socket created successfully" << std::endl;

    /* ---------------------------- Bind Socket ----------------------------- */

    sockaddr_in serverAddress;
    std::memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(ircserv.server_port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, reinterpret_cast<struct sockaddr *>(&serverAddress), sizeof(serverAddress)) == -1)
    {
        std::cerr << "Failed to bind socket" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Socket bound successfully" << std::endl;

    /* ------------------------- Listen for connect. ------------------------ */

    if (listen(sockfd, 5) < 0)
    {
        std::cerr << "Failed to listen on socket" << std::endl;
        return 1;
    }
    std::cout << "Listening for incoming connections..." << std::endl;

    /* --------------------------- Accept connect. -------------------------- */

    while (true)
    {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);

        int clientSockfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSockfd < 0)
        {
            std::cerr << "Failed to accept connection" << std::endl;
            continue;
        }
        std::cout << "Accepted a connection from: " << inet_ntoa(clientAddr.sin_addr) << std::endl;
        std::cout << "Client socket: " << clientSockfd << std::endl;
        std::cout << "Waiting for client to send data..." << std::endl;
        std::cout << "-----------------------------------------------" << std::endl;

        /* --------------------- Handle Client request ---------------------- */

        while (true)
        {
            char buffer[1024];
            int bytesRead = recv(clientSockfd, buffer, sizeof(buffer), 0);

            if (bytesRead > 0)
            {
                buffer[bytesRead] = '\0';
                printf("Received message: %s\n", buffer);
            }
            else if (bytesRead == 0)
            {
                printf("Client disconnected.\n");
                break;
            }
            else
            {
                perror("recv");
                break;
            }
        }
        close(clientSockfd);
    }

    /* ---------------------------------------------------------------------- */

    return EXIT_SUCCESS;
}
