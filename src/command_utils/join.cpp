/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 20:42:02 by kamin             #+#    #+#             */
/*   Updated: 2023/08/05 17:25:24 by kamin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"


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
    std::vector< std::string const> tmp_nicks = chan.getNicks();
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
                send(client_it->second.getClientSocket() , msg.c_str() , msg.length(), MSG_DONTWAIT);
                break;
            }
        }
    }

}


static void findKey( std::vector<std::string> &chanListKeys , std::vector<std::string> const & chanListKeysCandidates ) {
    vec_it chanListKeysIt = chanListKeys.begin();
    for (vec_const_it i = chanListKeysCandidates.begin(); i != chanListKeysCandidates.end(); i++)
    {
        *chanListKeysIt = *i;
        chanListKeysIt++;
    }
}

void Server::_joinCreate( Client const & client , string chan, string topic , string key , bool inv , bool top ) {
    string msg;
    std::pair<chan_map::iterator , bool> channel = _channels.insert( std::pair< std::string, Channel>( chan, Channel( chan, topic , key , inv , top ) ) );
    if ( _allowedToJoin( client , channel.first->second , key) ) {
        _channels.find( chan )->second.addUser( "@" + client.getNick(), client );
        msg = ":" + client.getNick() + "!" + client.getUser() + "@" + client.getIp() + " JOIN :" + chan + "\r\n";
        send(client.getClientSocket(), msg.c_str() , msg.length()  , MSG_DONTWAIT);
        DEBUG_MSG("SEND JOIN MESSA: " << msg << std::endl);

        msg = ":localhost MODE " + chan + " +t\r\n";
        send(client.getClientSocket(), msg.c_str() , msg.length()  , MSG_DONTWAIT);
        _sendNames(client , chan);
    }


}

void Server::_joinExistingChannel( Client const & client , string chan ) {
    DEBUG_MSG("joining existing channel" << std::endl);
    string msg;
    _channels.find( chan )->second.addUser( client.getNick(), client );
    msg = ":" + client.getNick() + "!" + client.getUser() + "@" + client.getIp() + " JOIN :" + chan + "\r\n";
    DEBUG_MSG("SEND JOIN MESSA: " << msg << std::endl);
    send(client.getClientSocket(), msg.c_str() , msg.length()  , MSG_DONTWAIT);
    msg = ":" + _ip_string + " 332 " + client.getNick() + " " + chan + " :" + _channels.find( chan )->second.getTopic() + "\r\n";
    send(client.getClientSocket(), msg.c_str(), msg.length(), MSG_DONTWAIT);
    msg = ":" + _ip_string + " 332 " + client.getNick() + " " + chan + "dan!~d@localhost 1547691506\r\n";
    send(client.getClientSocket(), msg.c_str(), msg.length(), MSG_DONTWAIT);
            // :irc.example.com 333 alice #test dan!~d@localhost 1547691506
    _broadcastJoin(client, _channels.find( chan )->second, chan);
    _sendNames(client , chan);
}

void Server::_sendNames( Client const & client , string chan) {
    string msg = ":" + _ip_string + " 353 " + client.getNick() + " = " + chan + " :" + _channels.find( chan )->second.getUsersStr() + "\r\n";
    send(client.getClientSocket(), msg.c_str(), msg.length(), MSG_DONTWAIT);
    msg = ":" + _ip_string + " 366 " + client.getNick() + " " + chan + " :End of /NAMES list\r\n";
    send(client.getClientSocket(), msg.c_str(), msg.length(), MSG_DONTWAIT);
}

bool Server::_allowedToJoin( Client client , Channel chan , string key ) const {
    bool allowed = true;
    if ( chan.isInviteOnly() ) {
        if ( !chan.isInvited( client.getNick() ) ){ 
            allowed = false;
            //:irc.example.com 473 alice #test :Cannot join channel (+i)
            string msg = ":" + _ip_string + " 473 " + client.getNick() + " " + chan.getName() + " :Cannot join channel (+i)\r\n";
            send( client.getClientSocket() , msg.c_str() , msg.length() , MSG_DONTWAIT );
        }
    }

    if ( allowed ){
        if ( key.compare(chan.getKey()) ) {
            allowed = false;
            //:irc.example.com 475 adrian #test :Cannot join channel (+k) - bad key
            string msg = ":" + _ip_string + " 475 " + client.getNick() + " " + chan.getName() + " :Cannot join channel (+k) - bad key\r\n";
        }
    }

    return ( allowed );
}
void    Server::_joinChannel( Client const & client , string chans , string keys ) {

    string msg;
    // TODO: Check invite only and key
    std::vector<std::string> chanList = utils::split( chans , "," );
    std::vector<std::string> chanListKeysCandidates = utils::split( keys , "," );
    std::vector<string> chanListKeys(chanList.size() , "");
    findKey( chanListKeys , chanListKeysCandidates );
    vec_it keysIt = chanListKeys.begin();
    for (std::vector<std::string>::iterator i = chanList.begin(); i != chanList.end(); i++)
    {
        string chan = *i;
        string key = *keysIt;
        chan_map::iterator channel = _channels.find( chan );
        if ( channel == _channels.end() ) {
            _joinCreate(client , chan , "Default Topic" , key , false , true );
        } else if ( _allowedToJoin( client , channel->second , key ) ) {
            _joinExistingChannel( client , chan);
        }
        keysIt++;
    }
}
