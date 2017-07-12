#include <iostream>
#include <map>
#include "AssocVector/AssocVector.h"

using namespace std;

int main()
{
    //typedef map<std::string, int> MP;
    typedef AssocVector<std::string, int> MP;
    MP mp_int{{"Bob", 1}, {"Oh", 5}};
    mp_int.insert(make_pair("Alice", 2));
    for (MP::iterator i = mp_int.begin(); i != mp_int.end(); i++)
        cout << i->first << ' ' << i->second << endl;
    for (auto i : mp_int)
        cout << i.first << ' ' << i.second << endl;
    if (mp_int.count("Alice")) cout << "Good" << endl;
    cout << mp_int["Oh"] << endl;
    mp_int["Jack"] = 10;
    cout << mp_int.size() << endl;
    cout << mp_int.find("Bob")->second << endl;
    return 0;
}