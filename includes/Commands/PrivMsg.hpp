# pragma once

#include "ICommand.hpp"
#include "../AClient.hpp"
#include "../Server.hpp"

class PrivMsg: public ICommand {
public:
    PrivMsg( Server& ircServ );
    ~PrivMsg( void );
    void execute( AClient *, const std::string & rawCommand );
    void clearCmd( void );
};