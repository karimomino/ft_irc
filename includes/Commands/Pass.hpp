# pragma once

#include "ICommand.hpp"

class Pass: public ICommand {
public:
    Pass( Server& ircServ );
    void execute( AClient &, const std::string & rawCommand );
    void clearCmd( void );
};