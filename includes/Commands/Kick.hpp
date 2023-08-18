# pragma once

#include "ICommand.hpp"

class Kick : public ICommand {
public:
    Kick();
    Kick( Server& ircServ );
    ~Kick();
    void execute( AClient &, const std::string & rawCommand );
    void clearCmd( void );
};
