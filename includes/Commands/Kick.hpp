# pragma once

#include "Commands/ICommand.hpp"

class Server;
class Channel;
class Client;

class Kick : public ICommand {
private:
    AClient*    _client;
    Channel*    _channel;
    std::string _kickedNick;
    std::string _rawCommand;
    std::string _reason;

    void initArgs( void ) ;
    void validateArgs( void ) const;

public:
    Kick( Server& ircServ );
    ~Kick();

    void execute( AClient *, const std::string & rawCommand );
    void clearCmd( void );

    friend class Server;
};
