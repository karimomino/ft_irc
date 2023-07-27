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
		wordList.push_back(word);
		str.erase(0, pos + delim.length());
	}
	pos = str.find(delim);
	word = str.substr(0, pos);
	wordList.push_back(word);
    return ( wordList );
}

std::vector<std::string const> utils::csplit( std::string const & str, std::string const & delm ) {
    std::string tmp;
    std::stringstream ss( str );
    std::vector<std::string const> strList;

    while ( getline(ss, tmp, delm[0] ) ) {
	strList.push_back( tmp );
    }

 //    if ( str.empty() )
	// return ( strList );
	//
 //    size_t  start = 0;
 //    while ( true ) {
	// size_t idx = str.find( delm, start );
	// if ( idx == string::npos )
	//     break ;
	// size_t len = idx - start;
	// strList.push_back( str.substr( start, len ) );
	// start += len + delm.size();
 //    }
 //    strList.push_back( str.substr( start ) );

    return ( strList );
}
