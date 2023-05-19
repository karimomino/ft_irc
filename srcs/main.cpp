#include "ircserv.hpp"

static bool validateInput( std::string const & port, std::string const & password ) {
    if ( port.length() > 5 || port.find_first_not_of( "1234567890" ) != std::string::npos ) {
        std::cerr << "Error: Invalid port" << std::endl;
        return ( false );
    }
    (void)password;
    return ( true );
}

bool ircserv( std::string const & port, std::string const & password ) {
    try {
        Server server( atol(port.c_str()), password );
        server.initServer();
        server.handleClient();
    } catch ( std::exception & e ) {
        std::cerr << e.what();
        if ( errno )
            std::cerr << ": " << strerror( errno );
        std::cerr << std::endl;
    }
    return ( true );
}

int main( int ac, char ** av ) {
    if ( ac != 3 || !validateInput( av[1], av[2] )) {
        std::cerr << "Error: try ./ircserv <port> <password>" << std::endl;
        return ( false );
    }
    return ( ircserv( av[1], av[2] ));
}
