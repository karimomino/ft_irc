# pragma once

#include "ICommand.hpp"

class Kick : public ICommand {
public:
    Kick();
    Kick( Server& ircServ , std::string rawCommand , AClient& client);
    ~Kick();
    void execute( void );
    void clearCmd( void );
};
