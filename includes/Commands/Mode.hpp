# pragma once

#include "ICommand.hpp"
#include "../AClient.hpp"
#include "../Server.hpp"

class Mode: public ICommand {
public:
    Mode();
    Mode( Server& ircServ );
    ~Mode();
    void execute( AClient *, const std::string & rawCommand );
    void clearCmd( void );
};
