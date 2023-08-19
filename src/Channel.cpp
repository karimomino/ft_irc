#include "Channel.hpp"

Channel::Channel( const std::string& name, const std::string& topic ) :
    _name( name ), _topic( topic ), _key( "" ), _isInviteOnly( false ), _isTopicOnly( false ) {}

Channel::~Channel() {}

/* Methods */
void Channel::addUser( const std::string& nick, Client* client ) {
    if ( !isMember( nick ) ) {
	_clients.insert( std::pair<std::string, Client*>( nick, client ) );
    }
}

void Channel::kickUser( const std::string& nick, const std::string& msg ) {
    std::map<std::string, AClient*>::iterator it;

    it = _clients.find( nick );
    if ( it == _clients.end() )
	it = _clients.find( "@" + nick );
    if ( it != _clients.end() ) {
	it->second->addMsg( msg );
	_clients.erase( it );
    }
}

void Channel::addInvitation( const std::string& nick ) {
    if ( !isInvited( nick ) )
	_invitations.push_back( nick );
}

void Channel::removeInvitation( const std::string& nick ) {
    _invitations.erase( std::find(_invitations.begin(), _invitations.end(), nick ) );
}

void Channel::addMsg( const std::string& msg ) {
    std::map<std::string, AClient*>::iterator it;

    for ( it = _clients.begin(); it != _clients.end(); it++ )
	it->second->addMsg( msg );
}

bool  Channel::isInvited( const std::string& nick ) const {
    if ( std::find( _invitations.begin(), _invitations.end(), nick ) == _invitations.end() )
	return ( false );
    return ( true );
}

bool  Channel::isMember( const std::string& nick ) const {
    if ( _clients.find( nick ) == _clients.end() && !isOperator( nick ) )
	return( false );
    return ( true );
}

bool  Channel::isOperator( const std::string& nick ) const {
    if ( _clients.find( "@" + nick ) == _clients.end() )
	return ( false );
    return ( true );
}

bool  Channel::isInviteOnly( void ) const { return ( _isInviteOnly ); }

bool  Channel::isTopicOnly( void ) const { return ( _isTopicOnly ); }

const std::string& Channel::getName( void ) const { return ( _name ); }

const std::string& Channel::getTopic( void ) const { return ( _topic ); }

const std::string& Channel::getKey( void ) const { return ( _key ); }

void Channel::setName( const std::string& name ) { _name = name; }

void Channel::setInviteMode( bool mode ) { _isInviteOnly = mode; }

void Channel::setTopicMode( bool mode ) { _isTopicOnly = mode; }
