#include "Client.hpp"

Client::Client( int socketfd ) : _socketfd( socketfd ) {}

Client::~Client( void ) {}

int Client::getSocketfd( void ) const {
    return ( this->_socketfd );
}


// input = "ab>  |"
// input[0] = 'a';
// input[1] = 'b';
// input[2] = '>';
// input[3] = ' ';
// input[4] = ' ';
// input[5] = '|';
// strchr( input[i], "<>|");
//
// 
// while ( input[i] == '>' ) {
//      i++;
//      count++;
// }
// if ( count > 2 )
//     return ( error );
// while ( isspace( input[i]))
//      i++;
// if ( input[i] == '\0' || strchr( input[i], "<>|" ))
//      return error
// return sah
