/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ommohame < ommohame@student.42abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 20:01:27 by kamin             #+#    #+#             */
/*   Updated: 2023/06/09 00:32:21 by ommohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <functional>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>

Client::Client( int listen_socket, struct sockaddr_in hint )
{
	int addrlen = sizeof(hint);
	_client_socket = accept(listen_socket, (sockaddr *)&hint, (socklen_t*)&addrlen);
	_pV4Addr = (struct sockaddr_in*)&hint;
	setAddrInfo(  );
	_isRegistered = false;
	_msgSent = 0;
}

int Client::getClientSocket( void ) const {
    return ( this->_client_socket );
}

size_t    Client::getMsgSent( void ) const {
    return ( this->_msgSent );
}

std::string    Client::getNick( void ) const {
    return ( _nick );
}

std::string    Client::getUser( void ) const {
    return ( _user );
}

void    Client::incMsgSent( void ) {
    ++_msgSent;
}

void	Client::setPass( std::string pass) {
	_pass =  pass.substr(0, pass.length());
	// _pass[_pass.length() - 1] = '\0';
	if ( _pass.length() && _nick.length() && _user.length() )
		_isRegistered = true;
}

void	Client::setNick( std::string nick) {
	_nick =  nick.substr(0, nick.length());
	// _nick[_nick.length() - 1] = '\0';

	if ( _pass.length() && _nick.length() && _user.length() )
		_isRegistered = true;
}

void	Client::setUser( std::string user) {
	_user =  user.substr(0, user.length());

    if ( _pass.length() && _nick.length() && _user.length() )
        _isRegistered = true;
}

bool    Client::getRegisteredStatus( void ) const {
	return ( _isRegistered );
}

void	Client::setAddrInfo( void ) {
	// struct addrinfo ai;

	// memset( &ai , 0 , sizeof(ai) );
	// ai.ai_family = AF_INET;
	// ai.ai_socktype = SOCK_STREAM;
	// getaddrinfo(NULL , port.c_str() , &ai , &_servinfo);
	// struct sockaddr_in *ipv4 = (struct sockaddr_in *)_servinfo->ai_addr;
	// _ip = inet_ntoa(ipv4->sin_addr);
	struct in_addr ipAddr = _pV4Addr->sin_addr;

	std::string blah = inet_ntoa(ipAddr);
	this->_ip = blah;
	// blah.copy(_ip, blah.length() , 0);
	// std::string(inet_ntoa(ipAddr)).copy(_ip, std::string(inet_ntoa(ipAddr)).length() , 0);
}

std::string Client::getIp( void ) const {
	return ( _ip );
}
