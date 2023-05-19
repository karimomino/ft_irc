#pragma once

#include <iostream>
#include <exception>
#include <cerrno>
#include <string.h>

class SocketError : public std::exception {
    const char * what( void ) const throw();
};

class BindError : public std::exception {
    const char * what( void ) const throw();
};

class ClientError : public std::exception {
    const char * what( void ) const throw();  
};
