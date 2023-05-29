/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 18:23:28 by kamin             #+#    #+#             */
/*   Updated: 2023/05/27 17:20:55 by kamin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>

int	validParams(int ac, char **av) {
	int	valid_params = 1;
	
	if ( ac != 3 ) {
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		std::cout << "where 1 <= <port> <= 65535" << std::endl;
		valid_params = 0;
	} 
	(void)av;
	// TODO: check valid port range
	return ( valid_params );
}

int main(int ac, char **av) {

	if ( validParams(ac, av) ) {
		Server ircserver = Server( atoi(av[1]) , av[2] );	
	}
	return ( 0 );
}