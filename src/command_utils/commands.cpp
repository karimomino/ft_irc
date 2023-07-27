/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ommohame < ommohame@student.42abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 15:40:43 by kamin             #+#    #+#             */
/*   Updated: 2023/07/28 01:09:03 by ommohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void Server::_parseMessage(Client &client, char *buff) {

    std::vector<std::string> cmd_list = utils::split(buff , "\r\n" );

    for ( std::vector<std::string>::iterator cmd_it = cmd_list.begin() ; cmd_it != cmd_list.end() ; cmd_it++ ) {
        std::string comm = (*cmd_it);
        DEBUG_MSG("command : " << comm);
        std::vector<std::string> word_list = utils::split(comm , " " );
        std::vector<std::string>::iterator word_it = word_list.begin();
        std::string command_prefix = *word_it;

        if (!command_prefix.compare("PING")) {
            _pong( client );
        } else if (!command_prefix.compare("PASS") && !client.getRegisteredStatus()) {
            word_it++;
            client.setPass(*word_it);
        } else if (!command_prefix.compare("NICK") && !client.getRegisteredStatus()) {
            word_it++;
            Client *c = _findClientByNick( _clientMap , *word_it );
            string err = ":127.0.0.1 433 * " + *word_it + " :Nickname is already in use.\r\n";
            if ( c == NULL )
                client.setNick(*word_it);
            else
                send( client.getClientSocket(), err.c_str(), err.length() , 0x80 );
        } else if (!command_prefix.compare("USER") && !client.getRegisteredStatus()) {
            word_it++;
            client.setUser(*word_it);
        } else if (!command_prefix.compare("JOIN") && client.getRegisteredStatus()) {
            word_it++;
            _joinChannel(client, (*word_it).substr(0, (*word_it).length()));
        } else if (!command_prefix.compare("PRIVMSG") && client.getRegisteredStatus()) {

            _privmsg( buff , client );

        } else if (!command_prefix.compare("QUIT")) {
            _connectionCount--;
            close(client.getClientSocket());
        } else if (!command_prefix.compare("KICK")) {
            _kickCommand( client, buff );
        }
        
    }


    
    // else {
    //     std::cout << buff << std::endl;
    // }

    // if ( !client.getRegisteredStatus() )
    // {
    // 	std::cout << "Client is not registered yet. Please set passowrd, nick, and user to be able to use any commands." << std::endl;
    // }
}
