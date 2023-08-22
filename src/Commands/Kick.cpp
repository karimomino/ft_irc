#include "Commands/Kick.hpp"
#include "AClient.hpp"
#include "Server.hpp"
#include "Channel.hpp"

Kick::Kick( Server & serv ) : ICommand( serv ) {}

Kick::~Kick( void ) {}

void Kick::initArgs( void ) {
    const std::vector<std::string>& args = utils::split( _rawCommand, " " );
    if ( args.size() < 2 )
        throw ( ERR_NEEDMOREPARAMS( _ircServ.getIp(), _client->getNick() + " KICK" ) );
    else if ( args.size() > 3 )
        throw ( ERR_MULTIPLEPARAMS( _ircServ.getIp(), _client->getNick() + " KICK " ) );

    std::map<std::string, Channel*>::iterator chan_it = _ircServ._channels.find( args[0] );
    if ( chan_it == _ircServ._channels.end() ) {
        _client->addMsg( ERR_NOSUCHCHANNEL( _ircServ.getIp(),
            _client->getNick() + " " +  args[0] ) );
        throw ( Kick::CmdError( "Error: [Channel doesn't exist" ) );
    }

    _channel = chan_it->second;
    _kickedNick = args[1];
    _reason = args[2];
}

void Kick::validateArgs( void ) const {
    if ( !_channel->isOperator( _client->getNick() ) ) {
        if ( !_channel->isMember( _client->getNick() ) )
            throw ( ERR_NOTONCHANNEL( _ircServ.getIp(), _client->getNick() + " " +_channel->getName() ) );
        else
            throw ( ERR_CHANOPRIVSNEEDED( _ircServ.getIp(), _client->getNick() + " " + _channel->getName() ) );
    }

    if ( !_channel->isMember( _kickedNick ) ) {
        if ( !_ircServ._findClientByNick( _kickedNick ) )
            throw ( ERR_NOSUCHNICK( _ircServ.getIp(), _kickedNick ) );
        else
            throw ( ERR_USERNOTINCHANNEL( _ircServ.getIp(), _kickedNick + " " +_channel->getName() ) );
    }
}

void Kick::execute( AClient* client, const std::string& rawCommand ) {
    _client = client;
    _rawCommand = rawCommand;
    try {
        initArgs();
        validateArgs();
        std::string kickResponse = _client->getOrigin() + "KICK " + _channel->getName()
        + " " + _kickedNick + " :";
        kickResponse += _reason.empty() ? "No Reason\r\n" : _reason + "\r\n";
        _channel->kickUser( _kickedNick, kickResponse );
        _channel->addMsg( kickResponse );
    } catch ( const std::exception& e ) {
        _client->addMsg( e.what() );
    }

    clearCmd();
}

void Kick::clearCmd( void ) {
    _client = NULL;
    _channel = NULL;
    _kickedNick.clear();
    _rawCommand.clear();
    _reason.clear();
}
