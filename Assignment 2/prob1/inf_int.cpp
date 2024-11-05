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
    else {
        // add zero padding for comparison
        int len = max(a.length, b.length);
        string temp_a = string("0", len - a.length) + string(a.digits.end(), a.digits.begin());
        string temp_b = string("0", len - b.length) + string(b.digits.end(), b.digits.begin());
        
        return a.sign ? (temp_a > temp_b) : (temp_a < temp_b);
    }
}

bool operator<(const inf_int& a, const inf_int& b) {
    return !(a > b) && (a != b);
}

inf_int operator+(const inf_int& a, const inf_int& b) {
    inf_int result;
    result.digits = "";

    int carry = 0;
    int len = max(a.length, b.length);

    // add zero-paddings to make both operands have equal lengths
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

    // add zero-paddings to make both oeprands have equal lengths
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

    // reserve space for maximum possible length
    result.digits = string(a.length + b.length, '0');
    result.sign = (a.sign == b.sign);

    for (int i = 0; i < a.length; ++i) {
        int carry = 0;
        int digit_a = a.digits[i] - '0';

        for (int j = 0; j < b.length; ++j) {
            int digit_b = b.digits[j] - '0';
            int sum = (result.digits[i + j] - '0') + digit_a * digit_b + carry;

            // update the current position with the last digit of sum
            result.digits[i + j] = (sum % 10) + '0';  
            carry = sum / 10;
        }

        // add remaining carry to the next position
        if (carry)
            result.digits[i + b.length] += carry;
    }

    // remove leading zeros
    while (result.digits.size() && result.digits.back() == '0')
        result.digits.pop_back();

    result.length = result.digits.size();
    return result;
}

inf_int operator/(const inf_int& a, const inf_int& b) {
    if (b.digits == "0") throw runtime_error("Division by zero");

    inf_int dividend = a, divisor = b, temp("0"), answer("0");
    string quotient = "", frac = "";

    // using absolute value for convenience
    dividend.sign = divisor.sign = true;

    // integer part (ex. 12345 / 678)
    for (int i = dividend.length - 1; i >= 0; i--) {
        temp.digits = dividend.digits[i] + temp.digits;
        temp.length = temp.digits.size();
        
        // check how many times divisor can fit into temp
        int cnt = 0;
        while (temp > divisor || temp == divisor) {
            temp = temp - divisor;
            cnt++;
        }  
        
        quotient = to_string(cnt) + quotient;
    }

    // fraction part (precision up to 50 digits)
    int precision = 50;
    while (temp.digits != "0" && precision) {
        temp.digits.insert(0, "0");
        temp.length = temp.digits.size();

        // check how many times divisor can fit into temp
        int cnt = 0;
        while(temp > divisor || temp == divisor) {
            temp = temp - divisor;
            cnt++;
        }
        
        frac = to_string(cnt) + frac;
        precision--;
    }

    answer.sign = (a.sign == b.sign);
    answer.digits = frac + "." + quotient;
    answer.length = answer.digits.size();
    
    return answer;
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