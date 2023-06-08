/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 17:23:20 by kamin             #+#    #+#             */
/*   Updated: 2023/05/28 18:37:58 by kamin            ###   ########.fr       */
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
#include <vector>
#include <ctime>
#include "Client.hpp"
#include "Replies.hpp"

#ifndef MAX_CLIENTS
#define MAX_CLIENTS 4
#endif

std::vector<std::string> split_string( std::string str );

class Server {
    private:
        int                                  _listen_socket;
        int                                  _port;
        size_t                               _connectionCount;
        std::string                          _pass;
        struct pollfd                        _poll_fds[ MAX_CLIENTS ];
        struct sockaddr_in                   _hint;
        std::map<int, Client>                _clientMap;
        void *                               _ptrAdrss;


        bool                                _initServer( void );
        int                                 _acceptConnection( void );
        void                                _runServer( void );
        std::string                         _parseMessage( Client &new_socket , char *buff );
        std::map<int, Client>::iterator     _getClient( const int fd );
        std::string                         _createMessage( Client client, std::string command );

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
        Server( const int port, const std::string pass );
        ~Server();

        int             getListenSocket( void )     const;
        const struct pollfd * getPollFds( void )    const;
        size_t          getConnectionCount( void ) const;

        typedef std::map<std::string, void (Client::*)( std::string )>::iterator command_it;
};
