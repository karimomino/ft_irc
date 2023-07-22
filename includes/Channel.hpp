/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 20:43:53 by kamin             #+#    #+#             */
/*   Updated: 2023/06/07 10:32:32 by kamin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP
#include <vector>
#include <string>
#include <algorithm>
#include <map>

typedef std::string string;

#endif

class Client;

class Channel {
    typedef std::vector< string > StrVector;
    typedef std::map< std::string const, const Client *>::iterator _clients_it;
    typedef std::map< std::string const, const Client *>::const_iterator _clients_const_it;

private:
    string                 _name;
    string                 _topic;
    string                 _key;
    bool                   _isInviteOnly;
    bool                   _topicOpOnly;
    StrVector              _invitations;
    std::map<std::string const, const Client *> _clients;

    void    _removeInvitation( string );

public:
    Channel( string name , string topic , string key , bool inv , bool top );
    ~Channel( void );

    /* METHODS */
    void    addInvitation( string );
    void    addUser( std::string const & nick, Client & client );
    bool    removeUser( std::string const & nick );
    // void    addUser( string );

    /* GETTERS */
    const string getName( void )     const;
    const string getTopic( void )    const;
    const string getMode( void )     const;
    string	 getUsersStr( void ) const;
    StrVector    getNicks( void ) const ;

    /* SETTERS */
    void        setName( string );
    void        setTopicMode( bool );
    void        setInviteMode( bool );
};
