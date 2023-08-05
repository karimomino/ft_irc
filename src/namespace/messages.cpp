#include "namespace.hpp"

std::vector<std::string const> message::extractNickList( std::string const & msg ) {
    size_t hash = msg.find( "#" );
    size_t firstSpace = msg.find( " ", hash );
    size_t secondSpace = msg.find( " ", firstSpace + 1);

    std::string const nickStr = msg.substr( firstSpace + 1, secondSpace - firstSpace  - 1 );

    std::vector<std::string const> list;
    if ( msg.find( "," ) == std::string::npos )
	list.push_back( nickStr ); 
    else
	list = utils::csplit( nickStr, "," );

    return ( list );
}

std::vector<std::string const> message::extractArgs( std::string const & msg, std::string const & delm ) {
    size_t delmIndex = msg.find( delm );
    size_t spaceAfter = msg.find( " ", delmIndex );

    std::string const argStr = msg.substr( delmIndex + 1, spaceAfter - delmIndex  - 1 );

    std::vector<std::string const> list;
    if ( msg.find( "," ) == std::string::npos )
	list.push_back( argStr ); 
    else
	list = utils::csplit( argStr, "," );

    return ( list );
}

std::string const message::getMsgOrigin( Client const & client ) {
    // TODO: remove "127.0.0.1" after implementing getIp
    std::string const origin = ":" + client.getNick() + "!" + client.getUser() + "@127.0.0.1" + client.getIp() + " ";

    DEBUG_MSG("ORIGIN OF MESSAGE: " << origin);
    return ( origin );
}

