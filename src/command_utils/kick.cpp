#include "Server.hpp"

// S <-   :dan!d@localhost KICK #test alice :dan
static std::string const kickeResponse( Channel const & chan, Client const & kickedUser, Client const & cmdUser ) {
    std::string const msg = ":" + cmdUser.getNick() + "!" + cmdUser.getUser() + "@10.18.201.76 KICK " +  chan.getName() + " " + kickedUser.getNick() + " :" + cmdUser.getNick() + "\r\n";
    std::cout << "KICK RESPONSE [" << msg.substr( 0, msg.length() - 1 ) << "]" << std::endl;;
    return ( msg );
}

// Parameters: <channel> <user> [<reason>]
// command : KICK #channel user :reason
void Server::_kickCommand( Client const & client, std::string const & msg ) {
    // TODO: check user priv
    // TODO: add the reason part to the response message
    typedef std::vector<std::string const> cstring_vec;

    cstring_vec chanList = message::extractArgs( msg, "#" );
    cstring_vec nickList = message::extractNickList( msg );
    cstring_vec reasonList = message::extractArgs( msg, ":" );

    cstring_vec::iterator chan_it = chanList.begin();
    cstring_vec::iterator  nick_it = nickList.begin();
    cstring_vec::iterator reason_it = reasonList.begin();

    for ( ; chan_it != chanList.end(); chan_it++ ) {
        std::string chan_name = "#" + *chan_it;
        std::map<std::string, Channel>::iterator currChan_it = _channels.find( chan_name );
        if ( currChan_it != _channels.end() ) {
            std::string const msgResponse = kickeResponse( currChan_it->second, currChan_it->second.findClient( *nick_it ), client );
            if ( currChan_it->second.kickUser( *nick_it, msgResponse ) )
                _sendMessage( currChan_it->second, msgResponse );
        }

        if ( nick_it != nickList.end() )
            nick_it++;
        if ( reason_it != reasonList.end() )
            reason_it++;
    }
}
