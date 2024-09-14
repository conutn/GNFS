#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include "C:/Users/justi/source/integer.h"

#pragma once

using namespace std;

auto start = chrono::steady_clock::now();

static void reset_timer()
{
	start = chrono::steady_clock::now();
}

static void time_passed()
{
	auto end = chrono::steady_clock::now();

	cout << chrono::duration<double, milli>(end - start).count() << '\n';
}

template <typename T>
ostream& operator << (ostream& o, const vector<T>& v)
{
	o << "[ ";

	for (T obj : v)
	{
		o << obj << " ";
	}

	o << "]";
	
	return o;
}

template <typename T, typename U>
ostream& operator << (ostream& o, const pair<T, U>& p)
{
	o << "(" << p.first << ", " << p.second << ")";
	
	return o;
}

static string add(const string& a, const string& b)
{
	bool carry = false;
	string res = a.length() > b.length() ? a : b;

	for (uint32_t i = 0; i < res.length(); ++i)
	{
		uint8_t da = i < a.length() ? (uint8_t)(a[a.length() - i - 1] - '0') : 0;
		uint8_t db = i < b.length() ? (uint8_t)(b[b.length() - i - 1] - '0') : 0;
		res[res.length() - i - 1] = (char)((da + db + carry) % 10 + '0');
		
		carry = (da + db + carry) >= 10;
	}

	if (carry)
	{
		res = "1" + res;
	}

	return res;
}

vector<string> powers{ "1" };

ostream& operator << (ostream& o, const integer& i)
{
	while (powers.size() < i.sig_bit())
	{
		powers.push_back(add(powers[powers.size() - 1], powers[powers.size() - 1]));
	}

	string res = "0";
	
	for (uint32_t n = 0; n < i.bits.size(); ++n)
	{
		bitset<64> b{ i.bits[n] };

		for (uint32_t idx = 0; idx < 64; ++idx)
		{
			if (b[idx])
			{
				res = add(res, powers[((uint64_t)(n) << 6) + idx]);
			}
		}
	}

	if (!i.sign)
	{
		res = "-" + res;
	}

	o << res;

	return o;
}