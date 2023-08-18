# pragma once

#include "Server.hpp"

class Kick : public ICommand {
private:
    std::vector<std::string> _nicks;
    std::vector<std::string> _channels;
    std::vector<std::string> _reasons;

    void extractNicks( void );
    std::vector<std::string> extractArgs( const std::string& delm );
    void getArgs( void );

public:
    Kick( Server & serv );
    void execute( Client* client, const std::string& command );
    void clearCmd( void );
};
