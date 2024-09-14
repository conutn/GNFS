#include <bitset>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

#pragma once

using namespace std;

class integer
{
public:
	bool sign = true;
	vector<uint64_t> bits = vector<uint64_t>(1);

	integer()
	{
		bits[0] = 0;
	}
	integer(int32_t n)
	{
		if (n < 0)
		{
			sign = false;
			bits[0] = -n;
		}
		else
		{
			bits[0] = n;
		}
	}
	integer(int64_t n)
	{
		if (n < 0)
		{
			sign = false;
			bits[0] = -n;
		}
		else
		{
			bits[0] = n;
		}
	}
	integer(uint32_t n)
	{
		bits[0] = n;
	}
	integer(uint64_t n)
	{
		bits[0] = n;
	}
	integer(const integer& n)
	{
		sign = n.sign;
		bits = n.bits;
	}

	~integer() {}

	void reduce()
	{
		for (size_t i = bits.size() - 1; i > 0; --i)
		{
			if (bits[i] > 0)
				return;
			bits.erase(bits.begin() + i);
		}
	}

	bool operator== (const integer& o) const
	{
		if (bits.size() == o.bits.size() && bits[0] == 0 && o.bits[0] == 0) return true;

		if (sign != o.sign) return false;

		if (bits.size() != o.bits.size()) return false;

		for (size_t i = 0; i < bits.size(); ++i)
		{
			if (bits[i] != o.bits[i]) return false;
		}

		return true;
	}

	bool operator!= (const integer& o) const
	{
		return !(*this == o);
	}

	bool unsigned_less(const integer& o) const
	{
		if (bits.size() != o.bits.size()) return bits.size() < o.bits.size();

		for (size_t i = 0; i < bits.size(); ++i)
		{
			size_t idx = bits.size() - i - 1;
			if (bits[idx] != o.bits[idx]) return bits[idx] < o.bits[idx];
		}

		return false;
	}

	bool operator< (const integer& o) const
	{
		if (sign != o.sign) return !sign;

		return !(sign ^ unsigned_less(o));
	}

	bool operator<= (const integer& o) const
	{
		return (*this == o) || (*this < o);
	}

	bool operator> (const integer& o) const
	{
		return !(*this < o) && !(*this == o);
	}

	bool operator>= (const integer& o) const
	{
		return !(*this < o);
	}

	bool get_bit(const uint64_t& n, const uint32_t& pos) const
	{
		return ((size_t)1 << pos & n) > 0;
	}

	integer abs() const
	{
		integer res = *this;
		res.sign = true;
		return res;
	}

	void add_helper(const integer& o, integer& new_int, bool& carry, bool& sum) const
	{
		for (size_t i = 0; i < new_int.bits.size(); ++i)
		{
			new_int.bits[i] += carry;
			if (bits.size() > i) new_int.bits[i] += lim & bits[i];
			if (o.bits.size() > i) new_int.bits[i] += lim & o.bits[i];

			bool a = bits.size() > i && (bits[i] >> 63),
				b = o.bits.size() > i && (o.bits[i] >> 63),
				c = new_int.bits[i] >> 63;

			sum = a ^ b ^ c;
			carry = ((a ^ b) & c) ^ (a & b);

			new_int.bits[i] = (lim & new_int.bits[i]) + ((uint64_t)sum << 63);
		}
	}

	void sub_helper(const integer& a, const integer& b, integer& new_int, bool& carry, bool& sum) const
	{
		for (size_t i = 0; i < new_int.bits.size(); ++i)
		{
			uint64_t x = a.bits.size() > i ? a.bits[i] : 0,
				y = b.bits.size() > i ? b.bits[i] : 0;

			carry = carry && (x-- == 0);
			if (x < y)
			{
				carry = true;
				new_int.bits[i] = (uint64_t)(-1) - y + x + 1;
			}
			else new_int.bits[i] = x - y;
		}
	}

	integer operator+ (const integer& o) const
	{
		integer new_int = integer();
		new_int.bits.resize(max(bits.size(), o.bits.size()));
		bool carry = false, sum = false;

		if (sign == o.sign)
		{
			add_helper(o, new_int, carry, sum);
			new_int.sign = sign;
		}
		else if (this->abs() < o.abs())
		{
			sub_helper(o, *this, new_int, carry, sum);
			new_int.sign = o.sign;
		}
		else
		{
			sub_helper(*this, o, new_int, carry, sum);
			new_int.sign = this->sign;
		}

		if (carry) new_int.bits.push_back(1);
		new_int.reduce();

		return new_int;
	}

	integer operator+= (const integer& o)
	{
		*this = *this + o;
		return *this;
	}

	integer operator- (const integer& o) const
	{
		integer tmp = o;
		tmp.sign ^= true;

		tmp = *this + tmp;
		tmp.reduce();

		return tmp;
	}

	integer operator-= (const integer& o)
	{
		*this = *this - o;
		return *this;
	}

	int64_t to_int() const
	{
		if (bits.size() > 1) return (uint64_t)(-1);

		if (sign) return bits[0];
		return -(int64_t)bits[0];
	}

	integer operator<< (const uint64_t& shift) const
	{
		integer result = integer();
		result.bits.resize((shift >> 6) + bits.size() + 1);

		for (size_t i = 0; i < bits.size(); ++i)
		{
			result.bits[i + (shift >> 6)] += bits[i] << shift % 64;
			if (shift % 64 != 0)
				result.bits[i + (shift >> 6) + 1] += bits[i] >> (64 - shift % 64);
		}

		result.sign = sign;
		result.reduce();

		return result;
	}

	integer operator<<= (const uint64_t& shift)
	{
		*this = *this << shift;
		return *this;
	}

	integer operator << (const integer& shift) const
	{
		integer tmp = *this;
		tmp <<= shift.to_int();
		return tmp;
	}

	integer operator>> (const uint64_t& shift) const
	{
		integer result = integer();
		result.bits.resize(max(1ll, (int64_t)bits.size() - (int64_t)(shift >> 6)));

		for (size_t i = shift >> 6; i < bits.size(); ++i)
		{
			result.bits[i - (shift >> 6)] += bits[i] >> shift % 64;
			if (i > (shift >> 6) && shift % 64 != 0)
				result.bits[i - (shift >> 6) - 1] += bits[i] << (64 - shift % 64);
		}

		result.sign = sign;
		result.reduce();

		return result;
	}

	integer operator>>= (const uint64_t& shift)
	{
		*this = *this >> shift;
		return *this;
	}

	integer operator >> (const integer& shift) const
	{
		integer tmp = *this;
		tmp >>= shift.to_int();
		return tmp;
	}

	uint64_t addc(uint64_t& mem, uint64_t v, uint64_t in) const
	{
		uint64_t r1 = mem + v;
		uint64_t out = r1 < v;
		uint64_t r2 = r1 + in;
		out += r2 < in;
		mem = r2;
		return out;
	}

	uint64_t muladdc(uint64_t& mem, uint64_t a, uint64_t b, uint64_t in) const
	{
		uint64_t hi;
		uint64_t lo = _umul128(a, b, &hi);
		hi += addc(mem, lo, in);
		return hi;
	}

	integer omul(integer a, integer b) const
	{
		integer c{};
		c.bits.resize(a.bits.size() + b.bits.size());
		for (uint64_t i = 0; i < a.bits.size(); ++i)
		{
			uint64_t carry = 0, j = 0;
			while (j < b.bits.size())
			{
				carry = muladdc(c.bits[i + j], a.bits[i], b.bits[j], carry);
				++j;
			}
			c.bits[i + j] = carry;
		}
		c.reduce();
		return c;
	}

	integer operator* (const integer& o) const
	{
		integer result = omul(*this, o);
		result.sign = !(sign ^ o.sign);

		return result;
	}


	integer operator*= (const integer& o)
	{
		*this = *this * o;
		return *this;
	}

	uint64_t sig_bit() const
	{
		if (bits.size() == 1 && bits[0] == 0) return 0;

		uint64_t sz = (uint64_t)bits.size() << 6;
		bitset<64> b{ bits[bits.size() - 1] };

		uint64_t pos = 63;

		while (b[pos] != 1)
		{
			--sz;
			--pos;
		}

		return sz;
	}

	bool div_helper(integer& result, const integer& o, integer& div) const
	{

		for (int64_t i = sig_bit() - o.sig_bit(); i >= 0; --i)
		{
			if (div.sig_bit() < o.sig_bit() + i) continue;

			integer shifted = o << i;

			if (shifted <= div)
			{
				div -= shifted;
				div.reduce();
				result.bits[i >> 6] += ((uint64_t)1 << (i % 64));
			}
		}

		return div == 0;
	}

	integer operator/ (const integer& o) const
	{
		if (this->abs() < o.abs()) return integer();

		integer result = integer(), div = abs();
		result.bits.resize(bits.size() - o.bits.size() + 1);

		div_helper(result, o.abs(), div);

		result.reduce();
		result.sign = !(sign ^ o.sign);

		return result;
	}

	integer operator/= (const integer& o)
	{
		*this = *this / o;
		return *this;
	}

	integer operator% (const integer& o) const
	{
		return *this - (*this / o) * o;
	}

	integer operator%= (const integer& o)
	{
		*this = *this % o;
		return *this;
	}

	integer ceil_divide(const integer& o) const
	{
		if (*this < o)
		{
			if (*this == 0) return integer();
			return integer(1);
		}

		integer result = integer(), div = *this;
		result.bits.resize(bits.size() - o.bits.size() + 1);

		bool b = div_helper(result, o, div);

		result.reduce();

		result.sign = !(sign ^ o.sign);

		if (b) return result;
		return result + 1;
	}

	uint64_t ceil_lg() const
	{
		uint64_t lg = sig_bit();

		for (uint64_t i = 0; i < (uint64_t)bits.size() - 1; ++i)
		{
			if (bits[i] != 0) return lg;
		}

		bitset<64> b{ bits[bits.size() - 1] };

		for (uint64_t i = 0; i < 64; ++i)
		{
			if (b[i] == 1 && i != (lg % 64) - 1) return lg;
		}

		return lg - 1;
	}

	integer operator- () const
	{
		integer new_int = *this;
		new_int.sign ^= true;

		return new_int;
	}

	integer operator++ ()
	{
		*this += 1;
		return *this;
	}

	integer operator-- ()
	{
		*this -= 1;
		return *this;
	}
private:
	uint64_t lim = ((uint64_t)1 << 63) - 1;
};

static integer pow(integer a, integer b)
{
	integer new_int = 1;

	while (b > 0)
	{
		if (b.bits[0] & 1) new_int *= a;
		a *= a;
		b >>= 1;
	}

	return new_int;
}

static integer sqrt(integer a)
{
	integer res = (uint64_t)sqrt(a.bits[a.bits.size() - 1]);
	integer rem = a.bits[a.bits.size() - 1] - res.to_int() * res.to_int();
	integer left = res << 33;

	for (int64_t i = (int64_t)a.bits.size() - 2; i > -1; --i)
	{
		rem.bits.insert(rem.bits.begin(), a.bits[i]);
		integer l = 0, r = (uint64_t)1 << 32, m = 0;

		integer val = 0;

		while (l <= r)
		{
			m = (l + r) / 2;

			val = (left + m) * m;

			if (val > rem)
			{
				r = m - 1;
			}
			else if (val + left + m * 2 >= rem)
			{
				break;
			}
			else
			{
				l = m + 1;
			}
		}

		rem -= val;

		res <<= 32;
		res += m;

		left += (m * 2);
		left <<= 32;
	}

	return res;
}

static integer min(integer a, integer b)
{
	if (a < b)
	{
		return a;
	}
	return b;
}