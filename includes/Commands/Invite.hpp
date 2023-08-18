# pragma once

#include "ICommand.hpp"
#include "../AClient.hpp"
#include "../Server.hpp"

class Invite : public ICommand {
public:
    Invite();
    Invite( Server& ircServ );
    ~Invite();
    void execute( AClient *, const std::string & rawCommand );
    void clearCmd( void );
};