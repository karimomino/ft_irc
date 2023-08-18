#include "Commands/Pass.hpp"

Pass::Pass( Server& ircServ ) : ICommand( ircServ ) {

}

Pass::~Pass( void ) {

}

void Pass::execute( AClient* const client, const std::string & rawCommand ){
    PreClient *target = dynamic_cast<PreClient *>(client);
    client->setPass(rawCommand);
    if ( target && !client->getPass().empty() && !client->getNick().empty() && !client->getUser().empty() ) {
        _ircServ._addClient(client);
        _ircServ._clients.erase(client->getSocketFd());
        delete this;
	}
}

void Pass::clearCmd( void ){};