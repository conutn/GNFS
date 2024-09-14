# GNFS
GNFS stands for General Field Number Sieve. The paper this project is based off of is https://personal.math.vt.edu/brown/doc/briggs_gnfs_thesis.pdf

Key terms to know before reading this document (suggested)

A function is injective if it maps distinct inputs to distinct outputs, e.g. f(x)=1/(1+e^x), f(x)=sqrt(x)

A function is surjective if it maps to all outputs in its range. In other words, for any element 'y' in the function's range, there exists a element 'x' in the function's domain such that f(x)=y, e.g. f(x)=x^3-3x, f(x)=xsin(x)

A function is bijective if it is injective and surjective, a.k.a. a one-to-one relation, e.g. f(x)=x, f(x)=cbrt(x)

A group is defined as a set combined with an operation. The operation should be associative, and there should be an identity element, like 0 for addition and 1 for multiplication. Every element should also have an inverse. A group is usually denoted as (G, \*) where G is the set and '*' is the operation.

An abelian group is a group where the operation is also commutative.

A ring is a set with two operations, called addition (+) and multiplication (*). The set should satisfy these properties: let a, b, and c be any elements of the set.

(a+b)+c=a+(b+c)

a+b=b+a

there is an element '0' such that a+0=a

there exists an element -a such that a+(-a)=0

(a * b) * c = a * (b * c)

there exists an element '1' such that a * 1 = a and 1 * a = a

a * (b + c) = a * b + a * c, (b + c) * a = b * a + c * a.

A field is almost the same as a ring. However, the multiplication operation should be commutative and every element should have a multiplicative inverse (for any element a, there exists an element a^-1 such that a * a^-1 = 1).

N refers to the set of positive integers, Z to the set of integers, Q to the set of rationals, R to the set of reals, and C to the set of complex numbers.

The notation 'Z/nZ' refers to the group with the set of elements from 0 to n-1. The group has addition as its operation. For example, Z/5Z refers to the set {0, 1, 2, 3, 4} with the operation '+'.

A monic polynomial is a polynomial with a leading coefficient of 1. For example, x^3+30x^2-2x+1 is a monic polynomial but 3.1y^3+y^2-3 is not.

The notation G\[x\] where G is a group refers to the set of polynomials with 'x' as its variable and elements of G as its coefficients. For example, Z\[x\] includes x^2-x and 1234x^987+x^333-19.

The notation G(x) refers to the set of all combinations when you divide an element of G\[x\] by another element of G\[x\]. For example, R(x) includes (log(17)x^3-0.5x)/(ex^29-sqrt(pi))

G-linear means you are only adding or subtracting and not multiplying or dividing. (will make sense later)

A basis of refers to the set of elements required to span a larger set. For example, the basis of Z is {1} because any element of Z can be expressed as a linear combination of the elements of {1}.

G^n refers to the set where each element is a tuple of length 'n' where each item in the tuple is an element of 'G'.

A isomorphism refers to a bijective function 'f' mapping the group (G, +) to (H, \*) such that f(x+y)=f(x)\*f(y).

A homomorphism is an isomorphism except it doesn't need to be bijective.

A metric space refers to a set of points M and a distance function d such that 'd' satisfies these conditions.

d(x,y)=d(y,x)

d(x,y)>0 where x and y are distinct

d(x,x)=0

d(x,y)+d(y,z)>=d(x,z)

For example, the cartesian plane is a metric space (its distance function is sqrt((x0-x1)^2+(y0-y1)^2))

A homeomorphism refers to a bijective function f mapping elements from metric space M to metric space N, such that 'f' is continuous and the inverse of 'f' is also continuous
## Disclaimer: Other terms may appear in the document

# Specifics
The resource classes 'integer.h', 'inf_float.h', and 'printing_press.h' are all ready to use (if there are bugs please fix them or let someone know). However, 'GNFS.cpp' is still in development.
