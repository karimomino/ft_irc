#pragma once

#include "Server.hpp"

class PreClient {
protected:
    const int   _socketFd;
    std::string _nick;
    std::string _user;
    std::string _pass;
    const std::string _ip;

public:
    PreClient( int socketFd, struct sockaddr_in* hint );

    /* Methods */

    /* Getters */
    const std::string& getNick( void ) const;
    const std::string& getPass( void ) const;
    const std::string& getUser( void ) const;
    const std::string& getIp( void ) const;
    int getSocketFd( void ) const;

    /* Setters */
    void setNick( const std::string& );
    void setPass( const std::string& );
    void setUser( const std::string& );
};

