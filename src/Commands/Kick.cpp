#include "Commands/Kick.hpp"

Kick::Kick( Server& ircServ ) : ICommand( ircServ ) {}

Kick::~Kick( void ) {

}
void Kick::execute( AClient* client, const std::string & rawCommand ){ (void)client; (void)rawCommand; };
void Kick::clearCmd( void ){};