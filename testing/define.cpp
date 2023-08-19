#include <iostream>

using namespace std;

std::string ip = "1231231";
# define ERR_NOTONCHANNEL(arg) ( ":" + ip + " 442 " + (std::string)arg + " :User not on channel\r\n" )

int main() {
    cout << ERR_NOTONCHANNEL( "Dad" ) << endl;
}
