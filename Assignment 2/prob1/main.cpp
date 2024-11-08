#include <iostream>
#include "inf_int.h"

using namespace std;

int main() {
    // Testing Constructors
    cout << "Testing Constructors...\n";
    inf_int a;                    // Default constructor
    inf_int b(123456789);         // Integer constructor
    inf_int c("-987654321");      // String constructor (negative)
    inf_int d("123456789");       // String constructor (positive)
    inf_int e(b);                 // Copy constructor
    cout << "Default: " << a << " (Expected: 0)\n";
    cout << "From int: " << b << " (Expected: 123456789)\n";
    cout << "From negative string: " << c << " (Expected: -987654321)\n";
    cout << "From positive string: " << d << " (Expected: 123456789)\n";
    cout << "Copy: " << e << " (Expected: 123456789)\n";

    // Testing Comparison Operators
    cout << "\nTesting Comparison Operators...\n";
    inf_int f("12345"), g("54321"), h("12345"), i("-12345");

    cout << "f == h: " << (f == h) << " (Expected: 1)\n";
    cout << "f != g: " << (f != g) << " (Expected: 1)\n";
    cout << "f < g: " << (f < g) << " (Expected: 1)\n";
    cout << "g > f: " << (g > f) << " (Expected: 1)\n";
    cout << "i < f: " << (i < f) << " (Expected: 1)\n";
    cout << "i == i: " << (i == i) << " (Expected: 1)\n";

    // Testing Arithmetic Operators
    cout << "\nTesting Arithmetic Operators...\n";
    inf_int j("12345678901234567890"), k("98765432109876543210");

    cout << "j + k: " << (j + k) << " (Expected: 111111111011111111100)\n";
    cout << "k - j: " << (k - j) << " (Expected: 86419753208641975320)\n";
    cout << "j * k: " << (j * k) << " (Expected: 1219326311370217952237463801111263526900)\n";

    inf_int l("50000000000000000001"), m("10");
    cout << "l / m: " << (l / m) << " (Expected: 5000000000000000000.1)\n";
    
    // Edge cases for addition
    inf_int negJ("-12345678901234567890");
    cout << "j + negJ: " << (j + negJ) << " (Expected: 0)\n";   // Zero check

    // Edge cases for subtraction
    cout << "j - j: " << (j - j) << " (Expected: 0)\n";         // Zero check
    cout << "j - k: " << (j - k) << " (Expected: -86419753208641975320)\n"; // Negative result check

    // Edge case for division by a random long number, checking 50-digit precision
    inf_int n("1234567890123456789012345678901234567890"), o("987654321");
    cout << "n / o (precision test): " << (n / o) << " (Expected: around 1250000000125.0000000000...)\n";

    // Testing I/O Operators
    cout << "\nTesting I/O Operators...\n";
    inf_int p;
    cout << "Enter a large integer: ";
    cin >> p;
    cout << "You entered: " << p << "\n";

    return 0;
}
