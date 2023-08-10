/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 16:10:25 by kamin             #+#    #+#             */
/*   Updated: 2023/08/10 12:25:18 by kamin            ###   ########.fr       */
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

static string findText( string full_command ) {
    string text;
    std::vector< string > split_command = utils::split( full_command , ":");
    std::vector< string >::iterator split_it = split_command.begin();
    if (split_command.size() < 2)
        text = "";
    else {
        split_it++;
        text = *split_it;
        for (split_it = split_it + 1; split_it != split_command.end() ; split_it++)
            text += ":" + *split_it;
    }
    DEBUG_MSG("MESSAGE CONTENT: " << text);
    return ( text );
}

Channel *Server::_findChanByName( string name ) {
    chan_map::iterator chan_it = _channels.find( name );
    Channel *chan;

    if ( chan_it != _channels.end() )
        chan = &chan_it->second;
    else
        chan = NULL;

    return ( chan );
}

void Server::_privmsgChan( Client client , string target , string origin , string text ) {
    Channel *channel = _findChanByName(target) ;
            if (channel )
                sendMsg( *channel , origin , text);
            else
                sendMsg(ERR_NOSUCHCHANNEL , client , "No such channel in server");
}

void Server::_privmsgClient( Client client , string target , string origin , string text ) {
    string msg_to_client = origin + "PRIVMSG " + target + " :" + text;
    Client *tar = _findClientByNick(_clientMap, target);
    
    if ( tar ) {
        send( tar->getClientSocket(), msg_to_client.c_str() , msg_to_client.length(), MSG_DONTWAIT);
    } else {
        sendMsg(ERR_NOSUCHNICK , client , "No such nick in server");
    }
}

void Server::_privmsg( string full_command , Client client) {

    DEBUG_MSG("####################START MESSAGE COMMAND#############################");
    DEBUG_MSG("COMMAND: " << full_command);
    string target = findTargetName( full_command );

    string text = findText( full_command );
    std::string actual_nick;
    if (!text.compare(""))
        sendMsg( ERR_NOTEXTTOSEND , client , "No text to send.");
    else {
        if (target[0] == '#') {
            _privmsgChan( client , target , message::getMsgOrigin( client ) , text );
            // Channel *channel = _findChanByName(target) ;
            // if (channel )
            //     sendMsg( *channel , message::getMsgOrigin( client ) , text);
            // else
            //     sendMsg(ERR_NOSUCHCHANNEL , client , "No such channel in server");
        } else {
            _privmsgClient( client , target , message::getMsgOrigin( client ) , text );
                // string msg_to_client = message::getMsgOrigin( client ) + "PRIVMSG " + target + " :" + text;
                // Client *tar = _findClientByNick(_clientMap, target);
                // if ( tar ) {
                //     send( tar->getClientSocket(), msg_to_client.c_str() , msg_to_client.length(), MSG_DONTWAIT);
                // } else {
                //     sendMsg(ERR_NOSUCHNICK , client , "No such nick in server");
                // }
        }
    }

    DEBUG_MSG("####################END MESSAGE COMMAND###############################");
}
