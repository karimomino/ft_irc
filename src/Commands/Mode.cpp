#include "Commands/Mode.hpp"
#include "AClient.hpp"
#include "Server.hpp"
#include "Channel.hpp"

void Mode::getArgs( void ) {
    std::vector<std::string> args = utils::split( _rawCommand, " " );
    if ( args.size() < 2 )
        // throw ( std::
}

void Mode::inviteMode( void ) {
    _channel->setInviteMode( _mode );
}

void Mode::topicMode( void ) {
    _channel->setTopicMode( _mode );
}

void Mode::keyMode( void ) {
    _channel->setKeyMode( _mode );
}

void Mode::opMode( void ) {}

Mode::Mode( Server& ircServ ) : ICommand( ircServ ), _types( "itko" ) {
    _channel_funPtr[0] = &Mode::inviteMode;
    _channel_funPtr[1] = &Mode::topicMode;
    _channel_funPtr[2] = &Mode::keyMode;
    _channel_funPtr[3] = &Mode::opMode;
}

Mode::~Mode( void ) {}

void Mode::getArgs( void ) {
}

// int findModeType( char c ) {}

void Mode::execute( AClient* client, const std::string & rawCommand ) {
    _client = client;
    _rawCommand = rawCommand;
}

void Mode::clearCmd( void ) {
    _client = NULL;
    _rawCommand.clear();
}
