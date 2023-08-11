/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 20:43:53 by kamin             #+#    #+#             */
/*   Updated: 2023/08/10 23:29:51 by kamin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef CHANNEL_HPP
# define CHANNEL_HPP
#include <vector>
#include <string>
#include <algorithm>
#include <map>
// #include "Server.hpp"

#ifdef DEBUG
# define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
# define DEBUG_MSG(str) do { } while ( false )
#endif

typedef std::string string;


class Client;
class Server;

class Channel {
    typedef std::vector<std::string>  _string_vec;
    typedef std::vector<Client *>    _clients_vec;

    typedef std::map< std::string const, Client *>    _cclients_map;
    typedef _cclients_map::iterator                   _cclients_const_it;

    typedef std::vector<std::string>            _invitations_vec;
    typedef _invitations_vec::const_iterator    _invitations_it;

private:
    std::string        _name;
    std::string        _topic;
    std::string        _key;
    bool               _isInviteOnly;
    bool               _topicOpOnly;
    _cclients_map      _clients;
    _invitations_vec   _invitations;


public:
    Channel( string name , string topic , string key , bool inv , bool top );
    ~Channel( void );

    /* METHODS */
    void           addUser( std::string const & nick, Client & client );
    bool           kickUser( std::string const & nick, std::string const & kickResponse );
    Client const & findClient( std::string const & name ) const;
    bool           sendMsg( Server const & t , std::string const & origin , std::string const & msg );
    bool           addInvitation( std::string const & nick );
    bool           removeInvitation( std::string const & nick );

    /* GETTERS */
    const string  getName( void )     const;
    const string  getTopic( void )    const;
    const string  getMode( void )     const;
    const string  getUsersStr( void );
    _string_vec   getNicks( void );
    _clients_vec  getClients( void ) ;
    bool          isInviteOnly( void ) const;
    bool          isTopicOpOnly( void ) const;
    std::string const & getKey( void ) const;
    bool          isInvited ( std::string const & nick ) const;
    bool          isMember( std::string const & nick ) const;
    bool          isOperator( std::string const & nick ) const;

    /* SETTERS */
    void        setName( string );
    void        setTopicMode( bool );
    void        setInviteMode( bool );
};

#endif
