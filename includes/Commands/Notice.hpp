# pragma once

#include "ICommand.hpp"
#include "../AClient.hpp"
#include "../Server.hpp"

class Notice: public ICommand {
public:
    Notice( Server& ircServ );
    ~Notice( void );
    void execute( AClient *, const std::string & rawCommand );
    void clearCmd( void );
};