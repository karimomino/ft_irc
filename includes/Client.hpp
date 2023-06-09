/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 19:56:23 by kamin             #+#    #+#             */
/*   Updated: 2023/05/17 17:06:41 by kamin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

class Client
{
private:
	int		_client_socket;
	char	*_nick;
	char	*_user;
	char	*_pass;
	size_t	_msgSent;
	std::map<std::string, int (*)( int )> commands;


public:
	Client( int listen_socket, struct sockaddr_in hint );
	// ~Client( void );
	int getClientSocket( void );
	size_t	getMsgSent( void );
	void	incMsgSent( void );
	std::string	getPass( void );
	void	setPass( std::string newPass );
	std::string	getNick( void );
	void	setNick( std::string newNick );
	std::string	getUser( void );
	void	setUser( std::string newUser );
};