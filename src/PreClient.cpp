#include "PreClient.hpp"


PreClient::PreClient( int socketFd, struct sockaddr_in* hint ) : AClient( socketFd , hint ) {	
}

PreClient::~PreClient( void ) {
    close( getSocketFd() );
    std::cout << "Pre client destructor called" << std::endl;
}

