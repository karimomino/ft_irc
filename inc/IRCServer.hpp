#pragma once

# include <iostream>

class IRCServer {
    private:
        int         const _port;
        std::string const _password;

        // container for clients so it's easer to handle
        // could be other container later
        // std::vector< Client > _clients;

    public:
        IRCServer( int port, std::string const password );
        ~IRCServer( void );
};
