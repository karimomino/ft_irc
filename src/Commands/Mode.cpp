#include "Commands/Mode.hpp"
#include "AClient.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "utils.hpp"

Mode::Mode( Server& ircServ ) : ICommand( ircServ ), _chan( NULL ), _client( NULL ) {
    _mPtr.insert( std::make_pair( 'i', &Mode::inviteMode ) );
    _mPtr.insert( std::make_pair( 't', &Mode::topicMode ) );
    _mPtr.insert( std::make_pair( 'k', &Mode::keyMode ) );
    _mPtr.insert( std::make_pair( 'o', &Mode::opMode ) );
}

Mode::~Mode( void ) {}

std::vector<std::string> Mode::validateArgs( AClient* client, const std::string& rawCommand ) {
    std::vector<std::string> args = utils::split( rawCommand, " " );
    if ( args.size() < 2 )
        throw ( std::runtime_error( ERR_NEEDMOREPARAMS( _ircServ.getIp(), client->getNick() + " MODE" ) ) );

    std::map<const std::string, Channel*>::iterator chan_it = _ircServ._channels.find( args[0] );
    if ( chan_it == _ircServ._channels.end() )
        throw ( std::runtime_error( ERR_NOSUCHCHANNEL( _ircServ.getIp(),
            client->getNick() + " " +  args[0] ) ) );

    if ( !chan_it->second->isOperator( client->getNick() ) )
        throw ( std::runtime_error( ERR_CHANOPRIVSNEEDED(
            _ircServ.getIp(), client->getNick() + " " + chan_it->second->getName() ) ) );

    return ( args );
}

const std::string getChanName( const std::string& rawCommand ) {
    std::vector<std::string> args = utils::split( rawCommand, " " );
    return ( args[0] );
}

vec_pCS getModes( const std::string& rawCommand ) {
    std::vector<std::string> args = utils::split( rawCommand, " " );
    args.erase( args.begin() );
    std::string modes = args[0];
    args.erase( args.begin() );
    vec_pCS list;
    bool state = modes[0] == '-' ? true : false;

    std::string::iterator modes_it = modes.begin();
    std::vector<std::string>::iterator args_it = args.begin();
    while ( modes_it != modes.end() ) {
        if ( *modes_it == '+' && state != true ) {
            state = true;
            list.push_back( std::make_pair( '+', "" ) );
        } else if ( *modes_it == '-' && state != false ) {
            state = false;
            list.push_back( std::make_pair( '-', std::string("") ) );
        }
        else if ( std::string( "ok" ).find( *modes_it ) != std::string::npos && args_it != args.end() ) {
            list.push_back( std::make_pair( *modes_it, *args_it ) );
            args_it++;
        } else
            list.push_back( std::make_pair( *modes_it, "" ) );
        modes_it++;
    }
    return ( list );
}

bool Mode::inviteMode(  bool state, std::string& arg ) {
    _chan->setInviteMode( state );
    return ( true );
    (void)arg;
}

bool Mode::topicMode( bool state, std::string& arg ) {
    _chan->setTopicMode( state );
    return ( true );
    (void)arg;
}

bool Mode::keyMode( bool state, std::string& arg ) {
    if ( state == true && arg.empty() && arg.find( " " ) != std::string::npos ) {
        _client->addMsg( ERR_INVALIDKEY( _ircServ.getIp(),
            _client->getNick() + " " + _chan->getName() ) );
        return ( false );
    }

    if ( state )
        _chan->setKey( arg );
    else if ( arg.empty() )
        arg = _chan->getKey();
    else if ( arg != _chan->getKey() )
        return ( false );

    _chan->setKeyMode( state );
    return ( true );
}

bool Mode::opMode( bool state, std::string& arg ) {
    if ( state && arg.empty() )
        return ( false );
    else if ( !state && !_chan->isOperator( arg ) )
        return ( false );

    if ( !_chan->isMember( arg ) && !_chan->isOperator( arg ) ) {
        _client->addMsg( ERR_USERNOTINCHANNEL( _ircServ.getIp(),
            _client->getNick() + " " + arg + " " + _chan->getName() ) );
        return ( false );
    }
    if ( state )
        _chan->promoteClient( arg );
    else
        _chan->demoteClient( arg );
    return ( true );
}

pSS Mode::executeMode( vec_pCS modes ) {
    pSS response;
    bool state = modes.begin()->first == '-' ? true : false;

    for ( vec_pCS::iterator modes_it = modes.begin(); modes_it != modes.end(); modes_it++ ) {
        if ( std::string( "+-" ).find( modes_it->first ) != std::string::npos ) {
            bool oldState = state;
            state = modes_it->first == '+' ? true : false;
            if ( oldState != state )
                response.first += modes_it->first;
        }
        else {
            std::map<char, modePtr>::iterator fun = _mPtr.find( modes_it->first );
            if ( fun == _mPtr.end() )
                _client->addMsg( ERR_UMODEUNKNOWNFLAG( _ircServ.getIp(), _client->getNick() ) );
            else if ( (this->*fun->second)( state, modes_it->second ) ) {
                response.first += modes_it->first;
                response.second += modes_it->second + " ";
            }
        }
    }
    utils::trim( response.second, " " );
    if ( !response.first.empty() && std::string( "+-" ).find(
            response.first[ response.first.length() - 1] ) != std::string::npos )
        response.first.erase(response.first.end() - 1);
    return ( response );
}

void Mode::execute( AClient* client, const std::string & rawCommand ) {
    if ( rawCommand.find( "#" ) == std::string::npos )
        return ;
    try {
        validateArgs( client, rawCommand );
        _client = client;
        _chan = _ircServ._channels.find( getChanName( rawCommand ) )->second;
        vec_pCS  modes = getModes( rawCommand );
        pSS response = executeMode( modes );
        if ( !response.first.empty() ) {
            std::string msg = client->getOrigin() + " MODE " + _chan->getName() + " " + response.first;
            msg += response.second.empty() ? "\r\n" : " " + response.second + "\r\n";
            _chan->addMsg( "", msg );
        }
    } catch ( const std::exception& e ) {
        client->addMsg( e.what() );
    }
    clearCmd();
}

void Mode::clearCmd( void ) {
    _chan = NULL;
    _client = NULL;
}
