#include "Commands/Invite.hpp"
#include "AClient.hpp"
#include "Server.hpp"
#include "Channel.hpp"

Invite::Invite( Server& ircServ ) : ICommand( ircServ ) {}

Invite::~Invite( void ) {}

void Invite::initArgs( void ) {
    const std::vector<std::string>& args = utils::split( _rawCommand, " " );
    std::vector<std::string>::const_iterator it = args.begin();

    _invitedNick = *( it++ );
    _invitedClient = _ircServ._findClientByNick( _invitedNick );
    std::map<const std::string, Channel*>::iterator chan_it = _ircServ._channels.find( *it );
    if ( chan_it == _ircServ._channels.end() )
        throw ( std::runtime_error( ERR_NOSUCHCHANNEL( _ircServ.getIp(), _client->getNick() + " " +  *it ) ) );
    _channel = chan_it->second;
}

void Invite::validateArgs( void ) const {
    if ( _rawCommand.find( "," ) != std::string::npos )
        throw ( std::runtime_error( ERR_MULTIPLEPARAMS( _ircServ.getIp(), _client->getNick() + " INVITE " ) ) );

    if ( _invitedNick.empty() )
        throw ( std::runtime_error( ERR_NEEDMOREPARAMS( _ircServ.getIp(), _client->getNick() + " INVITE " ) ) );
    else if ( _channel->isMember( _invitedNick ) )
        throw ( std::runtime_error( ERR_USERONCHANNEL( _ircServ.getIp(),
            _client->getNick() + " " + _invitedNick + " " + _channel->getName() ) ) );
    else if ( !_invitedClient )
        throw ( std::runtime_error( ERR_NOSUCHNICK( _ircServ.getIp(), _invitedNick ) ) );

    if ( !_channel->isMember( _client->getNick() ) )
        throw ( std::runtime_error( ERR_NOTONCHANNEL( _ircServ.getIp(),
            _client->getNick() + " " + _channel->getName() ) ) );
    else if ( _channel->isInviteOnly() && !_channel->isOperator( _client->getNick() ) )
        throw ( std::runtime_error( ERR_CHANOPRIVSNEEDED( _ircServ.getIp(),
            _client->getNick() + " " + _channel->getName() ) ) );
}

void Invite::execute( AClient* client, const std::string & rawCommand ){
    _client = client;
    _rawCommand = rawCommand;
    try {
        initArgs();
        validateArgs();
        _channel->addInvitation( _invitedNick );
        _client->addMsg( RPL_INVITING( _ircServ.getIp(),
            _client->getNick() + " " + _invitedNick + " " + _channel->getName() ) );
        _invitedClient->addMsg( _client->getOrigin() + " INVITE "
            + _invitedNick + " " + _channel->getName() + "\r\n" );

    } catch ( const std::exception& e ) {
        _client->addMsg( e.what() );
    }
    clearCmd();
}

void Invite::clearCmd( void ) {
    _client = NULL;
    _invitedClient = NULL;
    _rawCommand.clear();
    _invitedNick.clear();
}
