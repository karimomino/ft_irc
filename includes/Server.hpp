/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ommohame < ommohame@student.42abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 17:23:20 by kamin             #+#    #+#             */
/*   Updated: 2023/07/28 01:13:20 by ommohame         ###   ########.fr       */
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
#include "namespace.hpp"

#ifndef MAX_CLIENTS
#define MAX_CLIENTS 4

class Channel;
typedef std::string string;
std::vector<std::string> split_string( string str , string delim );
std::string    ft_itoa( int num );
// typedef std::vector< Channel > ChanVector;
typedef std::map< std::string, Channel> chan_map;
typedef std::vector< pollfd > PollVector;


class Server {
private:
    /* TYPEDEFS */
    typedef void ( Server::*cmdFun )( Client const &, std::string const & );
    typedef std::map< std::string const, cmdFun > CmdMap;

    /* VARIABLES */
    const int                            _port;
    const std::string                    _pass;
    int                                  _listen_socket;
    size_t                               _connectionCount;
    PollVector                           _poll_fds;
    struct sockaddr_in                   _hint;
    std::map<int, Client>                _clientMap;
    chan_map                             _channels;
    CmdMap                               _commands;

    /* METHODS */
    int                                 _initServer( void );
    int                                 _acceptConnection( void );
    void                                _runServer( void );
    void                                _parseMessage( Client &new_socket , char *buff );
    std::map<int, Client>::iterator     _getClient( const int fd );
    std::string                         _createMessage( Client client, string command );
    void                                _joinChannel( Client const & client, string name);
    void                                _broadcastJoin( Client client , Channel chan , string name );
    Client  *_findClientByNick( std::map<int, Client> &clients , string nick ) const;
    void    _pong( Client client);

    void                                _privmsg( string full_command , Client client );

        // ChanVector::iterator                _findChannel( chan_map & channels , std::string name ) const;
    bool  _addCommandFunction( std::string const & keyValue, cmdFun );
    bool  _initCommandsFunctions( void );
    void  _executeCommand( Client const & client, std::string const & message );

    /**                   COMMANDS FUN                   **/
    void    _joinCommand( Client const & user, std::string const & command );
    void    _kickCommand( Client const & client, std::string const & msg );

    bool    _sendMessage( Client const & client, std::string const & msg );
    bool    _sendMessage( Channel const & chan, std::string const & msg );
    bool    _sendMessage( Channel const & chan, std::string const & origin, std::string const & msg );
    void    _sendAMessage( std::string full_command );

public:
    typedef std::map<string, void (Client::*)( string )>::iterator command_it;
    Server( const int port, const string pass );
    // ~Server();
    int getListenSocket( void ) const;
    size_t    getConnectionCount ( void ) const;
    friend class Channel;
};
#endif
