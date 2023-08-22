# pragma once

#include "ICommand.hpp"
#include "../AClient.hpp"
#include "../Server.hpp"

typedef std::pair<std::string , std::string> strPair;

class Join : public ICommand {
public:
    Join();
    Join( Server& ircServ );
    ~Join();
    void execute( AClient *, const std::string & rawCommand );
    void clearCmd( void );
};
