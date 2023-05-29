/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 18:56:16 by kamin             #+#    #+#             */
/*   Updated: 2023/05/27 19:05:04 by kamin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>

class Message {
	private:
		std::string	_finalMessage;
		std::string	_prefix;
		std::string	_command;
		std::string	_arguments;
	public:
		Message( std::string nick, std::string user, std::string host, std::vector<std::string>::iterator tokens );

};