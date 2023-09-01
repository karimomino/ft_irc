#include "Commands/Topic.hpp"

Topic::Topic( Server& ircServ ) : ICommand( ircServ ) {}

Topic::~Topic( void ) {

}

std::vector<std::string> findTargetTopic(const std::string& rawCommand) {
    std::string topic;
    std::vector<std::string> targetTopic;
    std::vector<std::string> splitCommand = utils::split(rawCommand , " ");
    std::vector<std::string>::iterator cmd_it = splitCommand.begin();
    if (splitCommand.size() >= 1) {
        targetTopic.push_back(*cmd_it);
        while (++cmd_it != splitCommand.end())
            topic += " " + *cmd_it;
        utils::trim(topic , " ");
        targetTopic.push_back(topic);
    }
    return targetTopic;
}

void Topic::execute( AClient* client, const std::string & rawCommand ) {
    std::vector<std::string> targetTopic = findTargetTopic(rawCommand);

    if (!_errorFound(client, targetTopic)) {
        Channel* chan = _ircServ._channels.find(targetTopic.front())->second;
        _whichTopicExecute(client, chan, targetTopic);
    }
 }

bool Topic::_errorFound(AClient* client, std::vector<std::string> targetTopic) {
    bool errorFound = false;

    if (targetTopic.empty()) {
        client->addMsg(ERR_NEEDMOREPARAMS( _ircServ.getIp() , "PASS"));
        errorFound = true;
    }
    else {
        std::map<const std::string, Channel*>::iterator chan = _ircServ._channels.find(targetTopic.front());
        if ( chan == _ircServ._channels.end()) {
            client->addMsg(ERR_NOSUCHCHANNEL(_ircServ.getIp(), targetTopic.front()));
            errorFound = true;
        }
        else if (!chan->second->isMember(client->getNick())) {
            client->addMsg(ERR_NOTONCHANNEL(_ircServ.getIp() , client->getNick() + " " + chan->second->getName()));
            errorFound = true;
        }
        else if (chan->second->isTopicOnly() && !chan->second->isOperator(client->getNick())) {
            client->addMsg(ERR_CHANOPRIVSNEEDED(_ircServ.getIp() , client->getNick() + " " + chan->second->getName() + " "));
            errorFound = true;
        }
    }
    return errorFound;
}

void Topic::_whichTopicExecute(AClient* client, Channel* chan , std::vector<std::string> targetTopic) {
    if (targetTopic.size() == 1) {
        if (chan->getTopic().empty())
            client->addMsg(RPL_NOTOPIC(_ircServ.getIp() , client->getNick() + " " + chan->getName()));
        else
            client->addMsg(RPL_TOPIC( client->getNick() + " " + chan->getName() , chan->getTopic()));
    } else {
        if (!targetTopic.back().compare(":")) 
            chan->setTopic(client , "");
        else {
            std::string topic = targetTopic.back();
            std::cout << "[topic] #####" << topic << std::endl;
            topic.erase(topic.begin());
            chan->setTopic(client , topic);
        }
    }

}
void Topic::clearCmd( void ){};