#include "Commands/Topic.hpp"

Topic::Topic( Server& ircServ ) : ICommand( ircServ ) {}
void Topic::execute( AClient & client, const std::string & rawCommand ){ (void)client; (void)rawCommand; };
void Topic::clearCmd( void ){};