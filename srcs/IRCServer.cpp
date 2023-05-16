#include "IRCServer.hpp"

IRCServer::IRCServer( int port, std::string const password ) : _port( port ), _password( password ) {
    (void)_port;
}

IRCServer::~IRCServer( void ) {}
