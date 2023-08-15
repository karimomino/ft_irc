#include "Commands/Invite.hpp"

Invite::Invite( Server& ircServ , std::string rawCommand , AClient& client ) : ICommand( ircServ , client , rawCommand ) {}
void Invite::execute( void ){};
void Invite::clearCmd( void ){};