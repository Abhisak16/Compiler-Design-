#include <iostream>
#include <string>
using namespace std;

int main()
{
    string input;
    cout << "Enter a line: ";
    getline(cin, input);

    if (input.rfind("//", 0) == 0)
    {
        cout << "It is a single-line comment." << endl;
    }
    else if (input.rfind("/*", 0) == 0 &&
             input.size() >= 4 &&
             input.substr(input.size() - 2) == "*/")
    {
        cout << "It is a multi-line comment." << endl;
    }
    else
    {
        cout << "It is NOT a comment." << endl;
    }

    return 0;
}
