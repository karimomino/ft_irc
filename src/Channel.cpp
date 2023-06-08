/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 15:07:23 by kamin             #+#    #+#             */
/*   Updated: 2023/06/07 10:33:12 by kamin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"
#include <iostream>
#include <string>

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
    _isInviteOnly = mode;
}

const std::string Channel::getName( void ) {
    return ( _name );
}

const std::string Channel::getTopic( void ) {
    return ( _topic );
}

// const std::string Channel::getMode( void ) {
//     return ( _mode );
// }

void Channel::addInvitation( std::string nick ) {
    _invitations.push_back( nick );
}

void Channel::_removeInvitation( std::string nick ) {
    StrVector::iterator    nick_pos = std::find(_invitations.begin(), _invitations.end(), nick);
    if ( nick_pos != _invitations.end())
        _invitations.erase(nick_pos);
}

void Channel::addUser( std::string nick ) {
    _nicks.push_back( nick );
}

string  Channel::getUsersStr( void ) {
    string  nicks;
    
    for (size_t i = 0; _nicks.size() > 0 && i < _nicks.size(); i++) {
        if (i == 0 )
            nicks.append( _nicks[i] );
        else
         nicks.append( " " + _nicks[i]);
    }
    
    return ( nicks );
}

Channel::StrVector Channel::getNicks( void ) const {
            return ( _nicks );
        }