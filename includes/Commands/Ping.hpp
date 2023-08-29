# pragma once

#include "ICommand.hpp"
#include "../AClient.hpp"
#include "../Server.hpp"

class Ping: public ICommand {
public:
    Ping();
    Ping( Server& ircServ );
    ~Ping();
    void execute( AClient *, const std::string & rawCommand );
    void clearCmd( void );
};
