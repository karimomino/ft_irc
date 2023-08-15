#include <iostream>
using namespace std;
class stringTeest
{
private:
    /* data */
public:
    stringTeest(/* args */);
    ~stringTeest();
    string x;
};

stringTeest::stringTeest(/* args */)
{
}

stringTeest::~stringTeest()
{
}

int main(){ 
    stringTeest y;

    cout << y.x.compare("") << endl;
    return 0;
}