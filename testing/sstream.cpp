#include <iostream>
#include <sstream>

using namespace std;

static const vector<string> getArgs( const string& rawCmd ) {
    string tmp;
    vector<string> list;
    stringstream ss( rawCmd );

    for ( int i = 0; i < 3; i++ ) {
        getline(ss, tmp, ' ');
        list.push_back( tmp );
        tmp.clear();
    }
    return ( list );
}

int main( void ) {
    vector<string> v1 = getArgs( "#Tofa7a Baty5a" );
    vector<string> v2 = getArgs( "#Tofa7a Baty5a Far5a" );
    vector<string> v3 = getArgs( "#Tofa7a     Baty5a Far5a" );

    vector<string>::iterator it;

    for ( it = v1.begin(); it != v1.end(); it++ )
        cout << "[" << *it << "]" << endl;

    cout << endl << endl;

    for ( it = v2.begin(); it != v2.end(); it++ )
        cout << "[" << *it << "]" << endl;

    cout << endl << endl;

    for ( it = v3.begin(); it != v3.end(); it++ )
        cout << "[" << *it << "]" << endl;
}
