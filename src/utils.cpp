#include "Server.hpp"

void trim( std::string& str ) {
	for (std::string::iterator i = str.begin(); i != str.end(); i++)
	{
		if ( std::isspace(*i) || !std::isprint(*i) ) {
			str.erase(i);
			i = str.begin();
		}
		else
			break;
	}

	for (std::string::iterator i = str.end(); i != str.begin(); i--)
	{
		if ( std::isspace(*i) || !std::isprint(*i) ) {
			str.erase(i);
			i = str.end();
		}
		else
			break;
	}
}

std::vector<std::string> splitDelim( std::string str , std::string delim) {
	std::string strSection;
	std::vector<std::string> sections;

	size_t pos = 0;
	while ((pos = str.find(delim)) != std::string::npos) {
		strSection = str.substr(0, pos);
		trim(strSection);
		sections.push_back( strSection );
		str.erase(0, pos + delim.length());
	}
	pos = str.find(delim);
	strSection = str.substr(0, pos);
	trim(strSection);
	sections.push_back(strSection);
    return ( sections );
	
}
