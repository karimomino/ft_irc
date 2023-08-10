/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 16:06:19 by kamin             #+#    #+#             */
/*   Updated: 2023/08/10 15:06:30 by kamin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <sstream>
#include <string>

std::string utils::ft_itoa( int num ) {
	std::stringstream ss;

	ss << num;
	return (ss.str());
}

std::vector<std::string> utils::split( std::string str , string const & delim ) {
	std::string word;
	std::vector<std::string> wordList;

	size_t pos = 0;
	while ((pos = str.find(delim)) != std::string::npos) {
		word = str.substr(0, pos);
		wordList.push_back( word );
		str.erase(0, pos + delim.length());
	}
	pos = str.find(delim);
	word = str.substr(0, pos);
	wordList.push_back(word);
    return ( wordList );
}

std::vector<std::string> utils::splitTrim( std::string str , string const & delim ) {
	std::string word;
	std::vector<std::string> wordList;

	size_t pos = 0;
	while ((pos = str.find(delim)) != std::string::npos) {
		word = str.substr(0, pos);
		trim(word);
		wordList.push_back( word );
		str.erase(0, pos + delim.length());
	}
	pos = str.find(delim);
	word = str.substr(0, pos);
	trim(word);
	wordList.push_back(word);
    return ( wordList );
}

std::vector<std::string> const utils::csplit( std::string const & str, std::string const & delm ) {
    std::string tmp;
    std::stringstream ss( str );
    std::vector<std::string> strList;

    while ( getline(ss, tmp, delm[0] ) ) {
	strList.push_back( tmp );
    }

    return ( strList );
}

void utils::trim( std::string & str ) {
	// std::string::iterator str_it = str.begin();

	for (std::string::iterator i = str.begin(); i != str.end(); i++)
	{
		if ( std::isspace(*i) || !std::isprint(*i) ) {
			str.erase(i);
			i--;
		}
	}
}
