# pragma once

#include "ICommand.hpp"

class Server;
class Channel;
class Client;

class Mode: public ICommand {
private:
    AClient*          _client;
    Channel*          _channel;
    std::string       _rawCommand;
    const std::string _types;
    bool              _mode;
    std::vector<std::string>           _args;
    std::vector<std::string>::iterator _args_it;
    void ( Mode::*_channel_funPtr[4] )( void );

    void inviteMode( void );
    void topicMode( void );
    void keyMode( void );
    void opMode( void );
    void getArgs( void );

public:
    Mode();
    Mode( Server& ircServ );
    ~Mode();
    void execute( AClient *, const std::string & rawCommand );
    void clearCmd( void );
};
