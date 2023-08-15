#include <iostream>
#include <map>
using namespace std;
int main(){
    map<int , string> x;
    x.insert(make_pair( 3 , "testerrrrr"));
    x[5] = "swagge";

    cout << x[3] << endl;
    cout << x[5] << endl;
    cout << x[51] << endl;

    return 0;

}