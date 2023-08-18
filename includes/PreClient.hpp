#pragma once

#include "Server.hpp"
#include "AClient.hpp"
#include <queue>
#include <unistd.h>

class PreClient : public AClient {

public:
    PreClient( int socketFd, struct sockaddr_in* hint );
    ~PreClient( void );

    /* Methods */

    /* Getters */

    /* Setters */
};
