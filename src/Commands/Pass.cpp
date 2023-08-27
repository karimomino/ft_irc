#include "Commands/Pass.hpp"

Pass::Pass( Server& ircServ ) : ICommand( ircServ ) {

}

Pass::~Pass( void ) {

}

void Pass::execute( AClient* const client, const std::string & rawCommand ){
    PreClient *target = dynamic_cast<PreClient *>(client);
    client->setPass(rawCommand);
    if ( target && !client->getPass().empty() && !client->getNick().empty() && !client->getUser().empty() ) {
        _ircServ._clients.erase(client->getSocketFd());
        _ircServ._addClient(client);
        delete client;
	}
}

void Pass::clearCmd( void ){};