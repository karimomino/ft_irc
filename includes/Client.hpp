/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 19:56:23 by kamin             #+#    #+#             */
/*   Updated: 2023/05/12 20:31:40 by kamin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

class Client
{
private:
	int			_client_socket;
	std::string	_nick;
	std::string	_user;


public:
	Client( std::string nick, std::string user, int listen_socket, struct sockaddr_in hint );
	// ~Client( void );
	int getClientSocket( void );
};