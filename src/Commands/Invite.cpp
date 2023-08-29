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
        throw ( ERR_NOSUCHCHANNEL( _ircServ.getIp(), _client->getNick() + " " +  *it ) );
}

void Invite::validateArgs( void ) const {
    if ( _rawCommand.find( "," ) != std::string::npos )
        throw ( ERR_MULTIPLEPARAMS( _ircServ.getIp(), _client->getNick() + " INVITE " ) );

    if ( !_channel->isMember( _client->getNick() ) )
        throw ( ERR_NOTONCHANNEL( _ircServ.getIp(),
            _client->getNick() + " " + _channel->getName() ) );
    else if ( !_channel->isOperator( _client->getNick() ) && _channel->isInviteOnly() )
        throw ( ERR_CHANOPRIVSNEEDED( _ircServ.getIp(),
            _client->getNick() + " " + _channel->getName() ) );

    if ( _invitedNick.empty() )
        throw ( ERR_NEEDMOREPARAMS( _ircServ.getIp(), _client->getNick() + " INVITE " ) );
    else if ( _channel->isMember( _invitedNick ) )
        throw ( ERR_USERONCHANNEL( _ircServ.getIp(),
            _client->getNick() + " " + _invitedNick + " " + _channel->getName() ) );
    else if ( !_invitedClient )
        throw ( ERR_NOSUCHNICK( _ircServ.getIp(), _invitedNick ) );
}

void Invite::execute( AClient* client, const std::string & rawCommand ){
    _client = client;
    _rawCommand = rawCommand;
    try {
        initArgs();
        validateArgs();
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
