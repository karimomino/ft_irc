#include "PreClient.hpp"


PreClient::PreClient( int socketFd, struct sockaddr_in* hint ) : AClient( socketFd , hint ) {
    int addrlen = sizeof(hint);
    _socketFd = accept( socketFd , (sockaddr *)&hint , (socklen_t*)&addrlen);
}

PreClient::~PreClient( void ) {
    std::cout << "Pre client destructor called" << std::endl;
}

