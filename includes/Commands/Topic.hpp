# pragma once

#include "ICommand.hpp"
#include "../AClient.hpp"
#include "../Server.hpp"

class Topic: public ICommand {
public:
    Topic();
    Topic( Server& ircServ );
    ~Topic();
    void execute( AClient *, const std::string & rawCommand );
    void clearCmd( void );
};