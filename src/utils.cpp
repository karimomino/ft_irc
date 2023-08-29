#include "utils.hpp"

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

std::vector<std::string> utils::split( std::string str , std::string delm) {
    size_t pos = 0;
    std::string token;
    std::vector<std::string> list;
    while ( ( pos = str.find( delm ) ) != std::string::npos ) {
        token = str.substr(0, pos);
        if ( !token.empty() )
            list.push_back( token );
        str.erase(0, pos + delm.length());
    }
    if (!str.empty())
        list.push_back(str);
    return ( list );
}
