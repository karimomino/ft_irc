/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ommohame < ommohame@student.42abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 19:56:23 by kamin             #+#    #+#             */
/*   Updated: 2023/06/09 00:40:05 by ommohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>

class Client
{
private:
	int			_client_socket;
	std::string	_nick;
	std::string	_user;
	std::string	_pass;
	struct sockaddr_in* _pV4Addr;
	std::string	_ip;
	size_t		_msgSent;
	struct addrinfo *_servinfo;
	bool		_isRegistered;


public:
	Client( int listen_socket, struct sockaddr_in hint );
	// ~Client( void );
	int getClientSocket( void );
	size_t	getMsgSent( void );
	int		joinChannel( std::string chan );
	void	incMsgSent( void );
	std::string	getPass( void );
	void	setPass( std::string newPass );
	std::string	getNick( void );
	void	setNick( std::string newNick );
	std::string	getUser( void );
	void	setUser( std::string newUser );
	bool	getRegisteredStatus( void );
	void	setAddrInfo( void );
	std::string	getIp( void );

};
