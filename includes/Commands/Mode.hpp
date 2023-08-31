# pragma once

#include "ICommand.hpp"
#include <utility>
#include <vector>
#include <map>


class Mode;
class Server;
class Channel;
class Client;

typedef bool ( Mode::*modePtr )( bool, std::string& );
typedef std::pair<std::string, std::string> pSS;
typedef std::vector< std::pair<char, std::string> > vec_pCS;

class Mode: public ICommand {
private:
    Channel* _chan;
    AClient* _client;
    std::map<char, modePtr> _mPtr;

    bool inviteMode( bool, std::string& arg );
    bool topicMode( bool, std::string& arg );
    bool keyMode( bool, std::string& arg );
    bool opMode( bool, std::string& arg );
    std::vector<std::string> validateArgs( AClient*, const std::string& );
    pSS executeMode( vec_pCS modes );

public:
    Mode();
    Mode( Server& ircServ );
    ~Mode();
    void execute( AClient*, const std::string & rawCommand );
    void clearCmd( void );
};
