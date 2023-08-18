#include "Commands/Kick.hpp"

Kick::Kick( Server& ircServ ) : ICommand( ircServ ) {}
void Kick::execute( AClient & client, const std::string & rawCommand ){ (void)client; (void)rawCommand; };
void Kick::clearCmd( void ){};