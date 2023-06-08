/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 20:42:02 by kamin             #+#    #+#             */
/*   Updated: 2023/06/08 11:36:21 by kamin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

ChanVector::iterator Server::_findChannel( ChanVector &channels , std::string name ) const {

    ChanVector::iterator    chan_it = channels.begin();

    for (chan_it = channels.begin(); chan_it != channels.end(); chan_it++) {
        if ( chan_it->getName() == name )
            break ;
    }

    return ( chan_it );
}

void    Server::_broadcastJoin( Client client , Channel chan , string name ) {
    std::vector< std::string > tmp_nicks = chan.getNicks();
    std::string msg = ":" + client.getNick() + "!" + client.getUser() + "@" + client.getIp() + " JOIN :" + name + "\r\n";
    std::string actual_nick;
    for (size_t i = 0; i < tmp_nicks.size(); i++) {
        if ( tmp_nicks[i][0] == '@' )
            actual_nick = tmp_nicks[i].substr( 1, tmp_nicks[i].length() - 1 );
        else 
            actual_nick = tmp_nicks[i];

        for ( std::map<int, Client>::iterator client_it = _clientMap.begin() ; client_it != _clientMap.end(); client_it++) {
            if ( client_it->second.getNick() == actual_nick )
            {
                send(client_it->second.getClientSocket() , msg.c_str() , msg.length(), 0x80);
                break;
            }
        }
    }

}

void    Server::_joinChannel( Client client , std::string name ) {

    string msg;
    // figure out why below commented line segfaults
    // ChanVector::iterator chan = _findChannel( _channels, name);
    // TODO: join multiple channels in a single command
    // TODO: Check invite only and key
    if ( _findChannel( _channels, name) == _channels.end() )
    {
        _channels.push_back( Channel( name, "Default Topic" , "" , false , true ) );
        _findChannel( _channels, name)->addUser( "@" + client.getNick() );
        std::cout << "Attempting to join new channel ..." << std::endl;

        msg = ":" + client.getNick() + "!" + client.getUser() + "@" + client.getIp() + " JOIN :" + name + "\r\n";
        send(client.getClientSocket(), msg.c_str() , msg.length()  , 0x80);

        std::cout << "full join msg: " << msg << std::endl;

        msg = ":localhost MODE " + name + " +t\r\n";
        send(client.getClientSocket(), msg.c_str() , msg.length()  , 0x80);
    } else {
        _findChannel( _channels, name)->addUser( client.getNick() );
        msg = ":" + client.getNick() + "!" + client.getUser() + "@" + client.getIp() + " JOIN " + name + "\r\n";
        send(client.getClientSocket(), msg.c_str() , msg.length()  , 0x80);
        
        msg = ":localhost 332 " + client.getNick() + " " + name + " :" + _findChannel( _channels, name)->getTopic() + "\r\n";
        send(client.getClientSocket(), msg.c_str(), msg.length(), 0);
        _broadcastJoin(client, *_findChannel( _channels, name), name);
    }
    msg = ":localhost 353 " + client.getNick() + " = " + name + " :" + _findChannel( _channels, name)->getUsersStr() + "\r\n";
    send(client.getClientSocket(), msg.c_str(), msg.length(), 0);

    msg = ":localhost 366 " + client.getNick() + " " + name + " :End of /NAMES list\r\n";
    send(client.getClientSocket(), msg.c_str(), msg.length(), 0);
}