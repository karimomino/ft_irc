/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ommohame < ommohame@student.42abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 16:06:19 by kamin             #+#    #+#             */
/*   Updated: 2023/06/09 00:39:40 by ommohame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <sstream>
#include <string>

std::vector<std::string> split_string( std::string str , string delim ) {
    // std::stringstream buff(str);
	std::string word;
	std::vector<std::string> wordList;

	// while( std::getline(buff, word, delim) )
	// 	wordList.push_back(word);
	// std::cout << "Splitting string: " << str << "\twith this delim >>" << delim << "<<" << std::endl;

	size_t pos = 0;
	while ((pos = str.find(delim)) != std::string::npos) {
		word = str.substr(0, pos);
		wordList.push_back(word);
		str.erase(0, pos + delim.length());
	}
	pos = str.find(delim);
	word = str.substr(0, pos);
	wordList.push_back(word);
    return ( wordList );
}

std::string	ft_itoa( int num ) {
	std::stringstream ss;

	ss << num;
	return (ss.str());
}
