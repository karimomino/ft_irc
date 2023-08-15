#include "Commands/Join.hpp"

Join::Join( Server& ircServ , std::string rawCommand , AClient& client ) : ICommand( ircServ , client , rawCommand ) {}
void Join::execute( void ){}
void Join::clearCmd( void ){}