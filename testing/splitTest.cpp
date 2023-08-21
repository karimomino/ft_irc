#include <vector>
#include <string>
#include <iostream>
using namespace std;

std::vector<std::string> splitDelim( std::string str , std::string delim) {
	std::string strSection;
	std::vector<std::string> sections;

	size_t pos = 0;
	while ((pos = str.find(delim)) != std::string::npos) {
		strSection = str.substr(0, pos);
		sections.push_back( strSection );
		str.erase(0, pos + delim.length());
	}
	pos = str.find(delim);
	strSection = str.substr(0, pos);
	sections.push_back(strSection);
    return ( sections );
	
}

int main() {

    string x = "JOIN akjsndkajsndkjansd";

    cout << splitDelim(x , " ").size() << endl;

    return 0;
    
}