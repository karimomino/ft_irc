#include <iostream>
#include <map>
#include <string>

using namespace std;

string someFuntion(string str) {
    return (str);
}

int main() {
    typedef string ( *derp )(string);

    string (*f)( string ) = &someFuntion;
    void *test = &f;
    string retStr;
    map<string, void *> fun_map;
    fun_map.insert(pair<string, void *>("JOIN", test));
    map<string, void *>::iterator it = fun_map.find( "JOIN");
    if (it == fun_map.end())
        cout << "something wrong nigga" << endl;
    else
        // printf("this is address of f: % p \t this is address of test: %p \t this is test address: %p\n", f, test, *(derp *)(it->second) );
        retStr = (*(derp *)(it->second))("swag");
    cout << retStr << endl;
    return 0;
}
