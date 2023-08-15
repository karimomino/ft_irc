#include "Commands/Pass.hpp"

Pass::Pass( Server& ircServ , std::string rawCommand , AClient& client) : ICommand( ircServ , client , rawCommand) {

}

void Pass::execute( void ){};
void Pass::clearCmd( void ){};