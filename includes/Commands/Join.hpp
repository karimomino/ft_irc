# pragma once

#include "ICommand.hpp"

class Join : public ICommand {
public:
    void execute( void );
    void clearCmd( void );
};
