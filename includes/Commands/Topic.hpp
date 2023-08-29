# pragma once

#include "ICommand.hpp"
#include "../AClient.hpp"
#include "../Server.hpp"

class Topic: public ICommand {
private:
    void _whichTopicExecute(AClient* client, Channel* chan , std::vector<std::string> targetTopic);
    bool _errorFound(AClient* client, std::vector<std::string> targetTopic);

public:
    Topic();
    Topic( Server& ircServ );
    ~Topic();
    void execute( AClient *, const std::string & rawCommand );
    void clearCmd( void );

};
