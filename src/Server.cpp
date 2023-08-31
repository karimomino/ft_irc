#include "Server.hpp"

/**
 * @brief Construct a new Server object and request initializing all commands
 * 
 * @param port > port to listen on
 * @param pass > server password
 */
Server::Server( int port, const std::string& pass ) : _serverEnd(false), _port(  port ), _pass( pass ) {
    _hint.sin_family = AF_INET;
    _hint.sin_port = htons(port);
    _hint.sin_addr.s_addr = INADDR_ANY;
    _initCmds();
}

/**
 * @brief Deallocate ALL dynamic memory allocated inside the Server object then DESTROY the Server object.
 * 
 */
Server::~Server( void ) {
    std::map<int, AClient*>::iterator client_it;
    std::map<const std::string, Channel*>::iterator chan_it;
    std::map<const std::string, ICommand*>::iterator cmd_it;

    for (client_it = _clients.begin() ; client_it != _clients.end() ; client_it++)
        delete _clients[client_it->first];

    for (chan_it = _channels.begin() ; chan_it != _channels.end() ; chan_it++)
        delete _channels[chan_it->first];

    for (cmd_it = _cmds.begin() ; cmd_it != _cmds.end() ; cmd_it++)
        delete _cmds[cmd_it->first];
}

/**
 * @brief This function will create instances of all commands supported by the server and adds 
 * them to the `_cmds` map to be used later
 * 
 * @note All commands are to be instantiated once and re-used everytime the command needs to be executed
 * 
 */
void Server::_initCmds( void ) {
    _cmds.insert( std::pair<const std::string, ICommand*>( "JOIN", new Join( *this ) ) );
    _cmds.insert( std::pair<const std::string, ICommand*>( "KICK", new Kick( *this ) ) );
    _cmds.insert( std::pair<const std::string, ICommand*>( "MODE", new Mode( *this ) ) );
    _cmds.insert( std::pair<const std::string, ICommand*>( "TOPIC", new Topic( *this ) ) );
    _cmds.insert( std::pair<const std::string, ICommand*>( "INVITE", new Invite( *this ) ) );
    _cmds.insert( std::pair<const std::string, ICommand*>( "PASS", new Pass( *this ) ) );
    _cmds.insert( std::pair<const std::string, ICommand*>( "USER", new User( *this ) ) );
    _cmds.insert( std::pair<const std::string, ICommand*>( "NICK", new Nick( *this ) ) );
    _cmds.insert( std::pair<const std::string, ICommand*>( "PRIVMSG", new PrivMsg( *this ) ) );
    _cmds.insert( std::pair<const std::string, ICommand*>( "NOTICE", new Notice( *this ) ) );
    _cmds.insert( std::pair<const std::string, ICommand*>( "PING", new Ping( *this ) ) );
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
    tmp_fd.events = POLLIN | POLLOUT;
    tmp_fd.revents = 0;
    _pollFds.push_back(tmp_fd);

    _connectionCount = 1;

    _ip = inet_ntoa(_hint.sin_addr);
    std::cout << BOLDGREEN << "Successfuly created listening socket!" << RESET << std::endl;
}

/**
 * @brief Once the Listening Socket and the commands are initialized use this function 
 * @brief to handle new connections and send/recieve msgs. This function needs a vector of FDS
 * @brief to keep track of the active clients and closed clients (_pollFds)
 * 
 * @note This functio will monitor 2 events:
 * @note [POLLIN] - data can be read/recieved from the client without blocking
 * @note     - once this event is detected, the server attempts to read data using _handleClientRecv
 * @note     - if recv returns 0 -> client closed their side of the connection -> purge client
 * @note     - if recv > 0 -> parse the data -> add msg to the queue of the client
 * @note  
 * @note [POLLOUT] - data can be sent to the client without blocking
 * @note     - once this event is detected if there are messages in the queue -> attempt to send them
 * 
 */
void Server::run( void ) {

    while (!_serverEnd) {
        poll(_pollFds.data(), _pollFds.size(), -1);

        for (size_t i = 0; i < _pollFds.size(); i++) {
            if (_pollFds[i].revents == 0)
                continue;

            int fd = _pollFds[i].fd;
            if ( fd == _socketFd)
                _handlePreClientReg();
            else if (_pollFds[i].revents & POLLOUT && fd != _socketFd && _clients[fd]->getQueueSize() )
                _handleClientSend( fd );
            else if (_pollFds[i].revents & POLLIN && fd != _socketFd)
                _handleClientRecv( fd );

            _pollFds[i].revents = 0;
        }
    }
}

/**
 * @brief When poll detects a change in the listening socket revents-> create a client. 
 * @brief This client is later upgraded to a real client once registration is completed.
 * 
 * @note Once the client is created is it added to the _clients map
 * @note     - POLLIN and POLLOUT need to be monitored to handle data
 * @note     - The fd of the client needs to be added to the _pollFds vector to monitor the events
 */
void Server::_handlePreClientReg( void ) {
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
        _purgeClient(newClient->getSocketFd());
    }
}


/**
 * @brief This function is triggered when poll detects that the client is 
 * @brief ready to to be sent data and the client has messages in the queue.
 * 
 * @param socket -> the fd of the client
 * 
 * @note using the socket param, we find the client in the map to be able to 
 * end the first message in the queue.
 * @note If the client is triggered to be purged and there are no more messages in the queue:
 * @note     - purge the client from the server
 * 
 */
void Server::_handleClientSend(int socket) {
    AClient &cli= *_clients[socket];

    std::cout << "[ " << GREEN << "ATTEMPTING TO SEND - " << cli.getNick() << " " << RESET << "]" << std::endl;
    std::cout << cli.getFirstMsg() << std::endl;
    std::cout << "[ " << GREEN << "END ATTEMPT SEND " << RESET << "]\n" << std::endl;

    cli.sendMSg();
    if (cli.getPurge() && !cli.getQueueSize())
        _purgeClient( socket );
}

/**
 * @brief This function will remove all traces of a client from the server
 * 
 * @param fd -> the fd of the client to purge
 * 
 * @note Steps:
 * @note     1 - deallocating the dynamically allocated client inside _clients map using `fd` param
 * @note     2 - removing the client entry from the _clients map using the `fd` param
 * @note     3 - removing the fd from the _pollFds vector to exclude form polling
 * 
 */
void Server::_purgeClient(const int& fd) {
    std::vector<pollfd>::iterator poll_it;
    const std::string clientNick = _clients[fd]->getNick();
    const std::vector<std::string> chanList = _clients[fd]->getChannels();
    delete _clients[fd];
    _clients.erase(fd);
    for ( std::vector<std::string>::const_iterator it = chanList.begin(); it != chanList.end(); it++ ) {
        std::map<const std::string, Channel*>::iterator chan_it = _channels.find( *it );
        chan_it->second->removeUser( clientNick );
        if ( !chan_it->second->getUsersCount() )
            removeChannel( *it );
    }

    for (poll_it = _pollFds.begin(); poll_it != _pollFds.end(); poll_it++) {
        if (poll_it->fd == fd ) {
            _pollFds.erase(poll_it);
            break;
        }
    }
}

static std::string findCommands(std::string& fullMsg , std::string input) {
    size_t firstNL = input.find("\n");
    size_t firstRNL = input.find("\r\n");

    while ( firstNL != std::string::npos || firstRNL != std::string::npos ) {
        if ( firstNL < firstRNL ) {
            fullMsg += input.substr(0 , firstNL) + "\r\n";
            input.erase(0 , firstNL + 1);
        } else {
            fullMsg += input.substr(0 , firstRNL + 2);
            input.erase(0 , firstRNL + 2);
        }
        firstRNL = input.find("\r\n", 1);
        firstNL = input.find("\n", 1);
    }
    return (input);
}

/**
 * @brief This function is triggered when poll detects that the client is 
 * @brief ready to send data to the server.
 * 
 * @param socket > the fd of the client
 * 
 * @note Using the socket param, attempt to recieve data from the fd and store data in `buff`.
 * @note  
 * @note If recv return 0 -> purge the client right away. Since the client closed their half of the connection 
 * the client is not interested in recieving any more message from the server.
 * @note  
 * @note If recv returns a value > 0 -> get all the data available and execute all messages 
 * in order they are recieved.
 * 
 */
void Server::_handleClientRecv(const int& socket) {
    std::string fullMsg;
    char buff[1024];

    memset(buff, 0, 1024);
    int rcv = recv(socket, buff, 1024, MSG_DONTWAIT);

    if (rcv > 0 ) {
        _clients[socket]->partialCmd += buff;
        memset(buff, 0, 1024);
        do
        {
            rcv = recv(socket, buff, 1024, MSG_DONTWAIT);
            _clients[socket]->partialCmd += buff;
            memset(buff, 0, 1024);
        } while (rcv > 0);

        std::cout << "[" << BLUE << "RECEIVED" << RESET << "]" <<std::endl << _clients[socket]->partialCmd << std::endl;
        std::cout <<"[" << BLUE << "END RECEIVE" << RESET << "]\n" << std::endl;

        _clients[socket]->partialCmd = findCommands(fullMsg ,_clients[socket]->partialCmd);
        execCommand( *this , fullMsg , _clients[socket]);
    } else if ( rcv == 0) {
        std::cout << "sending purge" << std::endl;
        _purgeClient(socket);
    }
}

/**
 * @brief This function, as the name suggest, will find a client by nickname not fd in _clients map.
 * this is needed since clients are stored in _clients with the fd as the key.
 * 
 * @param nick > client nickname we are trying to find
 * @return AClient* -> `NULL` if not found.
 */
AClient* Server::_findClientByNick( const std::string& nick ) const {
    for ( std::map<int, AClient*>::const_iterator it = _clients.begin(); it != _clients.end(); it++ ) {
        if ( it->second->getNick() == nick )
            return ( it->second );
    }
    return ( NULL );
}

/**
 * @brief This function is used in `_execCommand` function as a helper to find the command name to execute & its args.
 *
 * @param rawCommand > full command message recieved from the client
 * @return std::pair<std::string const , std::string>
 * @note `pair.first` = command name | `pair.second` = command arguments
 */
static std::pair<std::string const , std::string> extractCommand( std::string rawCommand ) {
    std::stringstream ss(rawCommand);
    std::string command;
    std::string commandArgs;

    getline(ss , command , ' ');
    getline(ss , commandArgs);
    return (std::make_pair(command , commandArgs));
}

/**
 * @brief This function will execute each command recieved from the client in the order it came in
 * if its allowed by the server
 * 
 * @param ircServ > server object since this function is a `friend` and not inside the server class
 * @param clientMsg > command message(s) received fromt he client (could be multiple at the same time)
 * @param cli > the client that sent the command message(s)
 * 
 * @note Since the client can send multiple commands in 1 go, we have to use split to execute each command
 */
void execCommand( Server& ircServ , std::string clientMsg , AClient* cli) {
    std::vector<std::string> commands = utils::split(clientMsg , "\r\n");

    for (std::vector<std::string>::iterator it = commands.begin(); it != commands.end(); it++) {
        std::pair<std::string , std::string> cmdParts = extractCommand(*it);

        std::map<const std::string, ICommand*>::iterator cmd_it = ircServ._cmds.find( cmdParts.first );
        if ( cmd_it != ircServ._cmds.end() )
            ircServ._cmds[cmdParts.first]->execute( cli , cmdParts.second);
    }
}

/**
 * @brief This function will promote a pre-client to a full client after registration is complete
 * 
 * @param client > client to promote
 * 
 * @note We dont need to add client fd to the `_pollFds` vector since it was already added at the pre-client stage
 */
void Server::_addClient( const AClient* client ) {
    Client *newClient = new Client(*client);
    _clients.insert(std::make_pair(client->getSocketFd() , newClient));
}

/**
 * @brief This function will create a new channel and add it to `_channels` map
 * 
 * @param name > name of the channel to create
 * @param topic > topic of the channel to create
 */
void Server::_addChannel( const std::string& name , const std::string& topic ) {
	Channel *newChan = new Channel( *this, name , topic );
	_channels.insert( std::make_pair( name , newChan ) );
}

/**
 * @brief This function will check if a nickname is being used by a client already registered 
 * on the server.
 * 
 * @param nick > nickname to check
 * @return `true` if the nickname is already in use | 
 * @return `false` if the nickname is not already in use
 * 
 * @note This is used when a `pre-client` or `client` attempts to set their nickname
 */
bool Server::nickInUse ( const std::string& nick ) const {
    bool nickInUse = false;

    for (std::map<int, AClient*>::const_iterator it = _clients.begin(); it != _clients.end(); it++) {
        if (!(*it).second->getNick().compare(nick))
            nickInUse = true;
    }
    return (nickInUse);
}

/**
 * @brief This function deallocates the channel stored in the `_channels` map and removes
 * the entry from the map.
 * 
 * @param name > name of the channel to remove.
 */
void Server::_removeChannel( const std::string& name ) {
    delete _channels[name];
    _channels.erase(name);
}

/**
 * @brief This function should be called in the `main` function when the server is flagged to shutdown. It will deallocate any 
 * dynamic memory allocated.
 * 
 */
void Server::exit( int sigNum ) {
    if (sigNum == SIGINT)
        _serverEnd = true;
}

void Server::removeChannel( const std::string& chanName ) {
    std::map<const std::string, Channel*>::iterator chan_it;
    if ( ( chan_it = _channels.find( chanName ) ) != _channels.end() ) {
        delete chan_it->second;
        _channels.erase( chan_it );
    }
}

/**
 * @brief 
 * 
 * @return `const int&` const reference to the listening socket fd 
 */
const int& Server::getSockFd() const { return (_socketFd); }

/**
 * @brief 
 * 
 * @return `const std::string&` const reference to the password of the server
 */
const std::string& Server::getPass( void ) const { return ( _pass ); }

/**
 * @brief 
 * 
 * @return `const std::string&` const reference to the IP of the server
 */
const std::string& Server::getIp( void ) const { return ( _ip ); }

/**
 * @brief Construct a new Server Error when we need to throw any server related errors
 * 
 * @param msg > e.what() message retrieved in the catch block
 */
Server::ServerError::ServerError( const char * msg ) : std::runtime_error( msg ) { (void) msg; }
