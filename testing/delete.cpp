#include <string>
#include <map>
#include <iostream>
using namespace std;
int main() {
    map<int , string*> test;
    test.insert(make_pair(1 , new string("hello")));
    test.insert(make_pair(2 , new string("hello")));
    test.insert(make_pair(3 , new string("hello")));
    test.insert(make_pair(4 , new string("hello")));

    map<int , string*>::iterator it = test.begin();
    while ((it = test.begin())!= test.end() ) {
        cout << "Deleting: " << it->first << endl;
        delete it->second;
        test.erase(it->first);
    }
    return 0;
}