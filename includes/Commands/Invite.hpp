# pragma once

#include "ICommand.hpp"

class Invite : public ICommand {
public:
    Invite();
    Invite( Server& ircServ );
    ~Invite();
    void execute( AClient &, const std::string & rawCommand );
    void clearCmd( void );
};
