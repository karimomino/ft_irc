/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ommohame < ommohame@student.42abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 17:23:20 by kamin             #+#    #+#             */
/*   Updated: 2023/08/01 22:42:49 by ommohame         ###   ########.fr       */
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
typedef std::map< std::string, Channel> chan_map;
typedef std::vector< pollfd > PollVector;
typedef std::vector<std::string>::const_iterator vec_const_it;
typedef std::vector<std::string>::iterator vec_it;

# define SERVER_NAME "42IRCSERV"
# define NBSP " "

class Server {
private:
    /* TYPEDEFS */
    typedef void ( Server::*cmdFun )( Client const &, std::string const & );
    typedef std::map< std::string const, cmdFun > CmdMap;

    /* VARIABLES */
    const int                            _port;
    const std::string                    _pass;
    int                                  _listen_socket;
    std::string                          _ip_string;
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
    void                                _joinChannel( Client const & client, string name); // ?
    void                                _joinChannel( Client const & client, string chanList , string chanKeys ); // ?
    void                                _joinCreate( Client const & client , string chan , string topic , string key , bool inv , bool top );
    void                                _joinExistingChannel( Client const & client , string chan );
    void                                _broadcastJoin( Client client , Channel chan , string name );
    Client                              *_findClientByNick( std::map<int, Client> &clients , string nick ) const;
    void                                _pong( Client client);
    void                                _privmsg( string full_command , Client client );
    void                                _sendNames( Client const & client , string chan);
    bool                                _allowedToJoin( Client client , Channel chan , string key ) const;

    bool  _addCommandFunction( std::string const & keyValue, cmdFun );
    bool  _initCommandsFunctions( void );
    void  _executeCommand( Client const & client, std::string const & message );

    /**                   COMMANDS FUN                   **/
    void    _joinCommand( Client const & client, std::string const & msg );
    void    _kickCommand( Client const & client, std::string const & msg );
    void    _modeCommand( Client const & client, std::string const & msg );

    void    _sendAMessage( std::string full_command );

public:
    Server( const int port, const string pass );
    // ~Server();
    int       getListenSocket( void ) const;
    size_t    getConnectionCount ( void ) const;

    bool    sendMsg( Client const &, std::string const & msg ) const ;
    bool    sendMsg( Channel const &, std::string const & msg ) const ;
    bool    sendMsg( Channel const &, std::string const & origin, std::string const & msg ) const;

    bool    sendMsg( std::string const & numReply, Client const & client, std::string const & msg ) const;
    bool    sendMsg( std::string const & numReply, Client const & client, std::string const & arg, std::string const & msg ) const;
    friend class Channel;
};
#endif
