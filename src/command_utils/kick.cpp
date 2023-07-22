#include "Server.hpp"

static bool findIndicies( std::string const & msg, size_t * firstHash, size_t * firstSpace, size_t * secondSpace ) {
    *firstHash = msg.find( "#" );
    *firstSpace = msg.find( " ", *firstHash );
    *secondSpace = msg.find( " ", *firstSpace + 1 );

    if ( *firstHash == std::string::npos || *firstSpace == std::string::npos
        || *secondSpace == std::string::npos ) {
        std::cerr << "Error: executing kick command" << std::endl;
        return false;
    }

    return ( true );
}

std::vector<std::string const> split(std::string const & str, std::string const & delimiter) {
    std::vector<std::string const> v;
    if ( !str.empty() ) {
        size_t start = 0;
        do {
            size_t idx = str.find(delimiter, start);
            if (idx == string::npos) {
                break;
            }
 
            size_t length = idx - start;
            v.push_back(str.substr(start, length));
            start += ( length + delimiter.size() );
        } while ( true );
        v.push_back( str.substr( start ) );
    }
    return ( v );
}

// Parameters: <channel> <user> [<comment>]
// command : KICK #channel user :comment
void Server::_kickCommand( Client const & client, std::string const & msg ) {
    (void)client;
    typedef std::vector<std::string const> cstring_vec;

    size_t firstHash, firstSpace, secondSpace;
    if ( !findIndicies( msg, &firstHash, &firstSpace, &secondSpace ) )
        return ;

    std::string const channelName = msg.substr( firstHash, firstSpace - firstHash );
    std::cout << "channelName: [" << channelName << "]" << std::endl;
    std::string const username = msg.substr( firstSpace + 1, secondSpace - firstSpace  - 1 ); // user to be kicked
    std::cout << "username: [" << username << "]" << std::endl;
    std::string const kickComment = msg.substr( secondSpace + 1, msg.length() - secondSpace  - 1 );
    std::cout << "kickComment: [" << kickComment << "]" << std::endl;


    cstring_vec channels_vec = split( channelName, "," );
    cstring_vec usernames_vec = split( username, "," );
    cstring_vec comments_vec = split( kickComment, "," );

    for ( cstring_vec::iterator chanName_it = channels_vec.begin();
    chanName_it != channels_vec.end(); chanName_it++ ) {
        cstring_vec::iterator usernames_vec_it = usernames_vec.begin();
        cstring_vec::iterator comments_vec_it = comments_vec.begin();
        chan_map::iterator chan_it = _channels.find( *chanName_it );

        // TODO: check if it reached the limit of users or comments
        // TODO: Clean the code
        if ( chan_it != _channels.end() ) {
            Channel foundChannel = chan_it->second;
            if ( !foundChannel.removeUser( *usernames_vec_it ) ) {
                for ( std::map<int, Client>::iterator it = _clientMap.begin(); it != _clientMap.end(); it++ ) {
                    std::cout << "----------> [" << it->second.getNick() << "] <-------------" << std::endl;
                    if ( it->second.getNick() == *usernames_vec_it ) {
                        // _sendMessage( it->second, "test msg test msg" );
                        // S <-   :dan!d@localhost KICK #Melbourne alice :dan
                        _sendMessage( it->second, ":" + client.getNick() + "!" + client.getUser()
                            + "@localhost KICK " + *chanName_it + " " + it->second.getNick() + ":" + client.getNick() + "\r\n" );
                        // _sendMessage( );
                        break ;
                    }
                }
            }
            else {
                std::cerr << "Error: Can't find user [" << *usernames_vec_it << "] in channel ["
                    << *chanName_it << std::endl;
            }

        } else {
            std::cerr << "Error: Can't find channel [" << *chanName_it << "]" << std::endl;
        }
        if ( usernames_vec_it != usernames_vec.end() )
            usernames_vec_it++;
        if ( comments_vec_it != comments_vec_it )
            comments_vec_it++;
    }
}

// ChanVector::iterator it = _channels.begin();
// for ( ; it != _channels.end(); it++ ) {
//     if ( channelName == it->getName() )
//         break ;
// }
// if ( it == _channels.end() ) {
//     std::cerr << "Error: can't find channel" << std::endl;
//     return ;
// }
