#include "Commands/ICommand.hpp"

ICommand::ICommand( Server& ircServ  ) : _ircServ(ircServ) {}

ICommand::~ICommand( void ) {}
