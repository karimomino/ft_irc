#pragma once

#include <iostream>
#include "Server.hpp"

class ICommand {
protected:
    const Client*       _client;
    const std::string   _msg;

public:
    ICommand( void );
    ICommand( const Client &, const std::string & msg );

    /* Methods */
    virtual void execute( void ) = 0;
    virtual void clearCmd( void ) = 0;

    class cmdError : std::exception {
    public:
        const char* what( void ) const throw();
    };
};
