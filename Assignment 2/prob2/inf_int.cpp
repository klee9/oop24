#include "inf_int.h"
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

inf_int::inf_int() : digits("0"), length(1), sign(true) {}

inf_int::inf_int(int n) {
    sign = (n >= 0);
    digits = to_string(abs(n));
    reverse(digits.begin(), digits.end());
    length = digits.size();
}

inf_int::inf_int(const string str) {
    if (str.empty()) {
        sign = true;
        digits = "0";
        length = 1;
    } else {
        sign = (str[0] != '-');
        digits = (str[0] == '-') ? str.substr(1) : str;
        reverse(digits.begin(), digits.end());
        length = digits.size();
    }
}

inf_int::inf_int(const inf_int& a) : digits(a.digits), length(a.length), sign(a.sign) {}

inf_int::~inf_int() {}

inf_int& inf_int::operator=(const inf_int& a) {
    if (this != &a) {
        digits = a.digits;
        length = a.length;
        sign = a.sign;
    }
    return *this;
}

bool operator==(const inf_int& a, const inf_int& b) {
    return a.sign == b.sign && a.digits == b.digits;
}

bool operator!=(const inf_int& a, const inf_int& b) {
    return !(a == b);
}

bool operator>(const inf_int& a, const inf_int& b) {
    if (a.sign != b.sign) {
        return a.sign;
    }

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
    int carry = 0, len = max(a.length, b.length);
    
    string digits_a = a.digits + string(len - a.length, '0');
    string digits_b = b.digits + string(len - b.length, '0');

    if (a.sign == b.sign) {
        for (int i = 0; i < len; i++) {
            int sum = (digits_a[i] - '0') + (digits_b[i] - '0') + carry;
            carry = sum / 10;
            result.digits.push_back((sum % 10) + '0');
        }

        if (carry) {
            result.digits.push_back(carry + '0');
        }
        
        result.sign = a.sign;
    } else {
        // convert to subtraction if signs differ
        inf_int temp_b = b;
        temp_b.sign = !b.sign;
        result = a - temp_b;
    }

    // remove leading zeros
    while (result.digits.size() > 1 && result.digits.back() == '0') {
        result.digits.pop_back();
    }
        
    result.length = result.digits.size();
    return result;
}

inf_int operator-(const inf_int& a, const inf_int& b) {
    if (a == b) {
        return inf_int("0");
    }
    
    inf_int result;
    int borrow = 0, len = max(a.length, b.length);
    
    string digits_a = a.digits + string(len - a.length, '0');
    string digits_b = b.digits + string(len - b.length, '0');

    if (a.sign == b.sign) {
        // determine if |a| >= |b|, otherwise swap digits for correct subtraction order
        bool is_a_greater = (a > b) == a.sign;
        
        if (!is_a_greater) {
            swap(digits_a, digits_b);
            result.sign = !a.sign;
        } else {
            result.sign = a.sign;
        }

        // perform subtraction
        for (int i = 0; i < len; i++) {
            int diff = (digits_a[i] - '0') - (digits_b[i] - '0') - borrow;
            
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }

            result.digits.push_back(diff + '0');
        }
    } else {
        // if signs differ, convert subtraction to addition
        inf_int temp_b = b;
        temp_b.sign = !b.sign;
        return a + temp_b;
    }

    // remove leading zeros
    while (result.digits.size() > 1 && result.digits.back() == '0') {
        result.digits.pop_back();
    }

    result.length = result.digits.size();
    return result;
}

inf_int operator*(const inf_int& a, const inf_int& b) {
    inf_int result;
    result.digits = string(a.length + b.length, '0');
    result.sign = (a.sign == b.sign);

    for (int i = 0; i < a.length; ++i) {
        int carry = 0;
        int digit_a = a.digits[i] - '0';

        for (int j = 0; j < b.length; ++j) {
            int digit_b = b.digits[j] - '0';
            int sum = (result.digits[i + j] - '0') + digit_a * digit_b + carry;
            result.digits[i + j] = (sum % 10) + '0';
            carry = sum / 10;
        }

        if (carry)
            result.digits[i + b.length] += carry;
    }

    // remove leading zeros
    while (result.digits.size() > 1 && result.digits.back() == '0') {
        result.digits.pop_back();
    }

    result.length = result.digits.size();
    return result;
}

inf_int operator/(const inf_int& a, const inf_int& b) {
    if (b.digits == "0")
        throw runtime_error("division by zero");

    inf_int dividend = a, divisor = b, temp("0"), answer("0");
    string quotient = "", frac = ".";
    temp.digits = "";
    dividend.sign = divisor.sign = true;

    for (int i = dividend.length - 1; i >= 0; i--) {
        temp.digits = dividend.digits[i] + temp.digits;
        temp.length = temp.digits.size();
        int cnt = 0;

        while (temp > divisor || temp == divisor) {
            temp = temp - divisor;
            cnt++;
        }

        if(temp.digits == "0") {
            temp.digits = "";
        }
        
        quotient = to_string(cnt) + quotient;
    }

    int precision = 50;
    while (temp.digits != "0" && precision) {
        temp.digits.insert(temp.digits.begin(), '0');
        temp.length = temp.digits.size();
        int cnt = 0;

        while(temp > divisor || temp == divisor) {
            temp = temp - divisor;
            cnt++;
        }
        
        frac = to_string(cnt) + frac;
        precision--;
    }

    answer.sign = (a.sign == b.sign);
    answer.digits = frac + quotient;

    // remove leading zeros
    while (answer.digits.size() > 1 && answer.digits.back() == '0') {
        answer.digits.pop_back();
    }

    answer.length = answer.digits.size();
    return answer;
}

ostream& operator<<(ostream& out, const inf_int& a) {
    if (!a.sign) {
        out << '-';
    }
    
    string reversed_digits = a.digits;
    reverse(reversed_digits.begin(), reversed_digits.end());
    out << reversed_digits;
    return out;
}

istream& operator>>(istream& in, inf_int& a) {
    string input;
    in >> input;

    if (input.empty() || input == "0") {
        // set default zero values
        a.sign = true;
        a.digits = "0";
        a.length = 1;
    }

    else {
        // initialize from input string
        a.sign = (input[0] != '-');
        a.digits = (input[0] == '-') ? input.substr(1) : input;
        reverse(a.digits.begin(), a.digits.end());
        a.length = a.digits.size();
    }

    return in;
}
