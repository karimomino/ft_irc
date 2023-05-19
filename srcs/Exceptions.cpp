#include "Exceptions.hpp"

const char * SocketError::what( void ) const throw() {
    return ( "[ Socket Error ]" );
}

const char * BindError::what( void ) const throw() {
    return ( "[ Bind Error ]" );
}

const char * ClientError::what( void ) const throw() {
    return ( "[ Bind Error ]" );
}

