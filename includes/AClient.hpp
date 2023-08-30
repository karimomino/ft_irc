#pragma once

// #include "Server.hpp"
#include <iostream>
#include <queue>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class AClient {
protected:
    int   _socketFd;
    std::string _nick;
    std::string _user;
    std::string _pass;
    bool        _purge;
    std::string _ip;
    std::queue<std::string> _msgs;

public:
    std::string partialCmd;
    AClient( int socketFd, struct sockaddr_in* hint );
    AClient( const AClient& original );
    virtual ~AClient() = 0;
    /* Methods */
    void addMsg( const std::string& );
    void sendMSg( void );
    /* Getters */
    const std::string& getNick( void ) const;
    const std::string& getPass( void ) const;
    const std::string& getUser( void ) const;
    const std::string& getIp( void ) const;
    const std::string  getOrigin( void ) const;
    const int& getSocketFd( void ) const;
    const bool& getPurge( void ) const;
    int getQueueSize( void ) const;
    const std::string& getFirstMsg( void) const;

    /* Setters */
    void setNick( const std::string& );
    void setPass( const std::string& );
    void setUser( const std::string& );
    void setPurge( const bool& purge );
};

