#pragma once

#include "Server.hpp"
#include "PreClient.hpp"
#include <queue>
#include <unistd.h>

class Client : public PreClient {
private:
    std::queue<const std::string> _msgs;

public:
    Client( const PreClient& );
    ~Client( void );

    /* Methods */
    void addMsg( const std::string& );

    /* Getters */

    /* Setters */
};
