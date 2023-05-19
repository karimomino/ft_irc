/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 18:23:28 by kamin             #+#    #+#             */
/*   Updated: 2023/05/19 14:09:25 by kamin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

int	validParams(int ac, char **av) {
	int	ret = 1;
	std::cout << "validating params ..." << std::endl;
	if ( ac != 3 ) {
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		std::cout << "where 1 <= <port> <= 65535" << std::endl;
		ret = 0;
	}
	std::cout << "validated params ..." << std::endl;

	// TODO: check valid port range
	return ( ret );
}

int main(int ac, char **av) {
	int	rcv;
	Server ircserver = Server(atoi(av[1]) , av[2]);
	std::cout << "creating fds\n";
	// check if server parameters are correct && server started correctly
	if ( validParams(ac, av) && ircserver.initServer() ) {
		std::cout << "Ready for connections ..." << std::endl;
		struct pollfd *poll_fds = ircserver.getPollFds();

		// Server Loop
		while ( true )
		{
			int	pollCount = poll( poll_fds, MAX_CLIENTS + 1, 1000 );
			rcv = 0;

			for (size_t i = 0; i < pollCount; i++)
			{
				// if socket didnt return POLLIN run next iteration
				if (poll_fds[i].revents == 0)
					continue;
				
				int	socket = poll_fds[i].fd;

				// TODO: add check if returned something other than POLLIN error
				
				// this is a new connection
				// TODO: loop to accept all incoming connections
				if ( socket == ircserver.getListenSocket() ) {
					ircserver.acceptConnection();
				} else {
					std::map <int, Client>::iterator tmp = ircserver.getClient( socket );
					
					// TODO: loop to receive bigger messages
					char buff[1024];
					if ( recv(socket, buff, 1024, MSG_DONTWAIT) > 0 )
					{
						tmp->second.incMsgSent();
						std::cout << "Message: " << tmp->second.getMsgSent() << std::endl;
						ircserver.parseMessage( socket, buff );
						// TODO : check if password matches
						if ( tmp->second.getMsgSent() <= 3) {}

					}
					memset(buff, 0 , 1024);
				}
			}
			
		}
		
	}
	return ( 0 );
}