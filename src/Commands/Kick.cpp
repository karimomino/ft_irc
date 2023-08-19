#include "Commands/Kick.hpp"
#include "AClient.hpp"
#include "Server.hpp"
#include "Channel.hpp"

Kick::Kick( Server & serv ) : ICommand( serv ) {}

Kick::~Kick( void ) {}

static const std::vector<std::string> getArgs( const std::string& rawCmd ) {
    std::string tmp;
    std::vector<std::string> list;
    std::stringstream ss( rawCmd );

    for ( int i = 0; i < 3; i++ ) {
        getline(ss, tmp, ' ');
        list.push_back( tmp );
        tmp.clear();
    }
    return ( list );
}

void Kick::initArgs( void ) {
    const std::vector<std::string>& args = getArgs( _rawCommand );
    std::vector<std::string>::const_iterator it = args.begin();

    std::map<std::string, Channel*>::iterator chan_it = _ircServ._channels.find( *it );
    if ( chan_it == _ircServ._channels.end() ) {
        _client->addMsg( ERR_NOSUCHCHANNEL( _ircServ.getIp(),
            _client->getNick() + " " +  *it ) );
        throw ( Kick::CmdError( "Error: [Channel doesn't exist" ) );
    }
    _channel = chan_it->second;
    _kickedNick = *( ++it );
    _reason = *( ++it );
}

void Kick::validateArgs( void ) const {
    if ( _rawCommand.find( "," ) != std::string::npos ) {
        _client->addMsg( ERR_MULITPLEPARAMS( _ircServ.getIp(), _client->getNick() + " KICK " ) );
        throw ( Kick::CmdError( "Error: [Kick found multiple names]" ) );
    }

    if ( _kickedNick.empty() ) {
        _client->addMsg( ERR_NEEDMOREPARAMS( _ircServ.getIp(), _client->getNick() + " KICK" ) );
        throw ( Kick::CmdError( "Error: [Kick can't find the client" ) );
    }

    if ( !_channel->isOperator( _client->getNick() ) ) {
        if ( !_channel->isMember( _client->getNick() ) )
            _client->addMsg( ERR_NOTONCHANNEL( _ircServ.getIp(),
                _client->getNick() + " " +_channel->getName() ) );
        else
            _client->addMsg( ERR_CHANOPRIVSNEEDED( _ircServ.getIp(),
                _client->getNick() + " " + _channel->getName() ) );
        throw ( Kick::CmdError( "Error: [User isn't a channel operator]" ) );
    }

    if ( !_channel->isMember( _kickedNick ) ) {
        if ( !_ircServ._findClientByNick( _kickedNick ) )
            _client->addMsg( ERR_NOSUCHNICK( _ircServ.getIp(), _kickedNick ) ) ;
        else
            _client->addMsg( ERR_USERNOTINCHANNEL( _ircServ.getIp(),
                _kickedNick + " " +_channel->getName() ) );
        throw ( Kick::CmdError( "Error: [Kick can't find the client" ) );
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
    } catch ( const std::exception& e ) { /* TODO: print the error msg  */ }

    clearCmd();
}

void Kick::clearCmd( void ) {
    _client = NULL;
    _channel = NULL;
    _kickedNick.clear();
    _rawCommand.clear();
    _reason.clear();
}
