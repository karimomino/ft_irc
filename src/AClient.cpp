#include "AClient.hpp"

AClient::AClient( int socketFd, struct sockaddr_in* hint ) : _socketFd( socketFd ) , _purge(false), _ip( inet_ntoa( (struct in_addr)hint->sin_addr ) ) {
}

AClient::AClient( const AClient& original ) {
    _socketFd = original.getSocketFd();
    _nick = original.getNick();
    _user = original.getUser();
    _pass = original.getPass();
    _ip = original.getIp();
}

AClient::~AClient() {
}

void AClient::addMsg( const std::string& msg) { _msgs.push( msg ); }

void AClient::sendMSg( void ) {
    if (_msgs.size()) {
        std::string msg = getFirstMsg();
        send(_socketFd , msg.c_str() , msg.length() , MSG_DONTWAIT);
        _msgs.pop();
    }
}

const std::string& AClient::getNick( void ) const { return( _nick ); }

const std::string& AClient::getPass( void ) const { return( _pass ); }

const std::string& AClient::getUser( void ) const { return( _user ); }

const std::string& AClient::getIp( void ) const { return ( _ip ); }

const std::string AClient::getOrigin( void ) const {
    std::string const origin = ":" + getNick() + "!" + getUser() + "@" + getIp();
    return ( origin );
}

const int& AClient::getSocketFd( void ) const { return( _socketFd ); }

const bool& AClient::getPurge( void ) const { return _purge; }

int AClient::getQueueSize( void ) const { return _msgs.size(); }

const std::string& AClient::getFirstMsg( void) const { return _msgs.front(); }

void AClient::setNick( const std::string& nick ) { _nick = nick; }

void AClient::setPass( const std::string& pass ) { _pass = pass; }

void AClient::setUser( const std::string& user ) { _user = user; }

void AClient::setPurge( const bool& purge ) { _purge = purge; }


