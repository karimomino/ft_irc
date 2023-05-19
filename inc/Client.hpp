#pragma once

# include <iostream>

class Client {
    private:
        int     _socketfd;

    public:
        Client( int socketfd );
        ~Client( void );


        int getSocketfd( void ) const;
};
