#include <iostream>
using namespace std;

double findAverage(int arr[], int n) {
    if (n <= 0) return 0.0;   // avoid division by zero

    long long sum = 0;        // use long long to handle large sums
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    return static_cast<double>(sum) / n;
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    if (n <= 0) {
        cout << "Array size must be positive!" << endl;
        return 0;
    }

    int arr[1000];  // fixed-size array (adjust if needed)
    cout << "Enter " << n << " elements: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    double avg = findAverage(arr, n);
    cout << "Average value of array elements = " << avg << endl;

    return 0;
}
