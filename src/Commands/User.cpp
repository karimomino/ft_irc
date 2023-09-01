#include "Commands/User.hpp"

User::User( Server& ircServ ) : ICommand( ircServ ) {

}

User::~User( void ) {

}

static std::string extractUser( const std::string& rawCommand) {
    std::vector<std::string> tokens = utils::split(rawCommand , ":");
    std::vector<std::string>::iterator it = tokens.begin();
    std::string user;
    if (tokens.size() < 2 )
        user = *it;
    else
        user = *(++it);
    return (user);
}

void User::execute( AClient* const client, const std::string & rawCommand ){
    if (rawCommand.empty())
        client->addMsg(ERR_NEEDMOREPARAMS(_ircServ.getIp() , client->getNick() + " USER"));
    else {
        PreClient *target = dynamic_cast<PreClient *>(client);
        client->setUser(extractUser(rawCommand));
        if ( target && !client->getUser().empty() && !client->getNick().empty() && !client->getPass().empty() ) {
            _ircServ._clients.erase(client->getSocketFd());
            _ircServ._addClient(client);
            delete client;
        }
    }
}

void User::clearCmd( void ){};
