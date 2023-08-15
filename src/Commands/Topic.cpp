#include "Commands/Topic.hpp"

Topic::Topic( Server& ircServ , std::string rawCommand , AClient& client ) : ICommand( ircServ , client , rawCommand ) {}
void Topic::execute( void ){};
void Topic::clearCmd( void ){};