#include <iostream>

using namespace std;

class Test {
public:
    Test() {
        std::cout << "constructor" << std::endl;
    }

    ~Test() {
        std::cout << "detructor" << std::endl;
    }

    void des() {
        std::cout << "obj is gonna get deleted" << std::endl;
        delete this;
        std::cout << "obj deleted" << std::endl;
    }
};

int main() {
    Test* t = new Test();
    Test tt;

    t->des();
    // tt.des();

}
