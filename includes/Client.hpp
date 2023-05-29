/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 19:56:23 by kamin             #+#    #+#             */
/*   Updated: 2023/05/27 17:32:31 by kamin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <map>

class Client {
    private:
        int            _client_socket;
        std::string    _nick;
        std::string    _user;
        std::string    _pass;
        size_t        _msgSent;
        bool        _isRegistered;

    public:
        Client( int listen_socket, struct sockaddr_in hint );
        // ~Client( void );
        int getClientSocket( void );
        size_t    getMsgSent( void );
        int        joinChannel( std::string chan );
        void    incMsgSent( void );
        std::string    getPass( void );
        void    setPass( std::string newPass );
        std::string    getNick( void );
        void    setNick( std::string newNick );
        std::string    getUser( void );
        void    setUser( std::string newUser );
        bool    getRegisteredStatus( void );
};
