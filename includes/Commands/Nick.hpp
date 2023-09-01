# pragma once

#include "ICommand.hpp"
#include "../AClient.hpp"
#include "../Server.hpp"

class Nick: public ICommand {
private:
    void updateNick( AClient* client, const std::string& newNick );
public:
    Nick( Server& ircServ );
    ~Nick( void );
    void execute( AClient *, const std::string & rawCommand );
    void clearCmd( void );
};
