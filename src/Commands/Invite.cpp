#include "Commands/Invite.hpp"
#include "AClient.hpp"
#include "Server.hpp"
#include "Channel.hpp"

Invite::Invite( Server& ircServ ) : ICommand( ircServ ) {}

Invite::~Invite( void ) {}

static const std::vector<std::string> getArgs( const std::string& rawCmd ) {
    std::string tmp;
    std::vector<std::string> list;
    std::stringstream ss( rawCmd );

    for ( int i = 0; i < 2; i++ ) {
        getline(ss, tmp, ' ');
        list.push_back( tmp );
        tmp.clear();
    }
    return ( list );
}

void Invite::initArgs( void ) {
    const std::vector<std::string>& args = getArgs( _rawCommand );
    std::vector<std::string>::const_iterator it = args.begin();

    _invitedNick = *( it++ );
    _invitedClient = _ircServ._findClientByNick( _invitedNick );
    std::map<std::string, Channel*>::iterator chan_it = _ircServ._channels.find( *it );
    if ( chan_it == _ircServ._channels.end() ) {
        _client->addMsg( ERR_NOSUCHCHANNEL( _ircServ.getIp(),
            _client->getNick() + " " +  *it ) );
        throw ( Invite::CmdError( "Error: [Channel doesn't exist" ) );
    }
}

void Invite::validateArgs( void ) const {
    if ( _rawCommand.find( "," ) != std::string::npos ) {
        _client->addMsg( ERR_MULITPLEPARAMS( _ircServ.getIp(), _client->getNick() + " INVITE " ) );
        throw ( Invite::CmdError( "Error: [Invite found multiple targets]" ) );
    }

    if ( !_channel->isMember( _client->getNick() ) ) {
        _client->addMsg( ERR_NOTONCHANNEL( _ircServ.getIp(),
            _client->getNick() + " " + _channel->getName() ) );
        throw ( Invite::CmdError( "Error: [You're not on that channel]" ) );
    } else if ( !_channel->isOperator( _client->getNick() ) && _channel->isInviteOnly() ) {
        _client->addMsg( ERR_CHANOPRIVSNEEDED( _ircServ.getIp(),
            _client->getNick() + " " + _channel->getName() ) );
        throw ( Invite::CmdError( "Error: [You're not a channel operator]" ) );
    }

    if ( _invitedNick.empty() ) {
        _client->addMsg( ERR_NEEDMOREPARAMS( _ircServ.getIp(), _client->getNick() + " INVITE " ) );
        throw ( Invite::CmdError( "Error: [Invite no enough parameters]" ) );
    } else if ( _channel->isMember( _invitedNick ) ) {
        _client->addMsg( ERR_USERONCHANNEL( _ircServ.getIp(),
            _client->getNick() + " " + _invitedNick + " " + _channel->getName() ) );
        throw ( Invite::CmdError( "Error: [Invited user is already on channel]") );
    } else if ( !_invitedClient ) {
        _client->addMsg( ERR_NOSUCHNICK( _ircServ.getIp(), _invitedNick ) ) ;
        throw ( Invite::CmdError( "Error: [Can't find nick on the server]") );
    }
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

    } catch ( const std::exception& e ) { /* TODO: print the error msg  */ }
    clearCmd();
}

void Invite::clearCmd( void ) {
    _client = NULL;
    _invitedClient = NULL;
    _rawCommand.clear();
    _invitedNick.clear();
}
