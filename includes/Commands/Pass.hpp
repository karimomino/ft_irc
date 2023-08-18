# pragma once

#include "ICommand.hpp"
#include "../AClient.hpp"
#include "../Server.hpp"

class Pass: public ICommand {
public:
    Pass( Server& ircServ );
    ~Pass( void );
    void execute( AClient *, const std::string & rawCommand );
    void clearCmd( void );
};