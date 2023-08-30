#include "Client.hpp"


Client::Client( const AClient& preClient ) : AClient( preClient ) {
	std::string welcome_001 = ":" + _ip + " 001 " + getNick() + " :Welcome to FT_IRC " + getNick() + "!" + getUser() + "@" + getIp() + "\r\n";
	std::string your_host = ":" + _ip + " 002 " + getNick() + " :Your host is 127.0.0.1, running version idk anymore\r\n";
	std::string server_created = ":" + _ip + " 003 " + getNick() + " :Server created sometime this year.\r\n";
    std::string modes = ":" + _ip + " 004 " + getNick() + " " + _ip + " V-unknown i iotk ko\r\n";
    std::string motdStart = ":" + _ip + " 375 " + getNick() + " :Message of the Day\r\n";
    std::string motd = ":" + _ip + " 372 " + getNick() + " :" + " ▄▄▄▄    ▒█████   ███▄    █   ██████  ▒█████   ██▓ ██▀███     ▓█████  ██▓     ██▓     ██▓ ▒█████  ▄▄▄█████▓\r\n";
	std::string motd1= ":" + _ip + " 372 " + getNick() + " :" + "▓█████▄ ▒██▒  ██▒ ██ ▀█   █ ▒██    ▒ ▒██▒  ██▒▓██▒▓██ ▒ ██▒   ▓█   ▀ ▓██▒    ▓██▒    ▓██▒▒██▒  ██▒▓  ██▒ ▓▒\r\n";
	std::string motd2= ":" + _ip + " 372 " + getNick() + " :" + "▒██▒ ▄██▒██░  ██▒▓██  ▀█ ██▒░ ▓██▄   ▒██░  ██▒▒██▒▓██ ░▄█ ▒   ▒███   ▒██░    ▒██░    ▒██▒▒██░  ██▒▒ ▓██░ ▒░\r\n";
	std::string motd3= ":" + _ip + " 372 " + getNick() + " :" + "▒██░█▀  ▒██   ██░▓██▒  ▐▌██▒  ▒   ██▒▒██   ██░░██░▒██▀▀█▄     ▒▓█  ▄ ▒██░    ▒██░    ░██░▒██   ██░░ ▓██▓ ░ \r\n";
	std::string motd4= ":" + _ip + " 372 " + getNick() + " :" + "░▓█  ▀█▓░ ████▓▒░▒██░   ▓██░▒██████▒▒░ ████▓▒░░██░░██▓ ▒██▒   ░▒████▒░██████▒░██████▒░██░░ ████▓▒░  ▒██▒ ░ \r\n";
	std::string motd5= ":" + _ip + " 372 " + getNick() + " :" + "░▒▓███▀▒░ ▒░▒░▒░ ░ ▒░   ▒ ▒ ▒ ▒▓▒ ▒ ░░ ▒░▒░▒░ ░▓  ░ ▒▓ ░▒▓░   ░░ ▒░ ░░ ▒░▓  ░░ ▒░▓  ░░▓  ░ ▒░▒░▒░   ▒ ░░   \r\n";
	std::string motd6= ":" + _ip + " 372 " + getNick() + " :" + "▒░▒   ░   ░ ▒ ▒░ ░ ░░   ░ ▒░░ ░▒  ░ ░  ░ ▒ ▒░  ▒ ░  ░▒ ░ ▒░    ░ ░  ░░ ░ ▒  ░░ ░ ▒  ░ ▒ ░  ░ ▒ ▒░     ░    \r\n";
	std::string motd7= ":" + _ip + " 372 " + getNick() + " :" + " ░    ░ ░ ░ ░ ▒     ░   ░ ░ ░  ░  ░  ░ ░ ░ ▒   ▒ ░  ░░   ░       ░     ░ ░     ░ ░    ▒ ░░ ░ ░ ▒    ░      \r\n";
	std::string motd8= ":" + _ip + " 372 " + getNick() + " :" + " ░          ░ ░           ░       ░      ░ ░   ░     ░           ░  ░    ░  ░    ░  ░ ░      ░ ░           \r\n";
	std::string motd9= ":" + _ip + " 372 " + getNick() + " :" + "      ░                                                                                                    \r\n";
    std::string motdEnd = ":" + _ip + " 376 " + getNick() + " :End of /MOTD command.\r\n";
    std::string notice = ":" + _ip + " NOTICE " + getNick() + " :hope you enjoy the server like we enjoyed the project\r\n";
    std::string mode = getOrigin() + " MODE " + getNick() + " :+i\r\n";
    std::string whoIsUser = ":" + _ip + " 311 " + getNick() + " " + getUser() + " " + getIp() + " * :" + getUser() + "\r\n";
    std::string whoIsServer = ":" + _ip + " 312 " + getNick() + " " + getUser() + " " + getIp() + " :1337 h4x0r w0rld\r\n";
    this->addMsg(welcome_001);
    this->addMsg(your_host);
    this->addMsg(server_created);
    this->addMsg(modes);
    this->addMsg(motdStart + motd + motd1 + motd2 + motd3 + motd4 + motd5 + motd6 + motd7 + motd8 + motd9 + motdEnd);
    this->addMsg(notice);
    this->addMsg(mode);
    this->addMsg(whoIsUser);
    this->addMsg(whoIsServer);
}

Client::~Client( void ) {
    close( getSocketFd() );
    std::cout << "Client destructor called" << std::endl;
}
