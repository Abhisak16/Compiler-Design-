#include <iostream>
#include <string>
#include <cctype>   // for isalpha, isalnum

using namespace std;

bool isIdentifier(const string& str) {
    if (str.empty())
        return false;

    // Rule a: first character must be alphabet or underscore
    if (!(isalpha(str[0]) || str[0] == '_'))
        return false;

    // Rule b: remaining characters must be alphanumeric or underscore
    for (size_t i = 1; i < str.length(); i++) {
        if (!(isalnum(str[i]) || str[i] == '_'))
            return false;
    }

    return true;
}

int main() {
    string input;
    cout << "Enter input: ";
    cin >> input;

    if (isIdentifier(input))
        cout << "Valid Identifier" << endl;
    else
        cout << "Invalid Identifier" << endl;

    return 0;
}
