#include <iostream>
#include <string>
using namespace std;

int main()
{
    string input;
    cout << "Enter an expression: ";
    cin >> input;

    for (int i = 0; i < input.length(); i++)
    {
        if (input[i] == '+' || input[i] == '-' || input[i] == '*' ||
            input[i] == '/' || input[i] == '%' || input[i] == '=')
        {
            cout << "Operator: " << input[i] << endl;
        }
        else
        {
            cout << "Not an operator: " << input[i] << endl;
        }
    }

    return 0;
}
