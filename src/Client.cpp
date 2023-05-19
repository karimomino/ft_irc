/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 20:01:27 by kamin             #+#    #+#             */
/*   Updated: 2023/05/19 20:28:25 by kamin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"
#include <functional>

int multiply(int a, int b) { return a * b; }

Client::Client( int listen_socket, struct sockaddr_in hint )
{
	int addrlen = sizeof(hint);
	_client_socket = accept(listen_socket, (sockaddr *)&hint, (socklen_t*)&addrlen);
	_nick = nullptr;
	_user = nullptr;
	_pass = nullptr;
	_msgSent = 0;
	_commands.insert(std::pair<std::string, void (Client::*)( std::string  )>("NICK" , &Client::setNick ));
}

int Client::getClientSocket( void ) {
	return ( this->_client_socket );
}

size_t	Client::getMsgSent( void ) {
	return ( this->_msgSent );
}

void	Client::incMsgSent( void ) {
	++_msgSent;
}

void	Client::setPass( std::string pass) {
	pass.copy(_pass, pass.length());
}

void	Client::setNick( std::string nick) {
	nick.copy(_nick, nick.length());
}

void	Client::setUser( std::string user) {
	user.copy(_user, user.length());
}