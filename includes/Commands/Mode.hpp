# pragma once

#include "ICommand.hpp"

class Mode: public ICommand {
public:
    void execute( void );
    void clearCmd( void );
};
