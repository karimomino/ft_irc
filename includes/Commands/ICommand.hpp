#pragma once

#include <iostream>

class Server;
class Client;

class ICommand {
protected:
    Server&     _serv;
    Client*     _client;
    std::string _raw;

public:
    ICommand( Server& serv );

    /* Methods */
    virtual void execute( Client* client, const std::string& command ) = 0;
    virtual void clearCmd( void ) = 0;

    class cmdError : std::exception {
    public:
        const char* what( void ) const throw();
    };
};
