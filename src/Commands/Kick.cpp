#include "Commands/Kick.hpp"

Kick::Kick( Server & serv ) : ICommand( serv ) {}

static std::vector<std::string> splitArgs( const std::string& argsStr ) {
    std::vector<std::string> list;

    if ( argsStr.find( "," ) == std::string::npos )
	list.push_back( argsStr );
    else
	list = utils::split( argsStr, "," );
    return ( list );
}

void Kick::extractNicks( void ) {
    size_t hash = _raw.find( "#" );
    size_t firstSpace = _raw.find( "#", hash );
    size_t secondSpace = _raw.find( "#", firstSpace );

    const std::string nicksStr = _raw.substr( firstSpace + 1, secondSpace - firstSpace - 1 );
    _nicks = splitArgs( nicksStr );
}

std::vector<std::string> Kick::extractArgs( const std::string& delm ) {
    size_t delmIndex = _raw.find( delm );
    size_t spaceAfter = _raw.find( " ", delmIndex );

    std::string const argsStr = _raw.substr( delmIndex + 1, spaceAfter - delmIndex  - 1 );
    return ( splitArgs( argsStr ) );
}

void Kick::getArgs( void ) {
    extractNicks();
    _channels = extractArgs( "#" );
    _reasons = extractArgs( ":" );
}

void Kick::execute( Client* client, const std::string& command ) {
    _client = client;
    _raw = command;

    getArgs();
}

void Kick::clearCmd( void ) {
    _client = NULL;
    _raw = "";
}
