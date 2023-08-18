# pragma once

#include "ICommand.hpp"
#include "../AClient.hpp"
#include "../Server.hpp"

class Nick: public ICommand {
public:
    Nick( Server& ircServ );
    ~Nick( void );
    void execute( AClient *, const std::string & rawCommand );
    void clearCmd( void );
};