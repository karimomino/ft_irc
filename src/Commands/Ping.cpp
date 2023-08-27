#include "Commands/Ping.hpp"

Ping::Ping( Server& ircServ ) : ICommand( ircServ ) {}

Ping::~Ping( void ) {

}
void Ping::execute( AClient* client, const std::string & rawCommand ) {
    client->addMsg( ":" + _ircServ.getIp() + " PONG " + _ircServ.getIp() + " :" + rawCommand + "\r\n" );
};
void Ping::clearCmd( void ){};