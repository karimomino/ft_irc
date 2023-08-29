#include "Channel.hpp"

Channel::Channel( const std::string& name, const std::string& topic ) :
    _name( name ), _topic( topic ), _key( "" ), _isInviteOnly( false ), _isTopicOnly( true ), _isKeyOnly( false ) {}

Channel::~Channel() {}

/* PRIVATE Methods */

void Channel::_sendNames( AClient* client ) {
    client->addMsg( RPL_NAMREPLY( client->getNick() + " = " + _name , names(*this)) );
    client->addMsg( RPL_ENDOFNAMES( client->getNick() + " " + _name ) );
}

/* PUBLIC Methods */
void Channel::addUser( AClient* client ) {
    std::string nick = client->getNick();
    // TODO: get origin from client member function created by @niño
    std::string joinMsg( client->getOrigin() + " JOIN " + _name + "\r\n" );
    client->addMsg( joinMsg );
    if ( !_clients.size() ) {
        nick.insert(0 , "@");
        client->addMsg( ":0.0.0.0 MODE " + _name + " +t\r\n" );
    }
    else {
        client->addMsg( RPL_TOPIC( client->getNick() + " " + _name , _topic ) );
        client->addMsg( RPL_TOPICWHOTIME( client->getNick() + " " + _name + " dan!~d@localhost 1547691506" ) ); // TODO: use time and get origin and stor in the channel at creation time
        addMsg( client->getNick() , joinMsg);
    }
    _clients.insert( std::make_pair( nick , client ) );
    _sendNames( client );
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
    std::vector<std::string>::iterator begin = _invitations.begin();
    std::vector<std::string>::iterator end = _invitations.end();
    for ( std::vector<std::string>::iterator it = begin; it != end; it++ )
        if ( *it == nick )
            _invitations.erase( it );
}

void Channel::addMsg( const std::string& cli , const std::string& msg ) {
    std::map<std::string, AClient*>::iterator it;

    for ( it = _clients.begin(); it != _clients.end(); it++ )
        if (cli != it->second->getNick())
	        it->second->addMsg( msg );
}

bool  Channel::isInvited( const std::string& nick ) const {
    std::vector<std::string>::const_iterator begin = _invitations.begin();
    std::vector<std::string>::const_iterator end = _invitations.end();
    for ( std::vector<std::string>::const_iterator it = begin; it != end; it++ )
        if ( *it == nick )
            return ( true );
    return ( false );
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

bool  Channel::isKeyOnly( void ) const { return ( _isKeyOnly ); }

const std::string& Channel::getName( void ) const { return ( _name ); }

const std::string& Channel::getTopic( void ) const { return ( _topic ); }

const std::string& Channel::getKey( void ) const { return ( _key ); }

void Channel::setName( const std::string& name ) { _name = name; }

void Channel::setInviteMode( bool mode ) { _isInviteOnly = mode; }

void Channel::setTopicMode( bool mode ) { _isTopicOnly = mode; }

std::string names(const Channel& chan) {
    std::string aggNames;

    for (std::map<std::string , AClient*>::const_iterator i = chan._clients.begin(); i !=chan._clients.end(); i++)
        aggNames += i->first + ",";
    aggNames.erase(aggNames.end() - 1);
    return( aggNames );
}
