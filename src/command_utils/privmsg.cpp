/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 16:10:25 by kamin             #+#    #+#             */
/*   Updated: 2023/06/08 08:53:27 by kamin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include <cstdio>
#include <iostream>

string  findChannelName( string full_command ) {
    string chan_name;
    std::vector< string > split_command = split_string( full_command , " ");
    std::vector< string >::iterator split_it = split_command.begin();

    chan_name = *(++split_it);
    std::cout << "SEND IN CHANNEL: " << chan_name << std::endl;
    return ( chan_name );

}

string  findMsgOrigin ( Client client ) {
    string origin;

    origin = ":" + client.getNick() + "!" + client.getUser() + "@" + client.getIp() + " ";

    std::cout << "ORIGIN OF MESSAGE: " << origin << std::endl;
    return ( origin );
    
}

string findText( string full_command ) {
    string text;
    std::vector< string > split_command = split_string( full_command , ":");
    std::vector< string >::iterator split_it = split_command.begin();

    text = *(split_it + 1);
    std::cout << "MESSAGE CONTENT: " << text << std::endl;
    return ( text );
}

void Server::_privmsg( string full_command , Client client) {

    std::cout << "####################START MESSAGE COMMAND#############################" << std::endl;
    std::cout << "COMMAND: " << full_command;
    string channel = findChannelName( full_command );
    ChanVector::iterator    chan_it = _findChannel(_channels, channel);
    std::vector< std::string > tmp_nicks = chan_it->getNicks();

    string text = findText( full_command );
    string msg_to_client = findMsgOrigin( client ) + "PRIVMSG " + channel + " :" + text;
    std::string actual_nick;


    for (size_t i = 0; i < tmp_nicks.size(); i++) {
        if ( tmp_nicks[i][0] == '@' )
            actual_nick = tmp_nicks[i].substr( 1, tmp_nicks[i].length() - 1 );
        else 
            actual_nick = tmp_nicks[i];


        for ( std::map<int, Client>::iterator client_it = _clientMap.begin() ; client_it != _clientMap.end(); client_it++) {
            if ( actual_nick != client.getNick() && client_it->second.getNick() == actual_nick )
            {
                printf("SENDING TO NICK: %s\n" , actual_nick.c_str());
                std::cout << msg_to_client << std::endl;
                send(client_it->second.getClientSocket() , msg_to_client.c_str() , msg_to_client.length(), 0x80);
                break;
            }
        }
    }

    std::cout << "####################END MESSAGE COMMAND###############################" << std::endl;

}
