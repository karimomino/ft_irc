#include <iostream>
#include <map>

using namespace std;
typedef void ( *fun )( void );
typedef void ( *funS )( string );
typedef void ( *funx )( string, int, float, void * );
map<int, void *> funMap;

void test1( void ) {
    cout << "This is test 1 function" << endl;
}

void test2( void ) {
    cout << "This is test 2 function" << endl;
}

void test3( string s ) {
    cout << "This is test 3 function" << endl;
    cout << s << endl;
}

void findFun( string s, map<int, void *>::iterator it ) {
    // try {
    //     fun tmp = (*( fun * )( it->second ));
    //     tmp();
    //     return ;
    // } catch ( const std::exception & e ) {
    //     cout << "Not fun function" << endl;
    // }

    try {
        funx tmp = (*( funx * )( it->second ));
        tmp( s, 0, 0, NULL );
    } catch ( const std::exception & e ) {
        cout << "Not fun function" << endl;
    }
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
        funMap.insert( pair<int, void *>( 2, (void *)tmp3 ) );
    }

    {
        for ( int i = 0; i < 3; i++ ) {
            map<int, void *>::iterator it = funMap.find( i );
            findFun( "asdad", it );
        }
    }

    {
        map<int, void *>::iterator it = funMap.begin();
        for ( ; it != funMap.end(); it++ ) {
            delete ( ( fun * )it->second );
        }
    }
}
