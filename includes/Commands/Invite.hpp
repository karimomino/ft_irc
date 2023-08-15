# pragma once

#include "ICommand.hpp"

class Invite : public ICommand {
public:
    Invite();
    Invite( Server& ircServ , std::string rawCommand , AClient& client);
    ~Invite();
    void execute( void );
    void clearCmd( void );
};
