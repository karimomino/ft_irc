/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamin <kamin@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 16:06:19 by kamin             #+#    #+#             */
/*   Updated: 2023/05/17 16:15:00 by kamin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

std::vector<std::string> split_string( std::string str ) {
    std::stringstream buff(str);
	std::string word;
	std::vector<std::string> wordList;

	while( std::getline(buff, word, ' ') )
		wordList.push_back(word);
    return ( wordList );
}