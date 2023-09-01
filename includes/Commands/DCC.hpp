# pragma once

#include "ICommand.hpp"
#include "../AClient.hpp"
#include "../Server.hpp"

typedef std::pair<std::string , std::string> strPair;

class DCC : public ICommand {
public:
    DCC( Server& ircServ );
    ~DCC();
    void execute( AClient *, const std::string & rawCommand );
    void clearCmd( void );
};
