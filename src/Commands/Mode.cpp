#include "Commands/Mode.hpp"

Mode::Mode( Server& ircServ ) : ICommand( ircServ ) {}
void Mode::execute( AClient & client, const std::string & rawCommand ){ (void)client; (void)rawCommand; };
void Mode::clearCmd( void ){};