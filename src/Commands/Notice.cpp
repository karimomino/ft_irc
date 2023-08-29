#include "Commands/Notice.hpp"

Notice::Notice( Server& ircServ ) : ICommand( ircServ ) {

}

Notice::~Notice( void ) {

}

static std::string  findTargetName( const std::string& full_command ) {
    std::string target;
    std::vector< std::string > split_command = utils::split( full_command , " ");
    std::vector< std::string >::iterator split_it = split_command.begin();

    target = *split_it;
    return ( target );
}

static std::string  findContent( const std::string& full_command ) {
    std::string content;
    std::vector< std::string > split_command = utils::split( full_command , " ");
    std::vector< std::string >::iterator split_it = split_command.begin();
    
    content = *(++split_it);
    content.erase(content.begin());
    while ( ++split_it  != split_command.end())
        content += " " + *split_it;

    return ( content );
}

static bool isChannel(const std::string& target) {
    bool isChan = true;

    if (target[0] != '#' && target[0] != '&')
        isChan = false;
    return isChan;
}

void Notice::execute( AClient* const client, const std::string& rawCommand ){
    std::string target = findTargetName(rawCommand);
    std::string content = findContent(rawCommand);
    std::string msg = client->getOrigin() + " NOTICE " + target + " :" + content + "\r\n";

    if (isChannel(target)) {
        std::map<const std::string, Channel*>::iterator chan = _ircServ._channels.find(target);
        if (chan != _ircServ._channels.end())
            chan->second->addMsg(client->getNick() , msg);
    } else {
        AClient* cli = _ircServ._findClientByNick(target);
        if (cli)
            cli->addMsg(msg);
    }
}

void Notice::clearCmd( void ){};