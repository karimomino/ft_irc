#include "Commands/Nick.hpp"

Nick::Nick( Server& ircServ ) : ICommand( ircServ ) {

}

Nick::~Nick( void ) {

}

void Nick::execute( AClient* const client, const std::string & rawCommand ){
    PreClient *target = dynamic_cast<PreClient *>(client);
    client->setNick(rawCommand);
    if ( target && !client->getNick().empty() && !client->getUser().empty() && !client->getPass().empty() ) {
        _ircServ._addClient(client);
        _ircServ._clients.erase(client->getSocketFd());
        delete client;
	}
}

void Nick::clearCmd( void ) {
    
};