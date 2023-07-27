/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 20:42:02 by kamin             #+#    #+#             */
/*   Updated: 2023/07/22 13:54:44 by kamin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

// ChanVector::iterator Server::_findChannel( ChanVector &channels , std::string name ) const {
//
//     ChanVector::iterator    chan_it = channels.begin();
//
//     for (chan_it = channels.begin(); chan_it != channels.end(); chan_it++) {
//         if ( chan_it->getName() == name )
//             break ;
//     }
//
//     return ( chan_it );
// }

Client  *Server::_findClientByNick( std::map<int, Client> &clients , string nick ) const {

    std::map<int, Client>::iterator    client_it = clients.begin();
    Client                    *client = NULL;

    for (client_it = clients.begin(); client_it != clients.end(); client_it++) {
        if ( client_it->second.getNick() == nick ) {
            client = &client_it->second;
            break ;
        }
    }

    return ( client );
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

void    Server::_joinChannel( Client const & client , std::string name ) {

    string msg;
    // name = name.substr( 1, name.length() - 1 );
    // figure out why below commented line segfaults
    // ChanVector::iterator chan = _findChannel( _channels, name);
    // TODO: join multiple channels in a single command
    // TODO: Check invite only and key
    // if ( _findChannel( _channels, name) == _channels.end() )
    if ( _channels.find( name ) == _channels.end() )
    {
        _channels.insert( std::pair< std::string, Channel>( name, Channel( name, "Default Topic" , "" , false , true ) ) );

        // std::cout << "address new channel: [" << &client << "] - [" << client.getNick() << std::endl;
        _channels.find( name )->second.addUser( "@" + client.getNick(), client );

        std::cout << "Attempting to join new channel ..." << std::endl;

        msg = ":" + client.getNick() + "!" + client.getUser() + "@" + client.getIp() + " JOIN :" + name + "\r\n";
        send(client.getClientSocket(), msg.c_str() , msg.length()  , 0x80);

        std::cout << "full join msg: " << msg << std::endl;

        msg = ":localhost MODE " + name + " +t\r\n";
        send(client.getClientSocket(), msg.c_str() , msg.length()  , 0x80);
    } else {
        // std::cout << "address found channel: [" << &client << "] - [" << client.getNick() << std::endl;
        _channels.find( name )->second.addUser( client.getNick(), client );
        msg = ":" + client.getNick() + "!" + client.getUser() + "@" + client.getIp() + " JOIN " + name + "\r\n";
        send(client.getClientSocket(), msg.c_str() , msg.length()  , 0x80);

        // msg = ":localhost 332 " + client.getNick() + " " + name + " :" + _findChannel( _channels, name)->getTopic() + "\r\n";
        msg = ":localhost 332 " + client.getNick() + " " + name + " :" + _channels.find( name )->second.getTopic() + "\r\n";
        send(client.getClientSocket(), msg.c_str(), msg.length(), 0);
        // _broadcastJoin(client, *_findChannel( _channels, name), name);
        _broadcastJoin(client, _channels.find( name )->second, name);
    }
    // msg = ":localhost 353 " + client.getNick() + " = " + name + " :" + _findChannel( _channels, name)->getUsersStr() + "\r\n";
    msg = ":localhost 353 " + client.getNick() + " = " + name + " :" + _channels.find( name )->second.getUsersStr() + "\r\n";
    send(client.getClientSocket(), msg.c_str(), msg.length(), 0);

    msg = ":localhost 366 " + client.getNick() + " " + name + " :End of /NAMES list\r\n";
    send(client.getClientSocket(), msg.c_str(), msg.length(), 0);
}
