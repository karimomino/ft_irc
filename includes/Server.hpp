/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 17:23:20 by kamin             #+#    #+#             */
/*   Updated: 2023/05/19 14:14:46 by kamin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <fcntl.h>
#include <string>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <map>
#include <vector>
// #include <cstdio>
#include "../includes/Client.hpp"

#ifndef MAX_CLIENTS
#define MAX_CLIENTS 2
#endif

std::vector<std::string> split_string( std::string str );

class Server {
private:
	int					_listen_socket;
	int					_port;
	int					_connectionCount;
	struct sockaddr_in	_hint;
	struct pollfd		_poll_fds[ MAX_CLIENTS ];
	std::string			_pass;
	std::map<int, Client>	_clientMap;
	std::map<std::string, int (*)( int )> _commands;
public:
	Server( const int port, const std::string pass );
	// ~Server();
	int initServer( void );
	int	acceptConnection( void );
	struct pollfd *getPollFds( void );
	int getListenSocket( void );
	int	getConnectionCount ( void );
	std::map <int, Client>::iterator getClient( const int fd );
	std::string			parseMessage( int new_socket , char *buff );
};
