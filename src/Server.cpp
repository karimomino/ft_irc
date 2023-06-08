/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ommohame < ommohame@student.42abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 18:41:14 by kamin             #+#    #+#             */
/*   Updated: 2023/06/09 00:38:51 by ommohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstdio>
#include <cstring>
#include <iostream>

Server::Server(const int port, const std::string pass) : _port(port), _pass(pass) {
    _hint.sin_family = AF_INET;
    _hint.sin_port = htons(port);
    _hint.sin_addr.s_addr = INADDR_ANY;
    _connectionCount = 0;

    _initCommandsFunctions();

    if ( _initServer() )
        _runServer();
    else {
        perror( "Failed to init server." );
    }

    (void)_port;
}

Server::~Server( void ) {
    // TODO: free all the alloc memory for ptr to the functions
    std::map<std::string const, void *>::iterator it = _commandsFunctions.begin();

    for ( ; it != _commandsFunctions.end(); it++ )
        delete ( ( cmdFunPtr * )( it->second ));
}

bool    Server::_initServer( void ) {
    bool ret = true;
    int sock_opt = 1;
    pollfd  tmp_fd;
    // pollfd  *tmp_fd = new pollfd;

    _listen_socket = socket( AF_INET, SOCK_STREAM, 0 );
    if ( _listen_socket < 0 ) {
        std::cout << "Error creating socket!" << std::endl;
        ret = false;
    } else if ( setsockopt( _listen_socket, SOL_SOCKET, SO_REUSEADDR, &sock_opt,
                sizeof( sock_opt ))) {
        std::cout << "Error setting socket options!" << std::endl;
        ret = false;
    } else if ( fcntl( _listen_socket, F_SETFL, O_NONBLOCK ) == -1 ) {
        std::cout << "Error setting socket to non-blocking!" << std::endl;
        ret = false;
    } else if ( bind( _listen_socket, ( sockaddr * )&_hint, sizeof( _hint )) < 0 ) {
        std::cout << "Error binding!" << std::endl;
        ret = false;
    } else if ( listen( _listen_socket, MAX_CLIENTS )) {
        std::cout << "Failed to listen" << std::endl;
        ret = 0;
    }
    _connectionCount = 1;
    memset(&_poll_fds, 0, MAX_CLIENTS + 1);
    _poll_fds[0].fd = _listen_socket;
    _poll_fds[0].events = POLLIN;
    return (ret);
}

void Server::_runServer(void) {
    time_t now;
    struct pollfd *poll_fds = _poll_fds;

    // Server Loop
    while (true) {
        poll(poll_fds, MAX_CLIENTS + 1, 1000);

        for (size_t i = 0; i < _connectionCount; i++) {

            // if no change on this socket, run next iteration
            if (poll_fds[i].revents == 0)
                continue;

            int socket = (_poll_fds.data())[i].fd;

            // TODO: add check if returned something other than POLLIN error

            // now = time(0);
            // this is a new connection
            // TODO: loop to accept all incoming connections
            if (socket == _listen_socket) {
                _acceptConnection();
            } else {
                std::map<int, Client>::iterator tmp = _getClient(socket);

                // TODO: loop to receive bigger messages
                char buff[1024];
                memset(buff, 0, 1024);
                int rcv = recv(socket, buff, 1024, MSG_DONTWAIT);
                // std::cout << "receiving msg" << std::endl;
                if (rcv > 0) {
                    // tm *ltm = localtime(&now);
                    // std::cout << "[ " << 5 + ltm->tm_hour << ":" << ltm->tm_min << ":"
                    //                     << ltm->tm_sec << " ] " << "msg received from client: " << buff << std::endl;
                    // // TODO: Fix hour, its giving the wrong number
                    tmp->second.incMsgSent();
                    _parseMessage(tmp->second, buff);
                    // TODO : check if password matches
                } else if (rcv == 0) {
                    PollVector::iterator erase_pos = _poll_fds.begin() + ( i );
                    _poll_fds.erase( erase_pos );
                    _clientMap.erase(socket);
                    close(socket);
                    std::cout << "client connection closed" << std::endl;
                }
            }
        }
    }
}

std::string Server::_parseMessage(Client &client, char *buff) {

    std::vector<std::string> wordList = split_string(buff);
    std::vector<std::string>::iterator word_it = wordList.begin();
    std::string comm = (*word_it);

    if (!comm.compare("PASS")) {
        word_it++;
        client.setPass(*word_it);
        std::cout << "PASS changed" << std::endl;
    } else if (!comm.compare("NICK")) {
        word_it++;
        client.setNick(*word_it);
        std::cout << "Nickname changed" << std::endl;
    } else if (!comm.compare("USER")) {
        word_it++;
        client.setUser(*word_it);
        std::cout << "Username changed" << std::endl;
    } else if (!comm.compare("JOIN")) {
        // send(client.getClientSocket(), ":nickKooki!loginName@localhost JOIN #kk\r\n", 41,0x80);
        word_it++;
        std::string mes = ":" + client.getNick() + "!" + client.getUser() + "@localhost JOIN " +
        									(*word_it).substr(0, (*word_it).length() - 2) + "\r\n";
        send(client.getClientSocket(), mes.c_str(), mes.length(), 0x80);

        // // mes = ":127.0.0.1 332 " + client.getNick() + " " + (*word_it).substr(0, (*word_it).length() - 2) + " :some topic" + "\r\n";
        // // send(client.getClientSocket(), mes.c_str(), mes.length(), 0);
        std::cout << mes << std::endl;
        mes = ":127.0.0.1 MODE " + (*word_it).substr(0, (*word_it).length() - 2) + " +nt\r\n";
        send(client.getClientSocket(), mes.c_str(), mes.length(), 0);
        std::cout << mes << std::endl;
        // // MODE #test +nt
        mes = ":127.0.0.1 353 " + client.getNick() + " = " + (*word_it).substr(0, (*word_it).length() - 2) + " :@" + client.getNick() + "\r\n";
        send(client.getClientSocket(), mes.c_str(), mes.length(), 0);
        std::cout << mes << std::endl;

        mes = ":127.0.0.1 366 " + client.getNick() + " " + (*word_it).substr(0, (*word_it).length() - 2) + " :End of NAMES list\r\n";
        send(client.getClientSocket(), mes.c_str(), mes.length(), 0);
        std::cout << mes << std::endl;
    } else if (!comm.compare("PRIVMSG") && client.getRegisteredStatus()) {
        // std::string mes = ":nickKooki PRIVMSG #blah :hello himotha fucka\r\n";

        std::string mes = ":nickKooki!loginName@127.0.0.1 PRIVMSG #blah :im in blah\r\n";
        send(client.getClientSocket(), mes.c_str(), mes.length(), 0x80);
        mes = ":nickKooki PRIVMSG #karimm :im in karimmm\r\n";
        send(client.getClientSocket(), mes.c_str(), mes.length(), 0x80);

    } else if (!comm.compare("QUIT")) {
        _connectionCount--;
        close(client.getClientSocket());
    } 
    else {
        std::cout << buff << std::endl;
    }

    // if ( !client.getRegisteredStatus() )
    // {
    // 	std::cout << "Client is not registered yet. Please set passowrd, nick, and user to be able to use any commands." << std::endl;
    // }
    return ("	 ");
}

std::map<int, Client>::iterator Server::_getClient(const int fd) {
    return (_clientMap.find(fd));
}

int Server::_acceptConnection(void) {
    Client newClient = Client(_listen_socket, _hint);
    std::cout << "Current connection count " << _connectionCount << std::endl;
    _clientMap.insert(
            std::pair<int, Client>(newClient.getClientSocket(), newClient));
    if (_connectionCount < MAX_CLIENTS) {
        std::stringstream ss;
        std::string connectionMessage;

        _poll_fds[_connectionCount].fd = newClient.getClientSocket();
        _poll_fds[_connectionCount].events = POLLIN;

        // ss << ":127.0.0.1 001 "
        // 	 << "nickKooki :"
        // 	 << "Welcome to kamin's ft_irc "
        // 	 << "nickKooki. "
        // 	 << "This is MOTD\r\n";
        // connectionMessage = ss.str();
        // send(newClient.getClientSocket(), connectionMessage.c_str(),
        // 		 connectionMessage.length(), 0x80);
        _connectionCount++;
    } else {
        std::cout << "Connection Refused, MAX clients Reached" << std::endl;
        close(newClient.getClientSocket());
    }

    return (0);
}

struct pollfd *Server::getPollFds(void) {
    return (this->_poll_fds);
}

int Server::getListenSocket(void) { return (this->_listen_socket); }

size_t Server::getConnectionCount(void) { return (this->_connectionCount); }

// std::string	Server::
  

void    Server::_joinCommand( std::string const & command ) {
    std::cout << command << std::endl;
}


template <typename T>
bool Server::_addCommandFunction( std::string const & keyValue , T funPtr ) {
    if ( !funPtr )
        return ( false );

    T * allocPtr = new T;
    *allocPtr = funPtr;
    
    this->_commandsFunctions.insert(
            std::pair< std::string const, void *>( keyValue, allocPtr ) );

    return ( true );
}

bool Server::_initCommandsFunctions( void ) {
    _addCommandFunction( "JOIN", &Server::_joinCommand ); 

    return ( true );
}

void Server::_executeCommand( std::string const & message ) {
    size_t foundSpace = message.find( ' ' );
    std::string commandType = message.substr( 0, foundSpace );

    std::map< std::string const, void *>::iterator cmd = _commandsFunctions.find( commandType );

    // TODO:
    //      - maybe throw an exception if you can't find the command so the server can send back to the client
    //      - remove the print msg ..
    if ( cmd == _commandsFunctions.end() ) {
        std::cerr << "Can't find command" << std::endl;
        return ;
    }

    // TODO: find a way to detect the casting automatically ..
    cmdFunPtr cmdFun = ( *( cmdFunPtr * )( cmd->second ));
    ( this->*cmdFun )( message );

}