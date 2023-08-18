# pragma once

#include "ICommand.hpp"
#include "../AClient.hpp"
#include "../Server.hpp"

void trim( std::string& str );
std::vector<std::string> splitDelim( std::string str , std::string delim);
class User: public ICommand {
public:
    User( Server& ircServ );
    ~User( void );
    void execute( AClient *, const std::string & rawCommand );
    void clearCmd( void );
};