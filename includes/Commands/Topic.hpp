# pragma once

#include "ICommand.hpp"

class Topic: public ICommand {
public:
    Topic();
    Topic( Server& ircServ , std::string rawCommand , AClient& client);
    ~Topic();
    void execute( void );
    void clearCmd( void );
};
