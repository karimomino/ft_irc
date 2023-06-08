/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 17:23:20 by kamin             #+#    #+#             */
/*   Updated: 2023/06/08 08:51:10 by kamin            ###   ########.fr       */
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
		const int									_port;
		const std::string							_pass;
		
		int									_listen_socket;
		size_t								_connectionCount;

		PollVector							_poll_fds;
		struct sockaddr_in					_hint;
		std::map<int, Client>				_clientMap;
		ChanVector							_channels;

		int 								_initServer( void );
		int									_acceptConnection( void );
		void								_runServer( void );
		void								_parseMessage( Client &new_socket , char *buff );
		std::map <int, Client>::iterator	_getClient( const int fd );
		std::string							_createMessage( Client client, string command );
		void								_joinChannel( Client client , string name);
		void								_broadcastJoin( Client client , Channel chan , string name );
		ChanVector::iterator				_findChannel( ChanVector &channels , std::string name ) const;

		void								_privmsg( string full_command , Client client );

	
	public:
		typedef std::map<string, void (Client::*)( string )>::iterator command_it;
		Server( const int port, const string pass );
		// ~Server();
		int getListenSocket( void ) const;
		size_t	getConnectionCount ( void ) const;

};
