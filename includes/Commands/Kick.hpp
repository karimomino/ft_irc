# pragma once

#include "ICommand.hpp"

class Kick : public ICommand {
private:

public:
    void execute( void );
    void clearCmd( void );
};
