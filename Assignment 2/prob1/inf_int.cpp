#include "inf_int.h"
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

inf_int::inf_int() {
    this->digits = "0";
    this->length = 1;
    this->sign = true;
}

inf_int::inf_int(int n) {
    this->sign = n >= 0;
    this->digits = to_string(abs(n));
    reverse(this->digits.begin(), this->digits.end());
    this->length = this->digits.size();
}

inf_int::inf_int(const string str) {
    if (str.empty()) {
        this->sign = true;
        this->digits = "0";
        this->length = 1;
    } else {
        this->sign = (str[0] != '-');
        this->digits = (str[0] == '-') ? str.substr(1) : str;
        reverse(this->digits.begin(), this->digits.end());
        this->length = this->digits.size();
    }
}

inf_int::inf_int(const inf_int& a) {
    this->digits = a.digits;
    this->length = a.length;
    this->sign = a.sign;
}

inf_int::~inf_int() {}

inf_int& inf_int::operator=(const inf_int& a) {
    if (this == &a) return *this;
    this->digits = a.digits;
    this->length = a.length;
    this->sign = a.sign;
    return *this;
}

bool operator==(const inf_int& a, const inf_int& b) {
    return (a.digits == b.digits && a.sign == b.sign);
}

bool operator!=(const inf_int& a, const inf_int& b) {
    return !(a == b);
}

bool operator>(const inf_int& a, const inf_int& b) {
    if (a.sign != b.sign) return a.sign;

    if (a.length != b.length) {
        return a.sign ? (a.length > b.length) : (a.length < b.length);
    }
    return a.sign ? (a.digits > b.digits) : (a.digits < b.digits);
}

bool operator<(const inf_int& a, const inf_int& b) {
    return !(a > b) && (a != b);
}

inf_int operator+(const inf_int& a, const inf_int& b) {
    inf_int result;
    result.digits = "";

    int carry = 0;
    int len = max(a.length, b.length);
    string digits_a = a.digits + string(len - a.length, '0');
    string digits_b = b.digits + string(len - b.length, '0');

    if (a.sign == b.sign) {
        for (int i = 0; i < len; ++i) {
            int sum = (digits_a[i] - '0') + (digits_b[i] - '0') + carry;
            carry = sum / 10;
            result.digits.push_back((sum % 10) + '0');
        }
        if (carry) result.digits.push_back(carry + '0');
        result.sign = a.sign;
    } else {
        inf_int temp_b = b;
        temp_b.sign = !b.sign;
        result = a - temp_b;
    }
    result.length = result.digits.size();
    return result;
}

inf_int operator-(const inf_int& a, const inf_int& b) {
    inf_int result;
    result.digits = "";

    int borrow = 0;
    int len = max(a.length, b.length);
    string digits_a = a.digits + string(len - a.length, '0');
    string digits_b = b.digits + string(len - b.length, '0');

    if (a.sign == b.sign) {
        for (int i = 0; i < len; ++i) {
            int diff = (digits_a[i] - '0') - (digits_b[i] - '0') - borrow;
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result.digits.push_back(diff + '0');
        }
        result.sign = (a > b) == a.sign;
    } else {
        inf_int temp_b = b;
        temp_b.sign = !b.sign;
        result = a + temp_b;
    }
    result.length = result.digits.size();
    return result;
}

inf_int operator*(const inf_int& a, const inf_int& b) {
    inf_int result;
    result.digits = string(a.length + b.length, '0');  // Reserve space for the maximum possible length
    result.sign = (a.sign == b.sign);  // Result is positive if signs are the same

    for (int i = 0; i < a.length; ++i) {
        int carry = 0;
        int digit_a = a.digits[i] - '0';

        for (int j = 0; j < b.length; ++j) {
            int digit_b = b.digits[j] - '0';
            int sum = (result.digits[i + j] - '0') + digit_a * digit_b + carry;

            result.digits[i + j] = (sum % 10) + '0';  // Update the current position with the last digit of sum
            carry = sum / 10;  // Compute the carry for the next position
        }

        if (carry > 0) {
            result.digits[i + b.length] += carry;  // Add remaining carry to the next position
        }
    }

    // Remove leading zeros
    while (result.digits.size() > 1 && result.digits.back() == '0') {
        result.digits.pop_back();
    }

    result.length = result.digits.size();
    return result;
}

inf_int operator/(const inf_int& a, const inf_int& b) {
    if (b.digits == "0") throw runtime_error("Division by zero");

    inf_int dividend = a;
    inf_int divisor = b;
    dividend.sign = divisor.sign = true; // Work with absolute values for division

    inf_int quotient("0");
    inf_int temp("0");

    // Perform long division manually
    for (int i = dividend.length - 1; i >= 0; i--) {
        // Shift left: Insert the current dividend digit into temp
        temp.digits.insert(temp.digits.begin(), dividend.digits[i]);
        temp.length = temp.digits.size();

        // Normalize temp by removing leading zeros
        while (temp.digits.size() > 1 && temp.digits.back() == '0') {
            temp.digits.pop_back();
            temp.length = temp.digits.size();
        }

        // Determine how many times divisor fits into temp
        int count = 0;
        while (temp > divisor || temp == divisor) {
            temp = temp - divisor;
            count++;
        }

        // Append the quotient digit
        quotient.digits.insert(quotient.digits.begin(), count + '0');
    }

    // Remove leading zeros from the quotient
    while (quotient.digits.size() > 1 && quotient.digits.back() == '0') {
        quotient.digits.pop_back();
    }

    // Set the correct sign for the quotient
    quotient.sign = (a.sign == b.sign);

    // Update the length of the quotient
    quotient.length = quotient.digits.size();
    return quotient;
}

ostream& operator<<(ostream& out, const inf_int& a) {
    if (!a.sign) out << '-';
    string reversed_digits = a.digits;
    reverse(reversed_digits.begin(), reversed_digits.end());
    out << reversed_digits;
    return out;
}

istream& operator>>(istream& in, inf_int& a) {
    string input;
    in >> input;

    a.sign = (input[0] != '-');
    a.digits = (input[0] == '-') ? input.substr(1) : input;
    reverse(a.digits.begin(), a.digits.end());
    a.length = a.digits.size();

    return in;
}
