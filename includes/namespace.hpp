#pragma once

#include <iostream>
#include <vector>
#include "Server.hpp"

namespace server {}

namespace utils {
std::vector<std::string> split( std::string, std::string const & delm );
std::vector<std::string> const csplit( std::string const &, std::string const & delm );
std::string ft_itoa( int num );
}

namespace message {
std::string const getMsgOrigin( Client const & client );
std::vector<std::string> const extractNickList( std::string const & msg );
std::vector<std::string> const extractArgs( std::string const & msg, std::string const & delm );
}
