/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myvh <myvh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 18:23:28 by kamin             #+#    #+#             */
/*   Updated: 2024/01/01 15:19:51 by myvh             ###   ########.fr       */
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
    return ( valid_params );
}

Server* serv;

static void sigHandler(int sigNum) {
    serv->exit(sigNum);
}

bool IRCserver( int port, std::string const & pass ) {
    Server ircserver = Server( port, pass );
    serv = &ircserver;
    try {
        ircserver.init();
    } catch ( std::exception const & e ) {
        std::cerr << "[" << e.what() << "]" << std::endl;
    }
    ircserver.run();

    return ( true );
}

int main( int ac, char **av ) {
    struct sigaction sigIntHandler;

   sigIntHandler.sa_handler = sigHandler;
   sigemptyset(&sigIntHandler.sa_mask);
   sigIntHandler.sa_flags = 0;

    if (sigaction(SIGINT, &sigIntHandler, NULL) == -1)
        std::cout << "problem with handler" << std::endl; 
        
    if ( validParams( ac, av )) {
        IRCserver( atoi( av[1] ) , av[2] );
    }
    return ( 0 );
}