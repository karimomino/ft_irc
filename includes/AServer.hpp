/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AServer.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 01:58:33 by kamin             #+#    #+#             */
/*   Updated: 2023/08/11 02:01:09 by kamin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASERVER_HPP
# define ASERVER_HPP
#include <iostream>
#include <string>
class AServer
{
    protected:
        const int                            _port;
        const std::string                    _pass;
        int                                  _listen_socket;
        std::string                          _ip_string;
    public:
        AServer( const int port, const std::string pass );
        ~AServer();
};

#endif