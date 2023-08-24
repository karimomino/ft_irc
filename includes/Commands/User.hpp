# pragma once

#include "ICommand.hpp"
#include "../AClient.hpp"
#include "../Server.hpp"

class User: public ICommand {
public:
    User( Server& ircServ );
    ~User( void );
    void execute( AClient *, const std::string & rawCommand );
    void clearCmd( void );
};
