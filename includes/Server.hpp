/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ommohame < ommohame@student.42abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 17:23:20 by kamin             #+#    #+#             */
/*   Updated: 2023/06/09 00:37:43 by ommohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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
#include <cstring>
#include <vector>
#include <ctime>
#include "Client.hpp"
#include "Replies.hpp"
#include "Channel.hpp"

#ifndef MAX_CLIENTS
#define MAX_CLIENTS 4

std::vector<std::string> split_string( string str , string delim );
std::string	ft_itoa( int num );
typedef std::vector< Channel > ChanVector;
typedef	std::vector< pollfd > PollVector;

#endif


class Server {
	private:
		int									_listen_socket;
		int									_port;
		int 								_initServer( void );
		int									_acceptConnection( void );
		void								_runServer( void );
		size_t								_connectionCount;
		std::string							_pass;
		std::string							_parseMessage( Client &new_socket , char *buff );
		struct pollfd						_poll_fds[ MAX_CLIENTS ];
		struct sockaddr_in					_hint;
		std::map<int, Client>				_clientMap;
		std::map <int, Client>::iterator	_getClient( const int fd );
		std::string							_createMessage( Client client, std::string command );

        /**                 COMMANDS TYPEDEF                 **/
        typedef void ( Server::*cmdFunPtr )( std::string const & );
        std::map<std::string const , void *> _commandsFunctions;

        /**               COMMANDS HELPER FUN                **/
        template <typename T>
        bool  _addCommandFunction( std::string const & keyValue, T );
        bool  _initCommandsFunctions( void );
        void  _executeCommand( std::string const & message );

        /**                     COMMANDS FUN                 **/
        void  _joinCommand( std::string const & command );
	
	public:
		typedef std::map<std::string, void (Client::*)( std::string )>::iterator command_it;
		Server( const int port, const std::string pass );
		// ~Server();
		struct pollfd *getPollFds( void );
		int getListenSocket( void );
		size_t	getConnectionCount ( void );
};
