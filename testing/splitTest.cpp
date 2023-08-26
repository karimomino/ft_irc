#include <vector>
#include <string>
#include <iostream>
using namespace std;

std::vector<std::string> splitDelim( std::string str , std::string delm) {
size_t pos = 0;
    std::string token;
    std::vector<std::string> list;
    while ( ( pos = str.find( delm ) ) != std::string::npos ) {
        token = str.substr(0, pos);
        if ( !token.empty() )
            list.push_back( token );
        str.erase(0, pos + delm.length());
    }
    return ( list );	
}

int main() {

    string x = "#saha";

    cout << splitDelim(x , " ").size() << endl;

    return 0;
    
}
