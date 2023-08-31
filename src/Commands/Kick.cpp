#include "Commands/Kick.hpp"
#include "AClient.hpp"
#include "Server.hpp"
#include "Channel.hpp"

Kick::Kick( Server & serv ) : ICommand( serv ) {}

Kick::~Kick( void ) {}

void Kick::initArgs( const std::string& rawCommand ) {
    const std::vector<std::string>& args = utils::split( rawCommand, " " );
    if ( args.size() < 2 )
        throw ( std::runtime_error( ERR_NEEDMOREPARAMS( _ircServ.getIp(), _client->getNick() + " KICK" ) ) );
    else if ( args.size() > 3 )
        throw ( std::runtime_error ( ERR_MULTIPLEPARAMS( _ircServ.getIp(), _client->getNick() + " KICK " ) ) );

    std::map<const std::string, Channel*>::iterator chan_it = _ircServ._channels.find( args[0] );
    if ( chan_it == _ircServ._channels.end() )
        throw ( std::runtime_error( ERR_NOSUCHCHANNEL( _ircServ.getIp(),
            _client->getNick() + " " +  args[0] ) ) );

    _channel = chan_it->second;
    _kickedNick = args[1];
    _reason = args[2];
}

void Kick::validateArgs( void ) const {
    if ( !_channel->isOperator( _client->getNick() ) ) {
        if ( !_channel->isMember( _client->getNick() ) )
            throw ( std::runtime_error( ERR_NOTONCHANNEL( _ircServ.getIp(), _client->getNick() + " " +_channel->getName() ) ) );
        else
            throw ( std::runtime_error( ERR_CHANOPRIVSNEEDED( _ircServ.getIp(), _client->getNick() + " " + _channel->getName() ) ) );
    }

    if ( !_channel->isMember( _kickedNick ) ) {
        if ( !_ircServ._findClientByNick( _kickedNick ) )
            throw ( std::runtime_error( ERR_NOSUCHNICK( _ircServ.getIp(), _kickedNick ) ) );
        else
            throw ( std::runtime_error( ERR_USERNOTINCHANNEL( _ircServ.getIp(), _kickedNick + " " +_channel->getName() ) ) );
    }
}

void Kick::execute( AClient* client, const std::string& rawCommand ) {
    _client = client;
    try {
        initArgs( rawCommand );
        validateArgs();
        std::string kickResponse = _client->getOrigin() + " KICK " + _channel->getName()
        + " " + _kickedNick + " :";
        kickResponse += _reason.empty() ? "No Reason\r\n" : _reason + "\r\n";
        _channel->kickUser( _kickedNick, kickResponse );
        _channel->addMsg( _kickedNick, kickResponse );
    } catch ( const std::exception& e ) {
        _client->addMsg( e.what() );
    }
    clearCmd();
}

void Kick::clearCmd( void ) {
    _client = NULL;
    _channel = NULL;
    _kickedNick.clear();
    _reason.clear();
}
