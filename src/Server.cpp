/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 17:41:13 by kamin             #+#    #+#             */
/*   Updated: 2023/05/19 20:29:35 by kamin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::Server( const int port, const std::string pass ) : _port( port ), _pass( pass )
{
	_hint.sin_family = AF_INET;
	_hint.sin_port = htons( port );
	_hint.sin_addr.s_addr = INADDR_ANY;
	_connectionCount = 0;
}

int Server::initServer( ) {
	int	ret = 1;
	int	sock_opt = 1;
	
	_listen_socket = socket( AF_INET , SOCK_STREAM, 0 );
	if ( _listen_socket < 0 ) {
		std::cout << "Error creating socket!" << std::endl;
		ret = 0;
	} else if ( setsockopt(_listen_socket, SOL_SOCKET, SO_REUSEADDR, &sock_opt,sizeof(sock_opt)) ) {
		std::cout << "Error setting socket options!" << std::endl;
		ret = 0;
	} else if ( fcntl(_listen_socket, F_SETFL, O_NONBLOCK) == -1 ) {
		std::cout << "Error setting socket to non-blocking!" << std::endl;
		ret = 0;
	} else if (bind(_listen_socket, (sockaddr *)&_hint, sizeof(_hint)) < 0) {
		std::cout << "Error binding!" << std::endl;
		ret = 0;
	} else if ( listen(_listen_socket, MAX_CLIENTS) ) {
		std::cout << "Failed to listen" << std::endl;
		ret = 0;
	}
	_connectionCount++;
	std::cout << "setting fd \n";
	memset(&_poll_fds, 0 , MAX_CLIENTS + 1);
	_poll_fds[0].fd = _listen_socket;
	_poll_fds[0].events = POLLIN;
	return ( ret );
}

std::string Server::parseMessage( int new_socket , char *buff ) {

	std::map <int, Client>::iterator it = getClient(new_socket);
	std::vector<std::string> wordList = split_string( buff );
	std::vector<std::string>::iterator word_it = wordList.begin();

	// TODO: use map of functions instead of all these ifs - init the map on server start
	if(!(*word_it).compare("PASS")) {
		word_it++;
		it->second.setPass((*word_it));
	} else if (!(*word_it).compare("NICK")) {
		word_it++;
		it->second.setPass((*word_it));
	} else if (!(*word_it).compare("USER")) {
		word_it++;
		it->second.setUser((*word_it));
	}

	return ("   ");
}

std::map <int, Client>::iterator Server::getClient( const int fd ) {
	return( _clientMap.find( fd ) );
}

int	Server::acceptConnection( void ) {
	// TODO: recv() the connection message from the client and parse it
	// int val = recv(new_socket, buffer, 1024, MSG_DONTWAIT);
	Client newClient = Client(_listen_socket, _hint);
	_clientMap.insert( std::pair<int,Client>(newClient.getClientSocket(), newClient) );
	if (_connectionCount < MAX_CLIENTS) {
		std::stringstream ss;
		std::string connectionMessage;

		_poll_fds[_connectionCount].fd = newClient.getClientSocket();
		_poll_fds[_connectionCount].events = POLLIN;
		
		ss << ":127.0.0.1 001 " << "nickKooki :" << "Welcome to kamin's ft_irc " << "nickKooki. " << "This is MOTD\r\n";
		connectionMessage = ss.str();
		send(newClient.getClientSocket(), connectionMessage.c_str(), connectionMessage.length(), 0x80);
	} else {
		std::cout << "Connection Refused, MAX clients Reached" << std::endl;
		close( newClient.getClientSocket() );
	}
	
	return ( 0 ) ;
}

struct pollfd *Server::getPollFds( void ) {
	return ( this->_poll_fds );
}

int Server::getListenSocket( void ) {
	return ( this->_listen_socket );
}

int Server::getConnectionCount( void ) {
	return ( this->_connectionCount );
}
