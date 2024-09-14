#include "integer.h"
#include "printing_press.h"

#pragma once

using namespace std;

constexpr double log10_2 = 0.301029995664;

static vector<uint64_t> gen_prime(uint64_t lim)
{
	vector<bool> sieve = vector<bool>(lim + 1);

	if (lim >= 2)
		sieve[2] = true;
	if (lim >= 3)
		sieve[3] = true;

	for (uint64_t x = 1; x * x < lim; ++x)
	{
		for (uint64_t y = 1; y * y < lim; ++y)
		{
			uint64_t n = 4 * x * x + y * y;
			if (n <= lim && (n % 12 == 1 || n % 12 == 5))
				sieve[n] = sieve[n] ^ true;

			n = 3 * x * x + y * y;
			if (n <= lim && n % 12 == 7)
				sieve[n] = sieve[n] ^ true;

			n = 3 * x * x - y * y;
			if (n <= lim && x > y && n % 12 == 11)
				sieve[n] = sieve[n] ^ true;
		}
	}

	for (uint64_t r = 5; r * r <= lim; ++r)
		if (sieve[r])
			for (uint64_t i = r * r; i <= lim; i += r * r)
				sieve[i] = false;

	vector<uint64_t> list;

	for (uint64_t n = 1; n <= lim; ++n)
		if (sieve[n] == true)
			list.push_back(n);

	return list;
}

class flt
{
public:
	flt() : a(0), n(0), precision(0) {};
	flt(integer _a, integer _n) : a(_a), n(_n), precision(0)
	{
		n.sign = _n.sign;
	};
	~flt() {};
	integer a, n;
	uint64_t precision;
};

static void reduce(flt & r)
{
	if (r.n == 0)
	{
		r.a = 0;
		return;
	}

	uint64_t pow = 0;

	for (uint64_t i = 0; i < r.n.bits.size(); ++i)
	{
		if (r.n.bits[i] == 0) pow += 64;
		else break;
	}

	bitset<64> b{ r.n.bits[pow >> 6] };

	for (uint64_t i = 0; i < 64; ++i)
	{
		if (b[i] == 0) ++pow;
		else break;
	}

	r.a += integer{ pow };
	r.n >>= pow;
}

static bool operator< (const flt & r, const flt & s)
{
	integer f = min(r.a, s.a);
	if (r.n.sign != s.n.sign)
	{
		return !r.n.sign;
	}
	if (r.a + r.n.sig_bit() > s.a + s.n.sig_bit() + 5)
	{
		return !r.n.sign;
	}
	if (r.a + r.n.sig_bit() < s.a + s.n.sig_bit() - 5)
	{
		return r.n.sign;
	}
	return (r.n << (r.a - f) < (s.n << (s.a - f)));
}

static bool operator<= (const flt& r, const flt& s)
{
	integer f = min(r.a, s.a);
	if (r.n.sign != s.n.sign)
	{
		return !r.n.sign;
	}
	if (r.a + r.n.sig_bit() > s.a + s.n.sig_bit() + 5)
	{
		return !r.n.sign;
	}
	if (r.a + r.n.sig_bit() < s.a + s.n.sig_bit() - 5)
	{
		return r.n.sign;
	}
	return (r.n << (r.a - f)) <= (s.n << (s.a - f));
}

static bool operator> (const flt & r, const flt & s)
{
	integer f = min(r.a, s.a);
	if (r.n.sign != s.n.sign)
	{
		return r.n.sign;
	}
	if (r.a + r.n.sig_bit() > s.a + s.n.sig_bit() + 5)
	{
		return r.n.sign;
	}
	if (r.a + r.n.sig_bit() < s.a + s.n.sig_bit() - 5)
	{
		return !r.n.sign;
	}
	return (r.n << (r.a - f)) > (s.n << (s.a - f));
}

static bool operator>= (const flt & r, const flt & s)
{
	if (r.n.sign != s.n.sign)
	{
		return r.n.sign;
	}
	integer f = min(r.a, s.a);
	if (r.a + r.n.sig_bit() > s.a + s.n.sig_bit() + 5)
	{
		return r.n.sign;
	}
	if (r.a + r.n.sig_bit() < s.a + s.n.sig_bit() - 5)
	{
		return !r.n.sign;
	}
	return (r.n << (r.a - f)) >= (s.n << (s.a - f));
}

static bool operator== (const flt & r, const flt & s)
{
	integer f = min(r.a, s.a);
	if (r.a + r.n.sig_bit() > s.a + s.n.sig_bit() + 5)
	{
		return false;
	}
	if (r.a + r.n.sig_bit() < s.a + s.n.sig_bit() - 5)
	{
		return false;
	}
	return (r.n << (r.a - f)) == (s.n << (s.a - f));
}

static bool operator!= (const flt & r, const flt & s)
{
	integer f = min(r.a, s.a);
	if (r.a + r.n.sig_bit() > s.a + s.n.sig_bit() + 5)
	{
		return true;
	}
	if (r.a + r.n.sig_bit() < s.a + s.n.sig_bit() - 5)
	{
		return true;
	}
	return (r.n << (r.a - f)) != (s.n << (s.a - f));
}

static flt operator+ (const flt & r, const flt & s)
{
	integer f = min(r.a, s.a);
	flt x { f, (r.n << (r.a - f)) + (s.n << (s.a - f)) };
	x.precision = min(r.precision, s.precision);
	return x;
}

static flt operator+= (flt & r, const flt & s)
{
	r = r + s;
	return r;
}

static flt operator- (const flt & r, const flt & s)
{
	if (!s.n.sign)
	{
		flt tmp = s;
		tmp.n.sign = true;
		return r + tmp;
	}

	integer f = min(r.a, s.a);
	flt x { f, (r.n << (r.a - f)) - (s.n << (s.a - f)) };
	x.precision = min(r.precision, s.precision);
	return x;
}

static flt operator-= (flt & r, const flt & s)
{
	r = r - s;
	return r;
}

static flt operator* (const flt & r, const flt & s)
{
	flt x { r.a + s.a, r.n * s.n };
	x.precision = min(r.precision, s.precision);
	return x;
}

static flt operator*= (flt & r, const flt & s)
{
	r = r * s;
	return r;
}

static flt operator- (const flt& f)
{
	flt res = f;
	res.n.sign ^= true;
	return res;
}

static flt _abs(flt & r)
{
	flt new_flt = r;
	new_flt.n.sign = true;

	return new_flt;
}

template <typename M>
static flt abs(M && r)
{
	return _abs(r);
}

static integer _to_integer(flt & r)
{
	if (abs(r) < flt{ 0, 1 }) return 0;
	if (r.a < 0) return r.n >> (-r.a);
	return r.n << r.a;
}

template <typename M>
static integer to_integer(M && r)
{
	return _to_integer(r);
}

static flt _div(uint64_t& precision, flt& r, const integer& k)
{
	flt res{};
	res.a = r.a + r.n.sig_bit() - k.ceil_lg() - precision;

	if (r.n.sig_bit() > k.ceil_lg() + precision)
		res.n = r.n / (k << (r.n.sig_bit() - k.ceil_lg() - precision));
	else
		res.n = (r.n << (k.ceil_lg() + precision - r.n.sig_bit())) / k;

	if (abs(res) > flt{})
		reduce(res);

	res.n.sign = !(r.n.sign ^ k.sign);

	return res;
}

template <typename M, typename N, typename O>
static flt div(M&& precision, N&& r, O&& k)
{
	flt res = _div(precision, r, k);
	res.precision = (uint64_t)(precision * log10_2);
	return res;
}

static flt _trunc(uint64_t& precision, flt& r)
{
	flt res{};
	res.a = r.a + r.n.sig_bit() - precision;

	if (r.n.sig_bit() > precision)
		res.n = r.n >> (r.n.sig_bit() - precision);
	else
		res.n = r.n << (precision - r.n.sig_bit());
	
	reduce(res);

	return res;
}

template <typename M, typename N>
static flt trunc(M&& precision, N&& r)
{
	flt res = _trunc(precision, r);
	res.precision = (uint64_t)(precision * log10_2);
	return res;
}

static flt _pow(uint64_t& precision, flt& r, const integer& k)
{
	if (k == 1) return trunc(precision, r);

	if ((k.bits[0] & 1) == 0)
	{
		flt tmp{ _pow(precision, r, k >> 1) };
		return trunc(precision, tmp * tmp);
	}

	return trunc(precision, _pow(precision, r, k - 1) * trunc(precision, r));
}

template <typename M, typename N, typename O>
static flt pow(M&& precision, N&& r, O&& k)
{
	flt res = _pow(precision, r, k);
	res.precision = (uint64_t)(precision * log10_2);
	return res;
}

static flt _B(uint64_t& precision, flt& y, const integer& k)
{
	flt tmp = y;
	reduce(tmp);

	integer g = tmp.a + (tmp.n == 1) + tmp.n.sig_bit();

	int64_t a = (-g / k).to_int(), B = ((k * 2 + 1) * 66).ceil_lg();
	flt z{ a - 1, 3 };
	uint64_t uB = (uint64_t)B;
	for (int64_t j = 0; j < (int64_t)precision; ++j)
	{
		flt r = trunc(uB, pow(uB, z, k) * trunc(uB, y));
		if (r <= flt{ -10, 993 }) z += flt{ a - j - 1, 1 };
		if (r > flt{ 0, 1 }) z -= flt{ a - j - 1, 1 };
	}
	
	return z;
}

template <typename M, typename N, typename O>
static flt B(M&& precision, N&& y, O&& k)
{
	return _B(precision, y, k);
}

static flt _N(uint64_t& precision, flt& y, const integer& k)
{
	uint64_t bp = 1 + k.ceil_lg() + (int64_t)((precision - 1 - k.ceil_lg()) >> 1);
	uint64_t B = 2 * bp + 4 - k.ceil_lg();
	flt z;

	if (bp <= 3 + k.ceil_lg()) z = _B(bp, y, k);
	else z = _N(bp, y, k);

	flt r2 = trunc(B, z) * flt { 0, k + 1 };
	flt r3 = trunc(B, pow(B, z, k + 1) * trunc(B, y));

	return div(B, r2 - r3, k);
}

template <typename I, typename J, typename K>
static flt N(I&& precision, J&& y, K&& k)
{
	return _N(precision, y, k);
}

static flt _nroot(uint64_t& precision, flt& y, const integer& k)
{
	if (precision >= 4 + k.ceil_lg()) return N(precision, y, k);
	return B(precision, y, k);
}

template <typename M, typename N, typename O>
static flt nroot(M&& precision, N&& y, O&& k)
{
	flt t = _nroot((uint64_t&)precision, y, k);
	reduce(t);
	t.precision = (uint64_t)(precision * log10_2);
	return t;
}

static int64_t _C(integer& n, integer& x, integer& k)
{
	int64_t f = n.sig_bit(), b = 1;

	while (true)
	{
		flt r = pow(b + 3 + k.ceil_lg(), flt{ 0, x }, k);

		flt fn = flt{ 0, n };
		if (fn < r) return -1;

		integer tmp = 1;
		tmp <<= b;
		if (r * flt{ -b, tmp + 1 } <= fn) return 1;

		if (b >= f) return 0;

		b = min(b << 1, f);
	}
}

template <typename M, typename N, typename O>
static int64_t C(M&& n, N&& x, O&& k)
{
	return _C(n, x, k);
}

static integer _K(integer& n, integer& k, flt& y)
{
	integer f = n.sig_bit(), b = f / k + (f % k != 0) + 3;
	flt r = nroot(b.to_int(), y, k);
	integer x = to_integer(r);

	if (abs(r - flt{ 0, x }) > flt{ -3, 5 }) x += 1;
	if (x == 0 || abs(r - flt{ 0, x }) >= flt{ -2, 1 }) return 0;

	if (C(n, x, k) == 0) return x;

	return 0;
}

template <typename M, typename N, typename O>
static integer K(M&& n, N&& k, O&& y)
{
	return _K(n, k, y);
}

static pair<integer, integer> X(integer& n)
{
	uint64_t f = n.sig_bit();
	flt y = div(3 + ((f + 1) >> 1), flt{ 0, 1 }, n);

	vector<uint64_t> primes = gen_prime(f - 1);

	for (uint64_t i = 0; i < primes.size(); ++i)
	{
		integer p = primes[i];
		integer x = K(n, p, y);

		if (x > 0) return make_pair(x, primes[i]);
	}

	return make_pair(n, 1);
}

static flt _fpow(uint64_t& precision, flt& x, flt& y)
{
	if (y.a < 0)
	{
		flt k = pow(precision, x, y.n);
		return nroot(precision, div(precision, flt{ -k.a, 1 }, k.n), (integer)1 << -y.a);
	}
	return pow(precision, x, to_integer(y));
}

template <typename M, typename N, typename O>
static flt fpow(M&& precision, N&& x, O&& y)
{
	flt res = _fpow(precision, x, y);
	res.precision = (uint64_t)(precision * log10_2);
	return res;
}

bool done = false;
vector<flt> logs, pows;

static flt _ln(uint64_t precision, flt n)
{
	if (!done)
	{
		ifstream ifs;
		ifs.open("logarithm.txt");
		logs.push_back(flt{});
		pows.push_back(flt{});
		for (int64_t i = 0; i < 256; ++i)
		{
			flt f{};
			int64_t p;
			ifs >> p;
			f.a = p;
			int64_t len;
			ifs >> len;
			vector<uint64_t> bits(len);
			for (int64_t j = 0; j < len; ++j)
			{
				ifs >> bits[j];
			}
			f.n.bits = bits;
			logs.push_back(f);
			pows.push_back(flt{ 0, 1 } + flt{ -i - 1, 1 });
		}
	}

	flt P = { 1, 1 };
	flt next = { -1, 3 };
	flt res{};

	while (next <= n)
	{
		P = next;
		next *= pows[1];
		res += trunc(precision, logs[1]);
	}
	for (int64_t i = 2; i <= 256; ++i)
	{
		next = trunc(precision, P * pows[i]);

		if (next <= n)
		{
			P = next;
			res += trunc(precision, logs[i]);
			res = trunc(precision, res);
		}
	}

	return res;
}

template <typename M, typename N>
static flt ln(M&& precision, N&& n)
{
	flt res = _ln(precision, n);
	res.precision = (uint64_t)(precision * log10_2);
	return res;
}

vector<string> power5{ "5" };
bool use_prec = true;

class _flt_manip
{
public:
	_flt_manip(bool precise) : prec(precise) {};
	~_flt_manip() {};
	bool prec;
};

_flt_manip _no_prec(false), _yes_prec(true);

ostream& operator << (ostream& o, _flt_manip f)
{
	use_prec = f.prec;
	return o;
}

ostream& operator << (ostream& o, flt i)
{
	if (i.n == 0)
	{
		o << "0";
		return o;
	}

	uint64_t p = i.precision;
	
	if (!i.n.sign)
	{
		o << "-";
	}
	i.n.sign = true;
	o << to_integer(i);
	i -= flt{ 0, to_integer(i) };

	if (i == flt{ 0 ,0 })
	{
		return o;
	}
	
	reduce(i);
	
	while (integer{ -(int64_t)power5.size() } > i.a)
	{
		string tmp = add(power5[power5.size() - 1], power5[power5.size() - 1]);
		tmp = add(tmp, tmp);
		power5.push_back(add(tmp, power5[power5.size() - 1]));
	}
	
	string res = "0", zeroes = "", front = "";
	int64_t idx = -i.a.to_int() - 1;

	for (uint64_t j = 0; j < i.n.bits.size(); ++j)
	{
		bitset<64> b{ i.n.bits[j] };
		for (uint64_t k = 0; k < 64; ++k, --idx)
		{
			if (idx < 0)
			{
				break;
			}
			if (b[k])
			{
				res = add(res, power5[idx] + zeroes);
			}
			zeroes += "0";
		}
	}

	i.a.sign = true;
	
	for (uint64_t j = 0; j < i.a.to_int() - res.length(); ++j)
	{
		front += "0";
	}

	res = front + res;

	if (use_prec)
	{
		res = res.substr(0, p);
	}

	o << "." << res;

	return o;
}