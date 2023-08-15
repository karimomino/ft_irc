# pragma once

#include "ICommand.hpp"

class Join : public ICommand {
public:
    Join();
    Join( Server& ircServ , std::string rawCommand , AClient& client);
    ~Join();
    void execute( void );
    void clearCmd( void );
};
