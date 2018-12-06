#include <iostream>
#include <string>
#include <fstream>
#include <map>
using namespace std;

class ConfigReader {
    double MAXACCEL;
    double MAXDECEL;
    double TARGETSPEED;
  public:
};

int main () {
    std::ifstream file("Constants.txt");
    std::string temp;
    map<string, string> m;
    while(std::getline(file, temp)) {
        int pos = temp.find_first_of('=');
        string value = temp.substr(pos+1);
        string key = temp.substr(0, pos);
        m.insert(std::pair<string, string>(key,value));
    }

    for(auto elem : m)
    {
        std::cout << elem.first << " " << elem.second << "\n";
    }
}