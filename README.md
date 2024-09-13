# GNFS
GNFS stands for General Field Number Sieve. The paper this project is based off of is https://personal.math.vt.edu/brown/doc/briggs_gnfs_thesis.pdf
Key terms to know before reading this document.
A function is injective if it maps distinct inputs to distinct outputs, e.g. f(x)=1/(1+e^x), f(x)=sqrt(x)
A function is surjective if it maps to all outputs in its range. In other words, for any element 'y' in the function's range, there exists a element 'x' in the function's domain such that f(x)=y, e.g. f(x)=x^3-3x, f(x)=xsin(x)
A function is bijective if it is injective and surjective, a.k.a. a one-to-one relation, e.g. f(x)=x, f(x)=cbrt(x)
A group is defined as a set combined with an operation. The operation should be associative, and there should be an identity element, like 0 for addition and 1 for multiplication. Every element should also have an inverse.
An abelian group is a group where the operation is also commutative.
A ring is a set with two operations, called addition (+) and multiplication (*). The set should satisfy these properties: let a, b, and c be any elements of the set. (a+b)+c=a+(b+c), a+b=b+a, there is an element '0' such that a+0=a, there exists an element -a such that a+(-a)=0, (a * b) * c = a * (b * c), there exists an element '1' such that a * 1 = a, a * (b + c) = a * b + a * c, (b + c) * a = b * a + c * a.
