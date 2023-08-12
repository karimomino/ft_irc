#include "PreClient.hpp"

PreClient::PreClient( int socketFd, struct sockaddr_in* hint ) : _socketFd( socketFd ), _ip( inet_ntoa( (struct in_addr)hint->sin_addr ) ) {
}

const std::string& PreClient::getNick( void ) const { return( _nick ); }

const std::string& PreClient::getPass( void ) const { return( _pass ); }

const std::string& PreClient::getUser( void ) const { return( _user ); }

const std::string& PreClient::getIp( void ) const { return ( _ip ); }

int PreClient::getSocketFd( void ) const { return( _socketFd ); }

void PreClient::setNick( const std::string& nick ) { _nick = nick; }

void PreClient::setPass( const std::string& pass ) { _pass = pass; }

void PreClient::setUser( const std::string& user ) { _user = user; }
