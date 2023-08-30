# pragma once

#include "Commands/ICommand.hpp"

class Server;
class Channel;
class Client;

class Invite : public ICommand {
private:
    AClient*    _client;
    AClient*    _invitedClient;
    Channel*    _channel;
    std::string _rawCommand;
    std::string _invitedNick;

    void initArgs( void );
    void validateArgs( void ) const;

public:
    Invite( Server& ircServ );
    ~Invite();

    void execute( AClient *, const std::string & rawCommand );
    void clearCmd( void );
};
