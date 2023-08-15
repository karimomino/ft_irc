#include "Commands/Pass.hpp"

Pass::Pass( Server& ircServ ) : ICommand( ircServ ) {

}

void Pass::execute( AClient & client, const std::string & rawCommand ){ (void)client; (void)rawCommand; };
void Pass::clearCmd( void ){};