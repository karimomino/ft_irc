#include <iostream>

class Test {
    public:
        template< typename T >
        print( T t );
};

template< typename T >
Test::print( T t ) {
    std::cout << t << std::endl;
}


template< typename T >
const T & max( const T & first, const T & second ) { return first > second ? first : second; }

int main() {
    Test a;

    a.print( 69 );
    a.print( "test" );
}
