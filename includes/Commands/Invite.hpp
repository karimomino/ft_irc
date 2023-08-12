# pragma once

#include "ICommand.hpp"

class Invite : public ICommand {
public:
    void execute( void );
    void clearCmd( void );
};
