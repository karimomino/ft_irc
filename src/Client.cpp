/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 20:01:27 by kamin             #+#    #+#             */
/*   Updated: 2023/05/28 20:11:17 by kamin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <functional>
#include <iostream>

Client::Client( int listen_socket, struct sockaddr_in hint )
{
    int addrlen = sizeof( hint );
    _client_socket = accept( listen_socket, ( sockaddr * )&hint, ( socklen_t * )&addrlen );
    _isRegistered = false;
    _msgSent = 0;
}

int Client::getClientSocket( void ) {
    return ( this->_client_socket );
}

size_t    Client::getMsgSent( void ) {
    return ( this->_msgSent );
}

std::string    Client::getNick( void ) {
    return ( _nick );
}

std::string    Client::getUser( void ) {

    return ( _user );
}

void    Client::incMsgSent( void ) {
    ++_msgSent;
}

void    Client::setPass( std::string pass) {
    _pass =  pass.substr(0, pass.length() - 2);
    // _pass[_pass.length() - 1] = '\0';
    std::cout << "setting pass to :" << _pass << std::endl;

    if ( _pass.length() && _nick.length() && _user.length() )
        _isRegistered = true;
}

void    Client::setNick( std::string nick) {
    _nick =  nick.substr(0, nick.length() - 2);
    // _nick[_nick.length() - 1] = '\0';

    std::cout << "setting nick to :" << _nick << std::endl;

    if ( _pass.length() && _nick.length() && _user.length() )
        _isRegistered = true;
}

void    Client::setUser( std::string user) {
    std::cout << "setting user to :" << user << std::endl;
    _user =  user.substr(0, user.length());

    if ( _pass.length() && _nick.length() && _user.length() )
        _isRegistered = true;
}

bool    Client::getRegisteredStatus( void )
{
    return ( _isRegistered );
}
