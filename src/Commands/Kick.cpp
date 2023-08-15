#include "Commands/Kick.hpp"

Kick::Kick( Server& ircServ , std::string rawCommand , AClient& client ) : ICommand( ircServ , client , rawCommand ) {}
void Kick::execute( void ){};
void Kick::clearCmd( void ){};