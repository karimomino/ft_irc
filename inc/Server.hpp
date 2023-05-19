#pragma once

#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <vector>
#include "Exceptions.hpp"
#include "Client.hpp"

#ifndef MAX_CLIENTS
# define MAX_CLIENTS 69
#endif

class Server {
    private:
        const int           _port;
        const std::string   _password;
        int                 _socketfd;
        struct sockaddr_in  _servAddr;
        std::vector<Client> _clients;

    public:
        Server( int port, std::string const password );
        ~Server( void );

        bool        checkPort( void ) const;
        void        initServer( void );
        void        handleClient( void );

        int         getPort( void ) const;
        std::string getPassword( void ) const;
};
