#include "inf_int.h"

inf_int::inf_int()
{
	this->digits = "0";
	this->length = 1;
	this->sign = true;
}

inf_int::inf_int(int n)
{
	this->sign = n < 0 ? false : true;
	this->digits = to_string(n);
	reverse(this->digits.begin(), this->digits.end());
	this->length = this->digits.size();
}

inf_int::inf_int(const string str)
{
	// set to zero if parameter is an empty string
	if(str.empty()) {
		this->sign = true;
		this->digits = "0";
		this->length = 1;
	}
	else {
		this->sign = (str[0] == '-') ? false : true;
		this->digits = (str[0] == '-') ? substr(1) : str;
		this->length = this->digits.size();
	}
}

inf_int::inf_int(const inf_int& a)
{
	this->digits = a.digits;
	this->length=a.length;
	this->sign=a.sign;
}

inf_int::~inf_int(){}

inf_int& inf_int::operator=(const inf_int& a)
{
	if(this->digits)
		delete this->digits;

	this->digits = a.digits;
	this->length = a.length;
	this->sign = a.sign;

	return *this; 
}

bool operator==(const inf_int& a, const inf_int& b)
{
	return (a.digits == b.digits && a.sign == b.sign);
}

bool operator!=(const inf_int& a, const inf_int& b)
{
	return !operator==(a, b);
}

bool operator>(const inf_int& a, const inf_int& b)
{
	int len = max(a.length, b.length);

	// add zero-padding for string comparison
	string digits_a = string(len - a.length, '0') + a.digits;
	string digits_b = string(len - b.length, '0') + b.digits;

	if(a.sign == b.sign) {
		if(a.sign) return digits_a > digits_b;
		else return !(digits_a > digits_b);
	}
	return a.sign;
}

bool operator<(const inf_int& a, const inf_int& b)
{
	return !(operator>(a, b));
}

inf_int operator+(const inf_int& a, const inf_int& b)
{	
	inf_int result;
	result.digits = "";
	
	int carry = 0;
	int len = max(a.length, b.length);

	string digits_a = string(len - a.length, '0') + a.digits;
	string digits_b = string(len - b.length, '0') + b.digits;

	if(a.sign == b.sign) {
		for(int i = 0; i < len; i++) {
			int add = (digits_a[i] - '0') + (digits_b[i] - '0') + carry;
			carry = add / 10;
			result.digits.push_back(add % 10 + '0');
		}
		result.sign = a.sign;
	}
	else if(!a.sign) {
		a.sign = true;
		return operator-(b, a);
	}
	else {
		b.sign = true;
		return operator-(a, b);
	}
	
	// add leftover carry
	if(carry) 
		result.digits.push_back(carry);
	
	result.size = result.digits.size();
	
	return result;
}

inf_int operator-(const inf_int& a, const inf_int& b)
{
	inf_int result;
	result.digits = "";

	int borrow = 0;
	int len = max(a.length, b.length);
	
	string digits_a = string(len - a.length, '0') + a.digits;
	string digits_b = string(len - b.length, '0') + b.digits;

	if(a.sign == b.sign) {
		for(int i = 0; i < len; i++) {
			int diff = (digits_a[i] - '0') - (digits_b[i] - '0') - borrow;
			if(diff < 0) {
				diff += 10;
				borrow = 1;
			}
			result.digits.push_back(diff + '0');
		}
		result.sign = operator>(a, b) ? a.sign : !a.sign;
	}
	else {
		// -1 - 5, 1 - (-5), -6 - 4, -6 - (-4)
	}
}

inf_int operator*(const inf_int& a, const inf_int& b)
{
    
}

ostream& operator<<(ostream& out, const inf_int& a)
{
	if(!a.sign) out << '-';
	out << reverse(a.digits.begin(), a.digits.end());
	return out;
}
