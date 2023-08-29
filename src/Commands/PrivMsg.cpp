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
    std::vector< std::string >::iterator split_it = split_command.begin();

    content = *(*(++split_it)).erase((*split_it).begin());
    content.push_back( ' ');
    while ( split_it  != split_command.end()) {
        content += *split_it + " ";
        split_it++;
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

    if (isChannel(target)) {
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
    // allowed chars in nick [a-z A-Z `|^_-{}[]\ 0-9] and must start with letter

    /*
    Channels names are strings (beginning with a '&' or '#' character) of
    length up to 200 characters.  Apart from the the requirement that the
    first character being either '&' or '#'; the only restriction on a
    channel name is that it may not contain any spaces (' '), a control G
    (^G or ASCII 7), or a comma (',' which is used as a list item
    separator by the protocol).
   */

  // if no channel ERR_NOSUCHCHANNEL()

}

void PrivMsg::clearCmd( void ){};