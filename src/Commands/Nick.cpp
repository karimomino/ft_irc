#include "Commands/Nick.hpp"

Nick::Nick( Server& ircServ ) : ICommand( ircServ ) {

}

Nick::~Nick( void ) {

}

static bool nickAuthorizedChars(const std::string& nick) {
    bool isAuthorized = true;
    std::string authorizedChars("qazwsxedcrfvtgbyhnujmikolpQAZWSXEDCRFVTGBYHNUJMIKOLP`|^_-{}[]\\1234567890");

    if ( !std::isalpha(nick[0]) || nick.find_first_not_of(authorizedChars) != std::string::npos )
        isAuthorized = false;
    return (isAuthorized);
}

void Nick::execute( AClient* const client, const std::string & rawCommand ){
    PreClient *target = dynamic_cast<PreClient *>(client);
    if (nickAuthorizedChars(rawCommand))
        client->setNick(rawCommand);
    else {
        client->addMsg(ERR_ERRONEUSNICKNAME(client->getNick() + " " + rawCommand));
        if (target)
            client->setPurge(true);
    }
    if ( target && !client->getNick().empty() && !client->getUser().empty() && !client->getPass().empty() ) {
        _ircServ._addClient(client);
        _ircServ._clients.erase(client->getSocketFd());
        delete client;
	}
}

void Nick::clearCmd( void ) {
    
};