#include "Commands/Invite.hpp"

Invite::Invite( Server& ircServ ) : ICommand( ircServ ) {}
void Invite::execute( AClient & client, const std::string & rawCommand ){ (void)client; (void)rawCommand; };
void Invite::clearCmd( void ){};