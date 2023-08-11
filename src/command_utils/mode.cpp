#include "Server.hpp"
#include "Replies.hpp"

Channel & getChannel( Server const & serv, chan_map & chans, Client const & client, std::string const & msg ) {
    std::vector<std::string> chanName_vec = message::extractArgs( msg, "#" );
    if ( chanName_vec.size() != 1 ) {
        serv.sendMsg( ERR_NEEDMOREPARAMS, client, "MODE", "Not enough parameters" );
        throw ( std::runtime_error( "Not enough parameters" ) );
    }

    chan_map::iterator chan_it = chans.find( "#" + *chanName_vec.begin() );
    if ( chan_it == chans.end() ) {
        serv.sendMsg( ERR_NOSUCHCHANNEL, client, *chanName_vec.begin(), "No such channel" );
        throw ( std::runtime_error( "Can't find channel with this name" ) );
    }
    return ( chan_it->second );
}

int findModeType( char c ) {
    std::string const types = "itko";
    for ( int j = 0; types[j]; j++ ) {
        if ( types[j] == c )
            return ( j );
    }

    return ( -1 );
}

void Server::_modeCommand( Client const & client, std::string const & msg ) {
    if ( msg.find( "#" ) == msg.npos )
        return ;
    try {
        int modeNum;
        bool modeType;
        void ( Channel::*funPtr[4] )( bool ) = { &Channel::setInviteMode, &Channel::setTopicMode, &Channel::setKeyMode };
        Channel & chan = getChannel( *this, _channels, client, msg );

        if ( !chan.isOperator( client.getNick() ) ) {
            sendMsg( ERR_CHANOPRIVSNEEDED, client, chan.getName(), "You're not channel operator" );
            throw std::runtime_error( "User isn't a channel operator" );
        }

        for ( size_t i = msg.find_last_of( " " ); msg[i]; i++ ) {
            if ( msg[i] == '+' )
                modeType = true;
            else if ( msg[i] == '-' )
                modeType = false;
            else if ( ( modeNum = findModeType( msg[i] ) ) != -1 ) {
                (chan.*funPtr[modeNum])(modeType);
                // sendMsg( chan, message::getMsgOrigin( client ), msg );
                // sendMsg( RPL_CHANNELMODEIS, client, chan.getName() + " invite_only +i" );
            }
        }
    } catch ( std::exception const & e ) {
        DEBUG_MSG( e.what() << std::endl );
    }
}
