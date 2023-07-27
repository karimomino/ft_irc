/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ommohame < ommohame@student.42abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 20:43:53 by kamin             #+#    #+#             */
/*   Updated: 2023/07/28 01:14:26 by ommohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include "Server.hpp"

#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

typedef std::string string;


class Client;
class Server;

class Channel {
    typedef std::vector< string > StrVector;
    typedef std::map< std::string const, Client const *> _clients_const;
    typedef _clients_const::const_iterator _clients_const_it;

private:
    string                 _name;
    string                 _topic;
    string                 _key;
    bool                   _isInviteOnly;
    bool                   _topicOpOnly;
    StrVector              _invitations;
    std::map<std::string const, Client const *> _clients;

    void    _removeInvitation( string );

public:
    Channel( string name , string topic , string key , bool inv , bool top );
    ~Channel( void );

    /* METHODS */
    void    addInvitation( string );
    void    addUser( std::string const & nick, Client const & client );
    bool    kickUser( std::string const & nick, std::string const & kickResponse );
    Client const & findClient( std::string const & name ) const;
    bool    sendMessage( Server& t , std::string const & origin , std::string const & msg ) const;
    // void    addUser( string );

    /* GETTERS */
    const string                getName( void )     const;
    const string                getTopic( void )    const;
    const string                getMode( void )     const;
    string	                getUsersStr( void ) const;
    StrVector                   getNicks( void )    const;
    std::vector<Client const *> getClients( void )  const;

    /* SETTERS */
    void        setName( string );
    void        setTopicMode( bool );
    void        setInviteMode( bool );
};

#endif
