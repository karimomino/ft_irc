#include "utils.hpp"

void utils::trim( std::string& str, const std::string& delm ) {
    size_t start = str.find_first_not_of( delm );
    size_t end = str.find_last_not_of( delm );
    if ( start == std::string::npos )
        start = 0;
    if ( end == std::string::npos )
        end = str.length();
    str = str.substr( start, end - start + 1 );
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
