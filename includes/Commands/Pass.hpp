# pragma once

#include "ICommand.hpp"

class Pass: public ICommand {
public:
    Pass( Server& ircServ , std::string rawCommand , AClient& client);
    void execute( void );
    void clearCmd( void );
};