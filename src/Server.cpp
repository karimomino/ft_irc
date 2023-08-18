#include "Server.hpp"

Server::Server( int port, const std::string& pass ) : _port(  port ), _pass( pass ) {
    _initCmds();
}

Server::~Server( void ) {
    // TODO: clean up all the clients
}

void Server::_initCmds( void ) {
    _cmds.insert( std::pair<const std::string, ICommand*>( "JOIN", new Join() ) );
    _cmds.insert( std::pair<const std::string, ICommand*>( "KICK", new Kick( *this ) ) );
    _cmds.insert( std::pair<const std::string, ICommand*>( "MODE", new Mode() ) );
    _cmds.insert( std::pair<const std::string, ICommand*>( "TOPIC", new Topic() ) );
    _cmds.insert( std::pair<const std::string, ICommand*>( "INVITE", new Invite() ) );
}

void Server::_addClient( void ) {}

void Server::_addChannel( void ) {}

void Server::_removeClient( void ) {}

void Server::_removeChannel( void ) {}

void Server::init( void ) {
	int sock_opt = 1;

	if ( ( _socketFd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
		throw ( Server::ServerError( "Error creating the server socket") );
	if ( setsockopt( _socketFd, SOL_SOCKET, SO_REUSEADDR, &sock_opt, sizeof( sock_opt ) ) )
		throw ( Server::ServerError( "Error setting socket options" ) );
	if ( fcntl( _socketFd, F_SETFL, O_NONBLOCK ) == -1 )
		throw ( Server::ServerError( "Error setting socket to non-blocking" ) );
    if ( bind( _socketFd, ( sockaddr* )&_hint, sizeof( _hint) ) < 0 )
		throw ( Server::ServerError( "Error binding" ) );
    if (listen( _socketFd, MAX_CLIENTS))
		throw ( Server::ServerError( "Failed to listen" ) );

    pollfd tmp_fd;
	tmp_fd.fd = _socketFd;
	tmp_fd.events = POLLIN;
	tmp_fd.revents = 0;
	_pollFds.push_back(tmp_fd);
	_connectionCount = 1;
	struct in_addr ipAddr = _hint.sin_addr;

	std::string ip_string = inet_ntoa(ipAddr);
	_ip = ip_string;
}

void Server::run( void ) {
}

void Server::exit( void ) {
    // TODO: clean up before the destructor is called
}

const std::string& Server::getPass( void ) const { return ( _pass ); }

const std::string& Server::getIp( void ) const { return ( _ip ); }
