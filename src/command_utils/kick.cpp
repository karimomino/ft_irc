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
        std::string const & kickedNick, std::string const & reason  ) {

    if ( !chan.isOperator( client.getNick() ) ) {
        if ( !chan.isMember( client.getNick() ) )
            server.sendMsg( ERR_NOTONCHANNEL, client, chan.getName(), "You're not on that channel" );
        else
            server.sendMsg( ERR_CHANOPRIVSNEEDED, client, chan.getName(), "You're not channel operator" );
        return ( false );
    }

    std::string response;
    if ( reason.length() )
        response = kickResponse( chan, client, kickedNick, reason );
    else
        response = kickResponse( chan, client, kickedNick);

    if ( !chan.kickUser( kickedNick, response ) ) {
        server.sendMsg( ERR_USERNOTINCHANNEL, client, kickedNick + NBSP + chan.getName(), "They aren't on that channel" );
        return ( false );
    }
    server.sendMsg( chan, response );


    return ( true );
}

void Server::_kickCommand( Client const & client, std::string const & msg ) {
    typedef std::vector<std::string const> cstring_vec;

    cstring_vec chanList = message::extractArgs( msg, "#" );
    cstring_vec nickList = message::extractNickList( msg );
    cstring_vec reasonList = message::extractArgs( msg, ":" );

    cstring_vec::iterator chan_it = chanList.begin();
    cstring_vec::iterator nick_it = nickList.begin();
    cstring_vec::iterator reason_it = reasonList.begin();

    if ( chanList.size() != nickList.size() ) {
        // TODO: get the error code and send back to client
        sendMsg( ERR_NEEDMOREPARAMS, client, "KICK", "Not enough parameters" );
        DEBUG_MSG( "Error: KICK command" );
        return ;
    }

    for ( ; chan_it != chanList.end(); chan_it++ ) {
        std::map<std::string, Channel>::iterator cur_chan_it = _channels.find( "#" + *chan_it );
        if ( cur_chan_it == _channels.end() ) {
            // TODO: get the error code and send back to client
            sendMsg( ERR_NOSUCHCHANNEL, client, "#" + *chan_it, "No such channel" );
            continue;
        }
        Channel & cur_chan = cur_chan_it->second;
        std::string const cleanedReason = reason_it->substr( 0, reason_it->length() - 2 );
        if ( kickOperation( *this, client, cur_chan, *nick_it, cleanedReason ) ) {}
        nick_it++;
        reason_it++;
    }
}
