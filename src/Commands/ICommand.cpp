#include "Commands/ICommand.hpp"

ICommand::ICommand( Server& ircServ , AClient& client, const std::string& rawCommand ) : _ircServ(ircServ) , _client(client) , _rawCommand(rawCommand) {
}