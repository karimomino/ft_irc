/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pong.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 08:40:40 by kamin             #+#    #+#             */
/*   Updated: 2023/07/22 16:10:17 by kamin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

void    Server::_pong( Client client) {
    string pong_message = ":127.0.0.1 PONG 127.0.0.1 : 127.0.0.1\r\n";
    send( client.getClientSocket(), pong_message.c_str(), pong_message.length(), 0x80 );
    std::cout << "ponged the client" << std::endl;
}