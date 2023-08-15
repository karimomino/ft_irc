#pragma once

#include <iostream>

class Server;
class AClient;

class ICommand {
protected:
    Server&             _ircServ;
    AClient&             _client;
    const std::string   _rawCommand;

public:
    ICommand( Server & ircServ , AClient &, const std::string & rawCommand );

    /* Methods */
    virtual void execute( void ) = 0;
    virtual void clearCmd( void ) = 0;

    class cmdError : std::exception {
    public:
        const char* what( void ) const throw();
    };
};
