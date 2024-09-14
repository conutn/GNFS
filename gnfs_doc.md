# What do these functions do?
### mod_pow(integer base, integer exp, integer mod)
returns base ^ exp % mod.
### comp_barrett(integer n)
returns a tuple enabling the quick computation of numbers % a constant n. Read more at https://en.wikipedia.org/wiki/Barrett_reduction
### legendre_symbol(integer a, integer p)
returns the Legendre symbol (a|p) https://en.wikipedia.org/wiki/Legendre_symbol
### get_deg(integer n)
returns the optimal degree of the polynomial to be used in GNFS when factoring the integer 'n'
### get_poly(integer n, integer m, integer d)
returns the polynomial used in factoring the integer 'n'
### roots(poly f, integer p)
returns the roots of 'f' mod 'p'
### root_recur(poly f, integer val)
a helper function for 'roots'
### norm(poly f, integer a, integer b)
returns the norm of a+b*theta
### rat_factor_base(integer m, uint64_t upper_limit)
returns the rational factor base up to the specified limit (inclusive)
### alg_factor_base(poly f, uint64_t upper_limit)
returns the algebraic factor base up to the specified limit (inclusive)
### quadratic_factor_base(poly f, uint64_t lower_limit, uint64_t upper_limit)
returns the quadratic factor base from the  'lower_limit' (exclusive) to the 'upper_limit' (inclusive)
### get_primes(uint64_t bound, integer max_prime)
returns the powers of primes where the primes range from 2 to 'max_prime' ('bound' specifies the upper bound)

**get_primes(200, 11) would return {{2, 3, 5, 7, 11}, {4, 9, 25, 49, 121}, {8, 27, 125}, {16, 81}, {32}, {64}, {128}}**
### R_smooth(integer m, uint64_t bound, vector<pair<integer, integer>> R, uint64_t req)
returns a list of pairs of integers representing elements which are 'R-smooth'
### GNFS(integer n)
returns a non-trivial factor of 'n' **\*to implement\***
## The 'poly' class
A representation of polynomials with integer coefficients.
### Data
*coeff* - the vector of coefficients (coeff\[0\] corresponds to the coefficient of x^0)

*fac* - a vector storing large factorials modulo a prime

*bar_r, bar_p, bar_n* - components of barrett reduction, 'bar_n' equals the modulus
### Constructors
*default constructor* - intializes 'coeff' and 'fac' to empty vectors

*uint64_t constructor* - intializes 'coeff' to a vector of that size and 'fac' to an empty vector

*initializer_list constructor* - initalizes 'coeff' to the values inside the initializer_list and 'fac' to an empty vector

*vector, integer, integer, integer constructor* - initializes 'coeff' to the vector and 'bar_r', 'bar_p', and 'bar_n' to the respective values. 'fac' is initialzed to an empty vector

*copy constructor* - there is also a copy constructor
### Operators
*evaluation* - evaluates the polynomial (takes in an integer as a parameter), accessible through '()'

*Addition* - accessible through '+'

*Subtraction* - accessible through '-'

*Multiplication* - accessible through '*'
#### There exist in-place operators '+=', '-=', and '*='. Note the lack of the division operator
### Instance methods
*init_barrett(integer mod)* - initializes 'bar_r', 'bar_p', and 'bar_n' to enable barrett reduction modulo 'mod'

*barrett(integer x)* - returns 'x' modulo 'bar_n' via barrett reduction

*reduce()* - gets rid of unnecessary zero coefficients in the front of the polynomial (0x^3+2x^2+1 -> 2x^2+1)

*zero()* - returns true if this is the zero polynomial and false otherwise

*init_fac()* - initializes 'fac' to a vector the same size as 'coeff' and fills them with the respective factorials modulo 'bar_n' (fac\[4\] would contain 24 modulo 'bar_n')

*nCr(integer n, integer r)* - returns 'n' choose 'r' modulo 'bar_n'

*shift(integer s)* - returns the polynomial shifted right by 's' units

*mod(poly p)* - returns this polynomial modulo 'p' in the ring Z/(bar_n)Z

*init_reduce()* - returns x^(bar_n)-x modulo this polynomial

*gcd(poly p)* - returns the gcd of this polynomial and 'p' in the ring Z/(bar_n)Z

*synth_div(integer i)* - returns the remainder when synthetically dividing this polynomial by x-i
