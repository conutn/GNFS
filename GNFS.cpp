#include <bitset>
#include <cmath>
#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <tuple>
#include <vector>

#include "C:/Users/justi/source/inf_float.h"

using namespace std;

static integer mod_pow(integer base, integer exp, integer mod)
{
	base %= mod;
	integer res = 1;

	while (exp > 0)
	{
		if (exp.bits[0] & 1) res = res * base % mod;
		base = base * base % mod;
		exp >>= 1;
	}

	return res;
}

static tuple<integer, integer, integer> comp_barrett(integer n)
{
	uint64_t k = n.sig_bit();
	integer pow4{};
	pow4.bits.insert(pow4.bits.begin(), k >> 5, 0);
	pow4.bits[pow4.bits.size() - 1] = (uint64_t)1 << (2 * k % 64);
	integer r = pow4 / n;

	return { r, pow4, n };
}
class poly
{
public:
	vector<integer> coeff;
	vector<integer> fac;
	integer bar_r, bar_p, bar_n;
	poly() : coeff(), fac() {};
	poly(const uint64_t& i) : coeff(i) {};
	poly(const initializer_list<integer>& i) : coeff(i) {};
	poly(const vector<integer>& v) : coeff(v) {};
	poly(const vector<integer>& coeff, const integer& r, const integer& p, const integer& n)
		: coeff(coeff), bar_r(r), bar_p(p), bar_n(n) {}
	poly(poly& p) : coeff(p.coeff), fac(p.fac), bar_r(p.bar_r), bar_p(p.bar_p), bar_n(p.bar_n) {};
	poly(const poly& p) : coeff(p.coeff), fac(p.fac), bar_r(p.bar_r), bar_p(p.bar_p), bar_n(p.bar_n) {};
	~poly() {};

	void init_barrett(integer mod)
	{
		tuple<integer, integer, integer> b = comp_barrett(mod);
		bar_r = get<0>(b);
		bar_p = get<1>(b);
		bar_n = get<2>(b);
	}

	integer barrett(integer x) const
	{
		integer t = x - bar_n * ((x * bar_r) >> bar_p.ceil_lg());

		if (t < bar_n)
		{
			return t;
		}

		return t - bar_n;
	}

	integer operator () (integer x) const
	{
		integer res = coeff[0], mult = x;

		for (uint64_t i = 1; i < coeff.size(); ++i)
		{
			res += coeff[i] * mult;
			mult *= x;
		}

		return res;
	}

	poly reduce()
	{
		for (uint64_t i = coeff.size() - 1; i > 0; --i)
		{
			if (coeff[i] == 0)
			{
				coeff.erase(coeff.begin() + i);
			}
			else
			{
				break;
			}
		}

		if (coeff.size() == 0)
		{
			coeff.push_back(0);
		}

		return *this;
	}

	poly operator + (poly p) const
	{
		poly res(max(coeff.size(), p.coeff.size()));
		res.bar_n = bar_n, res.bar_p = bar_p, res.bar_r = bar_r;

		for (uint64_t i = 0; i < res.coeff.size(); ++i)
		{
			if (i < coeff.size())
			{
				res.coeff[i] += coeff[i];
			}
			if (i < p.coeff.size())
			{
				res.coeff[i] += p.coeff[i];
			}
		}

		res.reduce();

		return res;
	}

	poly operator += (poly p)
	{
		*this = *this + p;
		return *this;
	}

	poly operator - (poly p) const
	{
		for (uint64_t i = 0; i < p.coeff.size(); ++i)
		{
			p.coeff[i].sign = !p.coeff[i].sign;
		}

		return *this + p;
	}

	poly operator -= (poly p)
	{
		*this = *this - p;
		return *this;
	}

	poly operator * (poly p) const
	{
		poly res(coeff.size() + p.coeff.size() - 1);
		res.bar_n = bar_n, res.bar_p = bar_p, res.bar_r = bar_r;

		for (uint64_t i = 0; i < coeff.size(); ++i)
		{
			for (uint64_t j = 0; j < p.coeff.size(); ++j)
			{
				res.coeff[i + j] += coeff[i] * p.coeff[j];
			}
		}

		res.reduce();

		return res;
	}

	poly operator *= (poly p)
	{
		*this = *this * p;
		return *this;
	}

	bool zero()
	{
		reduce();

		if (coeff.size() == 1 && coeff[0] == 0)
		{
			return true;
		}

		return false;
	}

	void init_fac()
	{
		fac = vector<integer>(coeff.size());
		fac[0] = 1;

		for (uint64_t i = 1; i < fac.size(); ++i)
		{
			fac[i] = barrett(fac[i - 1] * i);
		}
	}

	integer nCr(integer n, integer r) const
	{
		return barrett(barrett(fac[n.to_int()] * mod_pow(fac[r.to_int()], bar_n - 2, bar_n)) *
			mod_pow(fac[(n - r).to_int()], bar_n - 2, bar_n));
	}

	poly shift(integer s)
	{
		vector<integer> pows(coeff.size());
		pows[0] = 1;

		for (uint64_t i = 1; i < pows.size(); ++i)
		{
			pows[i] = barrett(pows[i - 1] * -s);
		}

		init_fac();
		poly res = *this;

		for (uint64_t i = 0; i < res.coeff.size(); ++i)
		{
			res.coeff[i] = (bar_n + res.coeff[i] % bar_n) % bar_n;
			for (uint64_t j = 0; j < i; ++j)
			{
				res.coeff[j] = (bar_n + ((pows[i - j] * nCr(i, j) * res.coeff[i])
					+ res.coeff[j]) % bar_n) % bar_n;
			}
		}

		return res;
	}

	poly mod(poly p) const
	{
		if (coeff.size() == 1)
		{
			poly res = *this;
			res.coeff[0] = (bar_n + res.coeff[0] % bar_n) % bar_n;
			return res;
		}

		p = poly{ p.reduce().coeff, bar_r, bar_p, bar_n };

		poly tmp = *this;

		for (uint64_t i = 0; i < tmp.coeff.size(); ++i)
		{
			tmp.coeff[i] = (bar_n + tmp.coeff[i] % bar_n) % bar_n;
		}

		while (tmp.coeff.size() >= p.coeff.size())
		{
			integer mult = barrett(tmp.coeff[tmp.coeff.size() - 1] *
				mod_pow(p.coeff[p.coeff.size() - 1], bar_n - 2, bar_n));

			for (uint64_t i = 0; i < p.coeff.size(); ++i)
			{
				p.coeff[i] = barrett(p.coeff[i] * mult);
			}

			poly zero = p;
			zero.coeff.insert(zero.coeff.begin(), tmp.coeff.size() - zero.coeff.size(), 0);
			tmp -= zero;

			for (uint64_t i = 0; i < tmp.coeff.size(); ++i)
			{
				tmp.coeff[i] = barrett(bar_n + barrett(tmp.coeff[i]));
			}

			tmp.reduce();
		}

		return tmp;
	}

	poly init_reduce() const
	{
		poly x = *this;

		for (uint64_t i = 0; i < x.coeff.size(); ++i)
		{
			x.coeff[i] = (bar_n + x.coeff[i] % bar_n) % bar_n;
		}

		vector<poly> pows(bar_n.sig_bit());
		pows[0] = poly{ vector<integer>{0, 1}, bar_r, bar_p, bar_n };

		for (uint64_t i = 1; i < pows.size(); ++i)
		{
			pows[i] = (pows[i - 1] * pows[i - 1]).mod(x);
		}

		poly res{ vector<integer>{1}, bar_r, bar_p, bar_n };

		for (uint64_t i = 0; i < bar_n.bits.size(); ++i)
		{
			bitset<64> b{ bar_n.bits[i] };
			for (uint64_t j = 0; j < 64; ++j)
			{
				if (b[j])
				{
					res = (res * pows[(i << 6) + j]).mod(x);
				}
			}
		}

		res.coeff[1] -= 1;
		res.reduce();

		return res;
	}

	poly gcd(poly x) const
	{
		poly a = *this, b = { x.coeff, bar_r, bar_p, bar_n }, ret{ vector<integer>{1}, bar_r, bar_p, bar_n };

		if (a.coeff.size() == 1 && b.coeff.size() == 1)
		{
			return ret;
		}

		for (uint64_t i = 0; i < max(a.coeff.size(), b.coeff.size()); ++i)
		{
			if (i < a.coeff.size())
			{
				a.coeff[i] = (bar_n + a.coeff[i] % bar_n) % bar_n;
			}
			if (i < b.coeff.size())
			{
				b.coeff[i] = (bar_n + b.coeff[i] % bar_n) % bar_n;
			}
		}
		while (a.coeff.size() > 1 && b.coeff.size() > 1)
		{
			if (a.coeff.size() > b.coeff.size())
			{
				a = a.mod(b);
			}
			else
			{
				b = b.mod(a);
			}
		}
		if (a.coeff.size() > b.coeff.size())
		{
			if (b.coeff[0] == 0)
			{
				return a;
			}
			return ret;
		}
		if (a.coeff[0] == 0)
		{
			return b;
		}
		return ret;
	}

	poly synth_div(integer i) const
	{
		poly res{ vector<integer>(coeff.size() - 1), bar_r, bar_p, bar_n };
		integer carry = 0;
		i = i % bar_n;

		for (uint64_t j = coeff.size() - 1; j > 0; --j)
		{
			res.coeff[j - 1] = barrett(coeff[j] + carry);
			carry = barrett(res.coeff[j - 1] * i);
		}

		return res;
	}
};

static integer legendre_symbol(integer a, integer p)
{
	if (p == 2)
	{
		return 1;
	}
	integer res = mod_pow(a, (p - 1) / 2, p);

	if (res == p - 1)
	{
		return -1;
	}
	return res;
}

static uint64_t get_deg(integer n)
{
	if (n.sig_bit() * log10_2 < 80)
	{
		return 3;
	}
	return 5;
}

static poly get_poly(integer n, integer m, uint64_t d)
{
	poly res(d + 1);

	for (uint64_t i = 0; i <= d; ++i)
	{
		integer tmp = n / m;
		res.coeff[i] = n - tmp * m;
		n = tmp;
	}

	return res;
}

static vector<integer> root_recur(poly f, integer val)
{
	if (f.coeff.size() == 1)
	{
		return vector<integer>();
	}
	if (f.coeff.size() == 2)
	{
		integer mult = mod_pow(f.coeff[1], f.bar_n - 2, f.bar_n);
		return vector<integer>{f.bar_n - f.barrett(f.coeff[0] * mult)};
	}

	integer shift = ++val, power = (f.bar_n - 1) / 2, offset = -1;
	vector<integer> sols;
	poly res, shifted;

	do
	{
		shifted = f.shift(++offset);

		if (shifted(0) % f.bar_n == 0)
		{
			sols.push_back(0);
			shifted.coeff.erase(shifted.coeff.begin());
			vector<integer> rr = root_recur(shifted, offset);
			sols.insert(sols.begin(), rr.begin(), rr.end());

			for (uint64_t i = 0; i < sols.size(); ++i)
			{
				sols[i] = f.barrett(sols[i] + f.bar_n - offset);
			}

			return sols;
		}

		vector<poly> pows(power.sig_bit());
		pows[0] = poly{ {0, 1}, f.bar_r, f.bar_p, f.bar_n };

		for (uint64_t i = 1; i < pows.size(); ++i)
		{
			pows[i] = (pows[i - 1] * pows[i - 1]).mod(shifted);
		}

		res = poly{ vector<integer>{1}, f.bar_r, f.bar_p, f.bar_n };

		for (uint64_t i = 0; i < power.bits.size(); ++i)
		{
			bitset<64> b{ power.bits[i] };
			for (uint64_t j = 0; j < 64; ++j)
			{
				if (b[j])
				{
					res = (res * pows[(i << 6) + j]).mod(shifted);
				}
			}
		}
	} while (res.coeff.size() == 1 && (res.coeff[0] == 1 || res.coeff[0] == f.bar_n - 1));

	res.coeff[0] = f.barrett(res.coeff[0] + 1);
	vector<integer> r1 = root_recur(shifted.gcd(res), offset);
	sols.insert(sols.begin(), r1.begin(), r1.end());

	res.coeff[0] = f.barrett(res.coeff[0] - 2);
	vector<integer> r2 = root_recur(shifted.gcd(res), offset);
	sols.insert(sols.begin(), r2.begin(), r2.end());

	for (uint64_t i = 0; i < sols.size(); ++i)
	{
		sols[i] = f.barrett(sols[i] + f.bar_n - offset);
	}

	return sols;
}

static vector<integer> roots(poly f, integer p)
{
	f.init_barrett(p);
	
	poly x = f.init_reduce();
	poly g = f.gcd(x);
	
	vector<integer> res = root_recur(g, -1);
	
	for (uint64_t i = 0; i < res.size(); ++i)
	{
		res[i] = f.barrett(f.bar_n + f.barrett(res[i]));
	}
	
	sort(res.begin(), res.end());
	
	return res;
}

static integer norm(poly f, integer a, integer b)
{
	vector<integer> a_pow(f.coeff.size()), b_pow(f.coeff.size());
	a_pow[0] = 1, b_pow[b_pow.size() - 1] = 1;

	for (uint64_t i = 1; i < a_pow.size(); ++i)
	{
		a_pow[i] = a_pow[i - 1] * a;
		b_pow[b_pow.size() - i - 1] = b_pow[b_pow.size() - i] * b;
	}

	integer res = 0;

	for (uint64_t n = 0; n < f.coeff.size(); ++n)
	{
		integer x = f.coeff[n] * a_pow[n] * b_pow[n];

		if ((f.coeff.size() + n) & 1)
		{
			res += x;
		}
		else
		{
			res -= x;
		}
	}
	
	return res;
}

static vector<pair<integer, integer>> rat_factor_base(integer m, uint64_t upper_limit)
{
	vector<uint64_t> primes = gen_prime(upper_limit);
	vector<pair<integer, integer>> res;

	for (uint64_t p : primes)
	{
		res.push_back({ m % p, p });
	}
	
	return res;
}

static vector<pair<integer, integer>> alg_factor_base(poly f, uint64_t upper_limit)
{
	vector<uint64_t> primes = gen_prime(upper_limit);
	vector<pair<integer, integer>> res;

	for (uint64_t p : primes)
	{
		vector<integer> r = roots(f, p);
		for (integer i : r)
		{
			res.push_back({ i, (integer)p });
		}
	}

	return res;
}

static vector<pair<integer, integer>> quad_factor_base(poly f, uint64_t lower_limit, uint64_t upper_limit)
{
	vector<uint64_t> primes = gen_prime(upper_limit);
	vector < pair<integer, integer>> res;
	vector<uint64_t>::iterator iter = upper_bound(primes.begin(), primes.end(), lower_limit);
	
	while (iter != primes.end())
	{
		vector<integer> r = roots(f, *iter);
		for (integer i : r)
		{
			res.push_back({ i, (integer)(*iter) });
		}
		++iter;
	}

	return res;
}

static vector<vector<uint64_t>> get_primes(uint64_t bound, integer max_prime)
{
	vector<vector<uint64_t>> res;
	res.push_back(gen_prime(max_prime.to_int()));

	while (true)
	{
		bool reached = false;
		vector<uint64_t> p;

		for (uint64_t i = 0; i < res[res.size() - 1].size() && res[res.size() - 1][i] * res[0][i] <= bound; ++i)
		{
			p.push_back(res[res.size() - 1][i] * res[0][i]);
			reached = true;
		}

		res.push_back(p);

		if (!reached)
		{
			res.erase(res.end() - 1);
			return res;
		}
	}
}

static vector<pair<integer, integer>> R_smooth(integer m, uint64_t bound, vector<pair<integer, integer>> R,
	uint64_t req)
{
	uint64_t b = bound;
	vector<pair<integer, integer>> res;
	vector<vector<uint64_t>> plist = get_primes(bound, R[R.size() - 1].second);
	vector<flt> lnlist;

	for (uint64_t i = 0; i < plist[0].size(); ++i)
	{
		lnlist.push_back(ln((uint64_t)32, flt{ 0, plist[0][i] }));
	}

	while (res.size() < req)
	{
		++b;
		vector<flt> arr(bound, -ln((uint64_t)32, flt{ 0, m * b }));

		for (uint64_t i = 0; i < plist.size(); ++i)
		{
			for (uint64_t j = 0; j < plist[i].size(); ++j)
			{
				int64_t tmp = b * (m % plist[i][j]).to_int();
				uint64_t k = tmp / plist[i][j] + (tmp % plist[i][j] > 0);

				while (-tmp + plist[i][j] * k < bound)
				{
					arr[-tmp + plist[i][j] * k++] += lnlist[j];
				}
			}
		}
		
		for (uint64_t i = 0; i < arr.size(); ++i)
		{
			if (arr[i] > -flt{ -3, 1 })
			{
				res.push_back({ i, b });
			}
		}
	}

	return res;
}

static integer GNFS(integer n)
{
	uint64_t d = get_deg(n);
	integer m = to_integer(nroot((uint64_t)256, div((uint64_t)256, flt{ 0, 1 }, n), integer{ d }));
	m = 31;
	poly f = get_poly(n, m, d);
	
	vector<pair<integer, integer>> R = rat_factor_base(m, 29),
		A = alg_factor_base(f, 103),
		Q = quad_factor_base(f, 103, 109);

	for (pair<integer, integer>k : R_smooth(m, 1000, R, 40)) {
		cout << k.first << " " << k.second << '\n';
	}
	
	return 1;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
	integer k = 45113;
	//GNFS(k);
}