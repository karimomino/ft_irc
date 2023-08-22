#include "Server.hpp"

Server::Server( int port, const std::string& pass ) : _serverEnd(false), _port(  port ), _pass( pass ) {
    _hint.sin_family = AF_INET;
    _hint.sin_port = htons(port);
    _hint.sin_addr.s_addr = INADDR_ANY;
    _initCmds();
}

Server::~Server( void ) {
    // TODO: clean up all the clients
}

void Server::_initCmds( void ) {
    _cmds.insert( std::pair<const std::string, ICommand*>( "JOIN", new Join( *this ) ) );
    _cmds.insert( std::pair<const std::string, ICommand*>( "KICK", new Kick( *this ) ) );
    _cmds.insert( std::pair<const std::string, ICommand*>( "MODE", new Mode( *this ) ) );
    _cmds.insert( std::pair<const std::string, ICommand*>( "TOPIC", new Topic( *this ) ) );
    _cmds.insert( std::pair<const std::string, ICommand*>( "INVITE", new Invite( *this ) ) );
    _cmds.insert( std::pair<const std::string, ICommand*>( "PASS", new Pass( *this ) ) );
    _cmds.insert( std::pair<const std::string, ICommand*>( "USER", new User( *this ) ) );
    _cmds.insert( std::pair<const std::string, ICommand*>( "NICK", new Nick( *this ) ) );
}

/**
 * @brief Creating a listening non-blocking socket for the server to listen on.
 * @brief Then preparing the socket for Polling. After constructing a server, call this function in 
 * the main before running the server through Server::run().
 * 
 * @note ServerErrors are thrown if:
 * @note 1 - Failed to create server socket
 * @note 2 - Failed to set socket options
 * @note 3 - Failed to set socket non-blocking
 * @note 4 - Failed to bind local socket to descriptor socket
 * @note 5 - Failed to listen on socket
 * 
 */
void Server::init( void ) {
    int sock_opt = 1;

    if ( ( _socketFd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
        throw ( ServerError( "Error creating the server socket") );
    if ( setsockopt( _socketFd, SOL_SOCKET, SO_REUSEADDR, &sock_opt, sizeof( sock_opt ) ) )
        throw ( ServerError( "Error setting socket options" ) );
    if ( fcntl( _socketFd, F_SETFL, O_NONBLOCK ) == -1 )
        throw ( ServerError( "Error setting socket to non-blocking" ) );
    if ( bind( _socketFd, ( sockaddr* )&_hint, sizeof( _hint) ) < 0 )
        throw ( ServerError( "Error binding" ) );
    if (listen( _socketFd, MAX_CLIENTS))
        throw ( ServerError( "Failed to listen" ) );

    pollfd tmp_fd;
    tmp_fd.fd = _socketFd;
    tmp_fd.events = POLLIN | POLL_OUT;
    tmp_fd.revents = 0;
    _pollFds.push_back(tmp_fd);

    _connectionCount = 1;

    _ip = inet_ntoa(_hint.sin_addr);
    std::cout << BOLDGREEN << "Successfuly created listening socket!" << RESET << std::endl;
}

void Server::run( void ) {
    pollfd* pFdsData;
    while (!_serverEnd) {
        pFdsData = _pollFds.data();

        poll(pFdsData, _pollFds.size(), -1);

        for (size_t i = 0; i < _pollFds.size(); i++) {
            if (pFdsData[i].revents == 0)
                continue;

            int fd = pFdsData[i].fd;

            if ( fd == _socketFd)
                _handlePreClientReg();

            if (pFdsData[i].revents & POLLOUT && fd != _socketFd)
                _handleClientSend( fd );

            if (pFdsData[i].revents & POLLIN && fd != _socketFd)
                _handleClientRecv( fd );

            pFdsData[i].revents = 0;
        }

    }
}

void Server::_handlePreClientReg() {
    PreClient* newClient = new PreClient(_socketFd , &_hint);
    _clients[newClient->getSocketFd()] = newClient;
    if (_connectionCount < MAX_CLIENTS) {
        pollfd tmp_fd;
        tmp_fd.fd = newClient->getSocketFd();
        tmp_fd.events = POLLIN | POLLOUT;
        tmp_fd.revents = 0;
        _pollFds.push_back(tmp_fd);

    } else {
        std::cout << "[" << BOLDRED << "CONNECTION REFUSED" << RESET << "] " << "@" << newClient->getIp() << " Max Clients Reached!" << std::endl;
        close(newClient->getSocketFd());
        _preClients.pop_back();
    }
}

void Server::_handleClientSend(const int& socket) {
    AClient &cli= *_clients[socket];
    cli.sendMSg();
}

void Server::_handleClientRecv(const int& socket) {
    std::string fullMsg;
    char buff[1024];

    memset(buff, 0, 1024);
    int rcv = recv(socket, buff, 1024, MSG_DONTWAIT);

    if (rcv > 0 ) {
        fullMsg += buff;
        memset(buff, 0, 1024);
        do
        {
            rcv = recv(socket, buff, 1024, MSG_DONTWAIT);
            fullMsg += buff;
            memset(buff, 0, 1024);
        } while (rcv > 0);

        std::cout << "[" << BLUE << "RECEIVED" << RESET << "]" <<std::endl << fullMsg << std::endl;
        std::cout <<"[" << BLUE << "END RECEIVE" << RESET << "]" << std::endl;

        execCommand( *this , fullMsg , _clients[socket]);
    } else if ( rcv == 0) {
        //client closed connection
    }
}

AClient* Server::_findClientByNick( const std::string& nick ) const {
    for ( std::map<int, AClient*>::const_iterator it; it != _clients.end(); it++ ) {
        if ( it->second->getNick() == nick )
            return ( it->second );
    }
    return ( NULL );
}

static std::pair<std::string const , std::string> extractCommand( std::string rawCommand ) {
    std::stringstream ss(rawCommand);
    std::string command;
    std::string commandArgs;

    getline(ss , command , ' ');
    // commandArgs = ss.str();
    getline(ss , commandArgs);
    return (std::make_pair(command , commandArgs));
}

static bool isValidCommand( std::string cmd ) {
    bool isValid = false;
    std::string cmdList[] = {"JOIN" , "INVITE" , "KICK" , "MODE" , "PASS", "TOPIC", "USER" , "PRIVMSG" , "NOTICE" , "NICK"};
    for (size_t i = 0; i < sizeof(cmdList)/sizeof(cmdList[0]); i++)
    {
        if ( !cmdList[i].compare(cmd) ) {
            isValid = true;
            break;
        }
    }
    return ( isValid );
}

void execCommand( Server& ircServ , std::string clientMsg , AClient* cli) {
    std::vector<std::string> commands = utils::split(clientMsg , "\r\n");

    for (std::vector<std::string>::iterator it = commands.begin(); it != commands.end(); it++) {
        std::pair<std::string , std::string> cmdParts = extractCommand(*it);

        if ( isValidCommand(cmdParts.first) )
            ircServ._cmds[cmdParts.first]->execute( cli , cmdParts.second);
    }
}

void Server::_addClient( const AClient* client ) {
    Client *newClient = new Client(*client);
    _clients.insert(std::make_pair(client->getSocketFd() , newClient));
}

// void Server::_addChannel( void ) {}

// void Server::_removeClient( void ) {}

// void Server::_removeChannel( void ) {}

void Server::exit( void ) {
    // TODO: clean up before the destructor is called
}

const int& Server::getSockFd() const { return (_socketFd); }

const std::string& Server::getPass( void ) const { return ( _pass ); }

const std::string& Server::getIp( void ) const { return ( _ip ); }

Server::ServerError::ServerError( const char * msg ) : std::runtime_error( msg ) { (void) msg; }
