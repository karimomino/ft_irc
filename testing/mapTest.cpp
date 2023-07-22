#include <iostream>
#include <map>
#include <utility>

using namespace std;

class Client {
public:
    string nick;
    Client( string nick ) {
        this->nick = nick;
        cout << nick << " constructor is called" << endl;
    }
};

Client client( "Baty5a" );
map<int, Client> test1;
map<int, Client *> test2;

void addTest1( Client const & client ) {
    test1.insert( pair<int, Client>( 0, client ));
}

void addTest2( Client & client ) {
    // test2.insert( pair<int, Client>( 0, client ));
    test2.insert( pair<int, Client *>( 0, &client ));
}

int main( void ) {
    addTest1( client );
    addTest2( client );

    cout << "Address of original client\t" << &client << endl;
    cout << "Address of test1 client\t\t" << &test1.find( 0 )->second << endl;
    cout << "Address of test2 client\t\t" << test2.find( 0 )->second << endl;
}
