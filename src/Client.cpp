/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 20:01:27 by kamin             #+#    #+#             */
/*   Updated: 2023/05/12 20:11:59 by kamin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client(std::string nick, std::string user, int listen_socket, struct sockaddr_in hint) : _nick(nick), _user(user)
{
	int addrlen = sizeof(hint);
	_client_socket = accept(listen_socket, (sockaddr *)&hint, (socklen_t*)&addrlen);
}

int Client::getClientSocket( void ) {
	return ( this->_client_socket );
}