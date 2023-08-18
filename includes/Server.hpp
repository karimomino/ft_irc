#pragma once

#include <iostream>
#include <map>
#include <queue>
#include <poll.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "utils.hpp"
#include "Client.hpp"
#include "PreClient.hpp"
#include "Commands/ICommand.hpp"
#include "Commands/Join.hpp"
#include "Commands/Kick.hpp"
#include "Commands/Mode.hpp"
#include "Commands/Topic.hpp"
#include "Commands/Invite.hpp"

#ifndef MAX_CLIENTS
# define MAX_CLIENTS 42
#endif

class Channel;

class Server {
private:
    const int                       _port;
    const std::string               _pass;
    int                             _socketFd;
    std::string                     _ip;
    size_t                          _connectionCount;
    std::vector<pollfd>             _pollFds;
    struct sockaddr_in              _hint;
    std::queue<PreClient*>          _preClients;
    std::map<std::string, Client*>  _clients;
    std::map<std::string, Channel*> _channels;
    std::map<const std::string, ICommand*> _cmds;

    /* Methods */
    void _initCmds( void );
    void _addClient( const std::string& name );
    void _addChannel( const std::string& name );
    void _removeClient( const std::string& name );
    void _removeChannel( const std::string& name );

public:
    Server( int port, const std::string& pass );
    ~Server( void );

    /* Methods */
    void init( void );
    void run( void );
    void exit( void );

    /* Getters */
    const std::string& getPass( void ) const;
    const std::string& getIp( void ) const;
    const struct sockaddr_in& getHint( void ) const;

    /* Setters */

    class ServerError : public std::runtime_error {
    public:
        ServerError( const char * msg );
    };

    friend class Kick;
};

