#include "Replies.hpp"
#include "Server.hpp"

static std::pair<std::string const, std::string const> getArgs( std::string const & msg ) {
    size_t inviteIndex = msg.find( "INVITE" );
    size_t firstSpace = msg.find( " ", inviteIndex );
    size_t secondSpace = msg.find( " ", firstSpace + 1 );

    if ( firstSpace == std::string::npos || secondSpace == std::string::npos )
        throw ( std::logic_error( "wrong msg") );

    std::string const userNick = msg.substr( firstSpace + 1, secondSpace - firstSpace - 1);
    std::string const chanName = msg.substr( secondSpace + 1, msg.length() - secondSpace - 3 );

    return ( std::pair<std::string const, std::string const>( userNick, chanName ) );
}

static bool checkPermessions( Server const & serv, Client & client, Client const & user, Channel const & chan ) {
    if ( chan.isInviteOnly() && !chan.isOperator( client.getNick() ) ) {
        serv.sendMsg( ERR_CHANOPRIVSNEEDED, client, chan.getName(), "You're not channel operator" );
        return ( false );
    } else if ( !chan.isMember( client.getNick() ) ) {
        serv.sendMsg( ERR_NOTONCHANNEL, client, chan.getName(), "You're not on that channel" );
        return ( false );
    }

    if ( chan.isMember( user.getNick() ) ) {
        serv.sendMsg( ERR_USERONCHANNEL, client, user.getNick() + NBSP + chan.getName(), "is already on channel" );
        return ( false );
    }

    return ( true );
}

void    Server::_inviteCommand( Client & client, std::string const & msg ) {
    std::cout << "IN BITCHH" << std::endl;
    std::cout << "msg: [" << msg << "]" << std::endl;
    try {
        std::pair<std::string const, std::string const> args = getArgs( msg );

        Client * user = _findClientByNick( _clientMap, args.first );
        if ( !user ) {
            sendMsg( ERR_NOSUCHNICK, client, args.first, "No such nick" );
            return ;
        }

        if ( _channels.find( args.second ) == _channels.end() ) {
            sendMsg( ERR_NOSUCHCHANNEL, client, args.second, "No such channel" );
            return ;
        }
        Channel & chan = _channels.find( args.second )->second;

        if ( !checkPermessions( *this, client, *user, chan ) )
            return ;

        chan.addInvitation( client.getNick() );
        sendMsg( RPL_INVITING, client, user->getNick() + " " + chan.getName(), "Invite sent to user" );
        sendMsg( *user, message::getMsgOrigin( client ), "INVITE " + user->getNick() + " " + chan.getName() );
    } catch ( const std::logic_error & e ) {
        DEBUG_MSG( e.what() << std::endl );
        return ;
    }
}
