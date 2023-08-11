/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ommohame < ommohame@student.42abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 15:07:23 by kamin             #+#    #+#             */
/*   Updated: 2023/08/01 22:50:14 by ommohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"
#include "Client.hpp"
#include <iostream>
#include <map>
#include <string>
#include <Server.hpp>

Channel::Channel( string name , string topic , string key , bool inv , bool top )
    : _name( name ) , _topic( topic ) , _key( key ) , _isInviteOnly( inv ) , _topicOpOnly( top ) {}

Channel::~Channel( void ) {}

void	Channel::setName( std::string name ) {
    _name = name;
}

void	Channel::setTopicMode( bool mode ) {
    _topicOpOnly = mode;
}

void	Channel::setInviteMode( bool mode ) {
    if ( mode == _isInviteOnly )
        return ;
    _isInviteOnly = mode;
    // sendMsg( *this, ":" + _ip_string + " 324 " + client.getNick() + " " + chan.getName() + " +i\r\n");
}

void    Channel::setKeyMode( bool mode ) {
    _keyMode = mode;
}

const std::string Channel::getName( void ) const {
    return ( _name );
}

const std::string Channel::getTopic( void ) const {
    return ( _topic );
}

std::string const & Channel::getKey( void ) const {
    return ( _key );
}

bool Channel::isInviteOnly( void ) const {
    return ( _isInviteOnly );
}
bool Channel::isTopicOpOnly( void ) const {
    return ( _topicOpOnly );
}

bool Channel::isInvited ( std::string const & nick ) const {
    std::vector<std::string>::const_iterator it = std::find( _invitations.begin(), _invitations.end(), nick );
    if ( it == _invitations.end() )
        return ( false );
    return ( true );
}

bool Channel::addInvitation( std::string const & nick ) {
    if ( isInvited( nick ) )
        return ( false );
    _invitations.push_back( nick );
    return ( true );
}

bool Channel::removeInvitation( std::string const & nick ) {
    std::vector<std::string>::iterator it = std::find( _invitations.begin(), _invitations.end(), nick );
    if ( it == _invitations.end() )
        return ( false );
    _invitations.erase( it );
    return ( true );
}

void Channel::addUser( std::string const & nick, Client const & client ) {
    _clients.insert( std::pair<std::string, Client const *>( nick, &client ) );
}

const string  Channel::getUsersStr( void ) const {
    std::string  nicks;

    for ( _cclients_const_it it = _clients.begin(); it != _clients.end(); ++it ) {
        std::string nick = it->first;
        nicks.append( " " + nick );
    }

    return ( nicks );
}

Channel::_string_vec Channel::getNicks( void ) const {
    _string_vec nicks;

    // TODO: fix it later to make it more efficient
    for ( _cclients_const_it it = _clients.begin(); it != _clients.end(); it++ ) {
        std::string nick = it->first;
        nicks.push_back( nick );
    }
    return ( nicks );
}

bool Channel::removeUser( std::string const & nick ) {
    _cclients_const_it user = _clients.find( nick );
    if ( user == _clients.end() )
        return ( false );

    _clients.erase( nick );

    return ( true );
}

bool    Channel::sendMsg( Server const & t , std::string const & origin , std::string const & msg ) const {
    bool sendRet = false;
    for (_cclients_const_it it = _clients.begin(); it != _clients.end(); it++)
    {
        const Client *curr_client = it->second;
        std::string finalMsg = origin + "PRIVMSG " + _name + " :" + msg;
        DEBUG_MSG(finalMsg << std::endl);
        DEBUG_MSG("CLIENT NAME: " << it->first << "\nCLIENT FD: " << curr_client->getClientSocket() << std::endl);
        sendRet =  t.sendMsg( *curr_client , finalMsg );
    }
    return ( sendRet );
}

std::vector<Client const *> Channel::getClients( void ) const {
    std::vector<Client const *> clients;

    for ( _cclients_const_it it = _clients.begin(); it != _clients.end(); it++ )
        clients.push_back( it->second );

    return ( clients );
}

Client const & Channel::findClient( std::string const & nick ) const {
    return ( *_clients.find( nick )->second );
}

bool Channel::isMember( std::string const & nick ) const {
    if ( isOperator( nick ) )
        return ( true );
    if ( _clients.find( nick ) == _clients.end() )
        return ( false );
    return ( true );
}

bool Channel::isOperator( std::string const & nick ) const {
    if ( _clients.find( "@" + nick ) == _clients.end() )
        return ( false );
    return ( true );
}

