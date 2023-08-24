/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ommohame < ommohame@student.42abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 18:23:28 by kamin             #+#    #+#             */
/*   Updated: 2023/08/18 17:52:35 by ommohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>

bool validParams( int ac, char **av ) {
    bool valid_params = true;

    if ( ac != 3 ) {
        std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
        std::cout << "where 1 <= <port> <= 65535" << std::endl;
        valid_params = false;
    }
    ( void )av;
    // TODO: check valid port range
    return ( valid_params );
}

bool IRCserver( int port, std::string const & pass ) {
    Server ircserver = Server( port, pass );
    try {
        ircserver.init();
    } catch ( std::exception const & e ) {
        std::cerr << "[" << e.what() << "]" << std::endl;
    }
    ircserver.run();
    // ircserver.end();

    return ( true );
}

int main( int ac, char **av ) {
    if ( validParams( ac, av )) {
        IRCserver( atoi( av[1] ) , av[2] );
    }
    return ( 0 );
}
