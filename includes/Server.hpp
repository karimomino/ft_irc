/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 17:23:20 by kamin             #+#    #+#             */
/*   Updated: 2023/05/12 20:39:24 by kamin            ###   ########.fr       */
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

#ifndef MAX_CLIENTS
#define MAX_CLIENTS 2
#endif


class Server {
private:
	int					_listen_socket;
	int					_port;
	int					_connectionCount;
	struct sockaddr_in	_hint;
	struct pollfd		_poll_fds[ MAX_CLIENTS ];
	std::string			_pass;
public:
	Server( const int port, const std::string pass );
	// ~Server();
	int initServer( void );
	int	acceptConnection( void );
	struct pollfd *getPollFds( void );
	int getListenSocket( void );
	int	getConnectionCount ( void );
};
