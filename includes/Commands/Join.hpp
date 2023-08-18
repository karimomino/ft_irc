# pragma once

#include "ICommand.hpp"

class Join : public ICommand {
public:
    Join();
    Join( Server& ircServ );
    ~Join();
    void execute( AClient &, const std::string & rawCommand );
    void clearCmd( void );
};
