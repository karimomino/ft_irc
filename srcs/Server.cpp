#include "Server.hpp"

Server::Server( int port, std::string const password ) : _port( port ), _password( password ) {
    this->_socketfd = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

    this->_servAddr.sin_family = AF_INET;
    this->_servAddr.sin_addr.s_addr = htonl( INADDR_ANY );
    this->_servAddr.sin_port = htons( this->_port );
}

Server::~Server( void ) {}

void Server::initServer( void ) {
    if ( this->_socketfd == - 1 )
        throw SocketError();

    if ( bind( this->_socketfd,                                                \
            ( struct sockaddr * )&this->_servAddr, sizeof( this->_servAddr )))
        throw BindError();
}

int Server::getPort( void ) const {
    return ( this->_port );
}

std::string Server::getPassword( void ) const {
    return ( this->_password );
}

bool Server::checkPort( void ) const {
    return ( true );
}

void Server::handleClient( void ) {
    listen( this->_socketfd, MAX_CLIENTS );

    int clientfd = accept( this->_socketfd,                                    \
            ( struct sockaddr * )&this->_servAddr,                             \
            ( socklen_t * )&this->_servAddr.sin_len );

    if ( clientfd == -1 )
        throw ClientError();
    
    this->_clients.push_back( Client( clientfd ));
}
