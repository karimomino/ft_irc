#include "Server.hpp"
#include "Replies.hpp"

// S <-   :dan!d@localhost KICK #test alice :dan
static std::string const kickResponse( Channel const & chan, Client const & cmdUser, std::string const & kickedUser ) {
    std::string const msg = message::getMsgOrigin( cmdUser ) + "KICK " + chan.getName()
        + " " + kickedUser + " :" + cmdUser.getNick() + "\r\n";
    return ( msg );
}

static std::string const kickResponse( Channel const & chan, Client const & cmdUser, std::string const & kickedUser, std::string const & reason ) {
    std::string const msg = message::getMsgOrigin( cmdUser )  + " KICK " + chan.getName()
        + " " + kickedUser + " :" + reason + "\r\n";
    return ( msg );
}

static bool kickOperation( Server const & server, Client const & client, Channel & chan,
        Client const & kickedUser, std::string const & reason  ) {

    if ( !chan.isOperator( client.getNick() ) ) {
        if ( !chan.isMember( client.getNick() ) )
            server.sendMsg( ERR_NOTONCHANNEL, client, chan.getName(), "You're not on that channel" );
        else
            server.sendMsg( ERR_CHANOPRIVSNEEDED, client, chan.getName(), "You're not channel operator" );
        return ( false );
    }

    std::string response;
    if ( reason.length() )
        response = kickResponse( chan, client, kickedUser.getNick(), reason );
    else
        response = kickResponse( chan, client, kickedUser.getNick());

    if ( !chan.kickUser( kickedUser.getNick(), response ) ) {
        server.sendMsg( ERR_USERNOTINCHANNEL, client, kickedUser.getNick() + NBSP + chan.getName(), "They aren't on that channel" );
        return ( false );
    }
    server.sendMsg( chan, response );


    return ( true );
}

void Server::_kickCommand( Client const & client, std::string const & msg ) {
    typedef const std::vector<std::string> string_vec;

    string_vec chanList = message::extractArgs( msg, "#" );
    string_vec nickList = message::extractNickList( msg );
    string_vec reasonList = message::extractArgs( msg, ":" );

    string_vec::const_iterator chan_it = chanList.begin();
    string_vec::const_iterator nick_it = nickList.begin();
    string_vec::const_iterator reason_it = reasonList.begin();

    if ( chanList.size() != nickList.size() ) {
        sendMsg( ERR_NEEDMOREPARAMS, client, "KICK", "Not enough parameters" );
        DEBUG_MSG( "Error: KICK command" );
        return ;
    }

    for ( ; chan_it != chanList.end(); chan_it++ ) {
        std::map<std::string, Channel>::iterator cur_chan_it = _channels.find( "#" + *chan_it );
        if ( cur_chan_it == _channels.end() ) {
            sendMsg( ERR_NOSUCHCHANNEL, client, "#" + *chan_it, "No such channel" );
            continue;
        }
        Channel & cur_chan = cur_chan_it->second;
        std::string const cleanedReason = reason_it->substr( 0, reason_it->length() - 2 );
        Client const * user = _findClientByNick( _clientMap, *nick_it );
        if ( !user )
            sendMsg( ERR_NOSUCHNICK, client, *nick_it, "No such nick" );
        else
            kickOperation( *this, client, cur_chan, *user, cleanedReason );
        nick_it++;
        reason_it++;
    }
}
