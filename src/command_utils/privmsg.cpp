/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ommohame < ommohame@student.42abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 16:10:25 by kamin             #+#    #+#             */
/*   Updated: 2023/08/01 22:45:02 by ommohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include <cstdio>
#include <iostream>

static string  findTargetName( string full_command ) {
    string chan_name;
    std::vector< string > split_command = utils::split( full_command , " ");
    std::vector< string >::iterator split_it = split_command.begin();

    chan_name = *(++split_it);
    DEBUG_MSG("SEND IN CHANNEL: " << chan_name);
    return ( chan_name );
}

static string  findMsgOrigin ( Client client ) {
    string origin;

    origin = ":" + client.getNick() + "!" + client.getUser() + "@" + client.getIp() + " ";

    DEBUG_MSG("ORIGIN OF MESSAGE: " << origin);
    return ( origin );
}

static string findText( string full_command ) {
    string text;
    std::vector< string > split_command = utils::split( full_command , ":");
    std::vector< string >::iterator split_it = split_command.begin();
    split_it++;
    text = *split_it;
    for (split_it = split_it + 1; split_it != split_command.end() ; split_it++)
        text += ":" + *split_it;
    DEBUG_MSG("MESSAGE CONTENT: " << text);
    return ( text );
}

void Server::_privmsg( string full_command , Client client) {

    DEBUG_MSG("####################START MESSAGE COMMAND#############################");
    DEBUG_MSG("COMMAND: " << full_command);
    string target = findTargetName( full_command );

    string text = findText( full_command );
    std::string actual_nick;

    if (target[0] == '#') {
        Channel channel = _channels.find( target )->second;
        sendMsg( channel , message::getMsgOrigin( client ) , text);
    } else {
            string msg_to_client = message::getMsgOrigin( client ) + "PRIVMSG " + target + " :" + text;
            Client *client = _findClientByNick(_clientMap, target);
            if ( client ) {
                send( client->getClientSocket(), msg_to_client.c_str() , msg_to_client.length(), 0x80);
            }
        }

    DEBUG_MSG("####################END MESSAGE COMMAND###############################");
}
