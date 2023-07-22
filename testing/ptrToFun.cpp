#include <iostream>
#include <map>

using namespace std;

typedef void (*fun)( void );
map<int, fun> testFun;
map<int, void *> testVoid;

void testFunction( void ) {
    cout << "INSIDE TEST FUNCTION" << endl;
}

void initMap( void ) {
    // init
    // fun funAddrs= &testFunction;
    fun * funAddrs = (fun *)malloc( sizeof( fun ) );
    *funAddrs = &testFunction;
    testVoid.insert( pair<int, void *>( 0, funAddrs ));
    // testFun.insert( pair<int, fun>( 0, funAddrs ));

    // testing
    // map<int, fun>::iterator itFun = testFun.find( 0 );
    map<int, void *>::iterator itVoid = testVoid.find( 0 );
    // fun ff = itFun->second;
    fun fv = (*(fun *)(itVoid->second));
    // ff();
    fv();
}

int main( void ) {
    initMap();


    cout << endl << endl;
    // testing
    // map<int, fun>::iterator itFun = testFun.find( 0 );
    map<int, void *>::iterator itVoid = testVoid.find( 0 );
    // fun ff = itFun->second;
    fun fv = (*(fun *)(itVoid->second));
    // ff();
    fv();
}
