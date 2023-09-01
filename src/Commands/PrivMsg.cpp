#include "Commands/PrivMsg.hpp"

PrivMsg::PrivMsg( Server& ircServ ) : ICommand( ircServ ) {

}

PrivMsg::~PrivMsg( void ) {

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
    if (split_command.size() > 1) {
        std::vector< std::string >::iterator split_it = split_command.begin();
        
        content = *(++split_it);
        content.erase(content.begin());
        while ( ++split_it  != split_command.end())
            content += " " + *split_it;
    }

    return ( content );
}

static bool isChannel(const std::string& target) {
    bool isChan = true;

    if (target[0] != '#' && target[0] != '&')
        isChan = false;
    return isChan;
}

void PrivMsg::execute( AClient* const client, const std::string& rawCommand ){
    std::string target = findTargetName(rawCommand);
    std::string content = findContent(rawCommand);
    std::string msg = client->getOrigin() + " PRIVMSG " + target + " :" + content + "\r\n";

    if (content.empty())
        client->addMsg(ERR_NEEDMOREPARAMS(_ircServ.getIp() , client->getNick() + " PRIVMSG"));
    else if (isChannel(target)) {
        std::map<const std::string, Channel*>::iterator chan = _ircServ._channels.find(target);
        if (chan != _ircServ._channels.end())
            chan->second->addMsg(client->getNick() , msg);
        else
            client->addMsg(ERR_NOSUCHCHANNEL(_ircServ.getIp() , target));
    } else {
        AClient* cli = _ircServ._findClientByNick(target);
        if (cli)
            cli->addMsg(msg);
        else
            client->addMsg(ERR_NOSUCHNICK(_ircServ.getIp() , target));
    }
}

void PrivMsg::clearCmd( void ){};