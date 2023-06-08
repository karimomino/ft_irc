#include <iostream>
#include <map>

using namespace std;
typedef void ( *fun )( void );
typedef void ( *funS )( std::string );
map<int, void *> funMap;

void test1( void ) {
    cout << "This is test 1 function" << endl;
}

void test2( void ) {
    cout << "This is test 2 function" << endl;
}

void test3( string ) {
    cout << "This is test 3 function" << endl;
}

int main( void ) {
    {
        // test1
        fun * tmp = new fun( &test1 );
        funMap.insert( pair<int, void *>( 0, (void *)tmp ) );

        // test2
        fun *tmp2 = new fun( &test2 );
        funMap.insert( pair<int, void *>( 1, (void *)tmp2 ) );

        // test3
        funS *tmp3 = new funS( &test3 );
        funMap.insert( pair<int, void *>( 1, (void *)tmp3 ) );
    }

    for ( int i = 0; i < 3; i++ ) {
        map<int, void *>::iterator it = funMap.find( i );
        fun tmp = (*( fun * )( it->second ));
        tmp();
        // it();
    }
}
