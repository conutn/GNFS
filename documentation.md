# How to use these classes

## integer.h
### Data
*sign* - true indicates a positive integer, false indicates a negative integer. There is a negative 0, which is equal to positive 0.

*bits* - an array of bits, with the bits starting at bits\[0\] being the least significant and the ones at bits\[bits.size() - 1\] being the most significant.
### Constructors
*Default constructor* - instantiates integer with a value of 0.

*int constructor* - instantiates integer with a value that was passed in.

*Copy constructor* - there is also a copy constructor.
### Operators
*Addition* - accessible through '+'

*Subtraction* - accessible through '-'

*Multiplication* - accessible through '*'

*Division* - floor division, accessible through '/'

*Left shift* - shifts the bits leftwards, accessible through '<<' (does not affect the sign)

*Right shift* - shifts the bits rightwards, accessible through '>>' (does not affect the sign)
#### There exist in-place operators '+=', '-=', '*=', '/=', '<<=', '>>='
*Equality* - accessible through '==', returns true if sign is the same and value in bits is the same, false otherwise

*Inequality* - accessible through '!=', returns false if values are equal and true otherwise

*Less than* - accessible through '<'

*Greater than* - accessible through '>'

*Less than or equal to* - accessible through '<='

*Greater than or equal to* - accessible through '>='
### Instance methods
*.abs()* - returns the absolute value

*.sig_bit()* - returns the position (not the index) of the most significant set bit. If the integer a was equal to 26 (11010), then a.sig_bit() would return 5.

*.ceil_divide(const integer& o)* - returns the result when using ceiling division to divide by 'o'

*.ceil_lg()* - returns the result when taking the ceiling of the base 2 logarithm. If the integer a was equal to 16 (10000), then a.ceil_lg() would return 4.

*.to_int()* - returns the integer represented by the first entry in bits (with the sign)
### Other methods
*pow(integer a, integer b)* - returns the result of raising a to the bth power

*sqrt(integer a)* - returns the floor of the square root of a

*min(integer a, integer b)* - returns the minimum of a and b

*max(integer a, integer b)* - returns the maximum of a and b
## float.h NOTE THAT THE CLASS IS ACTUALLY CALLED 'flt'
### Data
*n* - indicates the 'integer' part
*a* - indicates the power of 2 to multiply n by to get the value actually represented by the float, can be negative
*precision* - indicates the precision when printing out this number
### Constructors
*Default constructor* - instantiates a to 0, n to 0, and precision to 0

*integer, integer constructor* - instantiates a to first integer, n to second integer, and precision to 0 (takes in parameters of type integer)
### Operators
*Addition* - accessible through '+'

*Subtraction* - accessible through '-'

*Multiplication* - accessible through '*'

*Division* - floor division, accessible through '/'

*Left shift* - shifts the bits leftwards, accessible through '<<'

*Right shift* - shifts the bits rightwards, accessible through '>>'
#### There exist in-place operators '+=', '-=', '*=', '/=', '<<=', '>>='
*Equality* - accessible through '==', returns true if the values are the same (takes into account different representations of the same number)

*Inequality* - accessible through '!=', returns false if values are equal and true otherwise

*Less than* - accessible through '<'

*Greater than* - accessible through '>'

*Less than or equal to* - accessible through '<='

*Greater than or equal to* - accessible through '>='
### Instance methods
There are no instance methods.
### Other methods
*reduce(flt& r)* - increases the value of 'a' as much as it can without losing precision (basically increases a by 1 and divides n by 2 until n is an odd integer)

*to_integer(flt& r)* - returns the closest integer approximation of 'r'

*div(uint64_t& precision, flt& r, const integer& k)* - returns the result r/k to the precision specified

*trunc(uint64_t& precision, flt& r)* - returns the result when truncating r to the precision specified (chopping off the least significant bits)

*pow(uint64_t& precision, flt& r, const integer& k)* - returns the result r^k to the precision specified

*fpow(uint64_t& precision, flt& x, flt& y)* - returns the result x^y to the precision specified \*\*slow\*\*

*ln(uint64_t& precision, flt& r)* - returns the result ln(r) to the precision specified
## printing_press.h
This class only contains methods, mostly for debugging / utility.

*reset_timer()* - resets a timer

*time_passed()* - prints the time passed since resetting the timer
### Printing to streams
This class enables a number of classes to be printed, all of which are listed below.

*vector* - prints a vector in the format "{ ... }"

*pair* - prints the pair in the format "(..., ...)"

*integer* - prints an instance of the 'integer' class

*float* - prints an instance of the 'float' class, to a precision specified by the float being printed
