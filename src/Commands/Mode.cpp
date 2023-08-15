#include "Commands/Mode.hpp"

Mode::Mode( Server& ircServ , std::string rawCommand , AClient& client ) : ICommand( ircServ , client , rawCommand ) {}
void Mode::execute( void ){};
void Mode::clearCmd( void ){};