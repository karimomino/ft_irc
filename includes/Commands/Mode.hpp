# pragma once

#include "ICommand.hpp"

class Mode: public ICommand {
public:
    Mode();
    Mode( Server& ircServ , std::string rawCommand , AClient& client);
    ~Mode();
    void execute( void );
    void clearCmd( void );
};
