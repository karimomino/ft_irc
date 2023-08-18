#include "Commands/Join.hpp"

Join::Join( Server& ircServ ) : ICommand( ircServ ) {}

Join::~Join( void ) {

}
void Join::execute( AClient* client, const std::string & rawCommand ){ (void)client; (void)rawCommand; }
void Join::clearCmd( void ){}