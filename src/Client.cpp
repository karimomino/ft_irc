#include "Client.hpp"


Client::Client( const AClient& preClient ) : AClient( preClient ) {
	std::string welcome_001 = ":" + _ip + " 001 " + getNick() + " :Welcome to FT_IRC " + getNick() + "!" + getUser() + "@" + getIp() + "\r\n";
	std::string your_host = ":" + _ip + " 002 " + getNick() + " :Your host is 127.0.0.1, running version idk anymore\r\n";
	std::string server_created = ":" + _ip + " 003 " + getNick() + " :Server created sometime this year.\r\n";
    this->addMsg(welcome_001);
    this->addMsg(your_host);
    this->addMsg(server_created);
}

Client::~Client( void ) {
    close( getSocketFd() );
    std::cout << "Client destructor called" << std::endl;
}

