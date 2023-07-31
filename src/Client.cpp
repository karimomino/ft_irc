/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 20:01:27 by kamin             #+#    #+#             */
/*   Updated: 2023/07/27 20:52:23 by kamin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <functional>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>

Client::Client( int listen_socket, struct sockaddr_in const &hint ) :   _pV4Addr((struct sockaddr_in*)&hint), _isRegistered(false), _isWelcomed(false)
{
	int addrlen = sizeof(hint);
	_client_socket = accept(listen_socket, (sockaddr *)&hint, (socklen_t*)&addrlen);
	_msgSent = 0;
	setAddrInfo();
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
	
	if ( _pass.length() && _nick.length() && _user.length() ) {
		_isRegistered = true;
		if ( _isWelcomed == false )
			_welcomeClient();
	}
}

void	Client::setNick( std::string nick) {
	_nick =  nick.substr(0, nick.length());

	if ( _pass.length() && _nick.length() && _user.length() ) {
		_isRegistered = true;
		if ( _isWelcomed == false )
			_welcomeClient();
	}
}

void	Client::setUser( std::string user) {
	_user =  user.substr(0, user.length());

    if ( _pass.length() && _nick.length() && _user.length() ) {
        _isRegistered = true;
		if ( _isWelcomed == false )
			_welcomeClient();
	}
}

bool    Client::getRegisteredStatus( void ) const {
	return ( _isRegistered );
}

void	Client::setAddrInfo( void ) {
	struct in_addr ipAddr = _pV4Addr->sin_addr;

	std::string blah = inet_ntoa(ipAddr);
	this->_ip = blah;
}

void	Client::_welcomeClient( void ) {
	std::string welcome_001 = ":" + _ip + " 001 " + getNick() + " :Welcome to FT_IRC " + getNick() + "!" + getUser() + "@" + getIp() + "\r\n";
	std::string your_host = ":" + _ip + " 002 " + getNick() + " :Your host is 127.0.0.1, running version idk anymore\r\n";
	std::string server_created = ":" + _ip + " 003 " + getNick() + " :Server created sometime this year.\r\n";
	send(getClientSocket() , welcome_001.c_str() , welcome_001.size() , 0x80);
	send(getClientSocket() , your_host.c_str() , your_host.size() , 0x80);
	send(getClientSocket() , server_created.c_str() , server_created.size() , 0x80);
	_isWelcomed = true;
}

std::string Client::getIp( void ) const {
	return ( _ip );
}


