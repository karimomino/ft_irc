#include <iostream>
#include <map>
#include <utility>

using namespace std;

typedef void (*f)( void );
typedef map< int, f > mapF;

mapF test;

void test1( void ) {
    cout << "huh 1" << endl;
}

void test2( void ) {
    cout << "huh 2" << endl;
}

void test3( void ) {
    cout << "huh 3" << endl;
}

void initFun( void ) {
    // f * tmp = new f( test1 );
    test.insert( pair< int, f >( 0, test1 ) );
    test.insert( pair< int, f >( 1, test2 ) );
    test.insert( pair< int, f >( 2, test3 ) );
}

void execFun( int i ) {
    mapF::iterator it = test.find( i );
    if ( it == test.end() )
        return ;
    f fun = *( f )( it->second );
    fun();
}

int main( void ) {
    initFun();

    for ( int i = 0; i < 3; i++ ) {
        execFun( i );
    }
}
