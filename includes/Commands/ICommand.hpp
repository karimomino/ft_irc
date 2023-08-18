#pragma once

#include <iostream>

class Server;
class AClient;

class ICommand {
protected:
    Server&             _ircServ;

public:
    ICommand( Server & ircServ );
    virtual ~ICommand();

    /* Methods */
    virtual void execute( AClient *, const std::string & rawCommand ) = 0;
    virtual void clearCmd( void ) = 0;

    class cmdError : std::exception {
    public:
        const char* what( void ) const throw();
    };
};
