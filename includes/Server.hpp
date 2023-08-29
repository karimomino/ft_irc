#pragma once

#include <iostream>
#include <map>
#include <queue>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sstream>
#include <cstring>
#include <vector>

#include "AClient.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "PreClient.hpp"
#include "Commands/ICommand.hpp"
#include "Commands/Join.hpp"
#include "Commands/Kick.hpp"
#include "Commands/Mode.hpp"
#include "Commands/Topic.hpp"
#include "Commands/Invite.hpp"
#include "Commands/Pass.hpp"
#include "Commands/User.hpp"
#include "Commands/Nick.hpp"
#include "Commands/PrivMsg.hpp"
#include "utils.hpp"
#include "colors.hpp"
#include "replies.hpp"

#ifndef MAX_CLIENTS
# define MAX_CLIENTS 42
#endif

class Channel;

class Server {
private:
    bool                            _serverEnd;
    const int                       _port;
    const std::string               _pass;
    int                             _socketFd;
    std::string                     _ip;
    size_t                          _connectionCount;
    std::vector<pollfd>             _pollFds;
    struct sockaddr_in              _hint;
    // std::deque<AClient*>            _preClients;
    std::map<int, AClient*>         _clients;
    std::map<const std::string, Channel*> _channels;
    std::map<const std::string, ICommand*> _cmds;

    /* Methods */
    void _initCmds( void );
    void _addClient( const AClient* client );
    void _addChannel( const std::string& name , const std::string& topic );
    void _removeClient( const std::string& name );
    void _removeChannel( const std::string& name );

    void _handlePreClientReg (void);
    void _handleClientSend(const int& socket);
    void _handleClientRecv(const int& socket);

    AClient* _findClientByNick( const std::string& nick  ) const;

public:
    Server( int port, const std::string& pass );
    ~Server( void );

    /* Methods */
    void init( void );
    void run( void );
    void exit( void );

    /* Getters */
    const int& getSockFd(void) const;
    const std::string& getPass( void ) const;
    const std::string& getIp( void ) const;
    const struct sockaddr_in& getHint( void ) const;

    /* Setters */

    class ServerError : public std::runtime_error {
    public:
        ServerError( const char * msg );
    };

    friend class Kick;
    friend class Pass;
    friend class User;
    friend class Nick;
    friend class Join;
    friend class PrivMsg;
    friend class Mode;
    friend void execCommand( Server& ircServ , std::string clientMsg , AClient* cli );
};

