# pragma once

#include "ICommand.hpp"

class Topic: public ICommand {
public:
    void execute( void );
    void clearCmd( void );
};
