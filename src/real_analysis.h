
///
/// @file real_analysis.h Real functions
///

#ifndef UROBORO_COMMON_H
#define UROBORO_COMMON_H

#include "./constants.h"
#include "./error.h"


namespace uroboro {


	/// Compute the square of a real number
	/// @param x A real number
	/// @return The square of x
	///
	/// Domain: [-inf, +inf]
	inline real square(real x) {
		return x * x;
	}


	/// Compute the cube of a real number
	/// @param x A real number
	/// @return The cube of x
	///
	/// Domain: [-inf, +inf]
	inline real cube(real x) {
		return x * x * x;
	}


	/// Compute the square root of a real number
	/// @param x A real number
	/// @return The square root of x
	///
	/// Domain: [0, +inf] \n
	/// The Newton-Raphson algorithm, optimized for
	/// the square root and limited by the
	/// `UROBORO_MAX_NEWTON_ITER` macro constant, is used.
	/// Domain reduction to [0, 1] is applied
	/// to ensure convergence of the algorithm.
	/// On x86 architectures, if `UROBORO_X86` is defined,
	/// the `fsqrt` instruction will be used.
	inline real sqrt(real x) {

		if(x < 0) {
			UMATH_ERROR("sqrt", x, OUT_OF_DOMAIN);
			return nan();
		}

#ifdef UROBORO_X86

	#ifdef MSVC_ASM
		__asm {
			fld x
			fsqrt
		}
	#else
		asm("fsqrt" : "+t"(x));
	#endif

		return x;
#else

		if(x < 1) {

			if(x == 0)
				return 0;

			// Approximate sqrt(x) between 0 and 1
			// The root of the inverse is the inverse of the root
			// !!! Possible precision problems with smaller numbers
			return 1.0 / sqrt(1.0 / x);
		}

		// Approximate sqrt(x) using Newton-Raphson
		real y = x;
		int i = 0;

		while((square(y) - x) > ROOT_APPROX_TOL && i < MAX_NEWTON_ITER) {
			y = (y + x / y) / 2.0;
			i++;
		}

		return y;
#endif

	}


	/// Compute the cubic root of x
	/// @param x A real number
	/// @return The cubic root of x
	///
	/// Domain: [-inf, +inf] \n
	/// The Newton-Raphson algorithm, optimized for
	/// the cubic root and limited by the
	/// `UROBORO_MAX_NEWTON_ITER` macro constant, is used.
	/// Domain reduction to [0, 1] is applied
	/// to ensure convergence of the algorithm.
	inline real cbrt(real x) {

		if(x < 1) {

			if(x == 0)
				return 0;

			// cbrt(x) is odd
			if(x < 0)
				return -cbrt(-x);

			// Approximate cbrt between 0 and 1
			// The root of the inverse is the inverse of the root
			// !!! Possible precision problems with smaller numbers
			return 1.0 / cbrt(1.0 / x);
		}

		// Approximate cbrt(x) using Newton-Raphson

		real y = x;
		int i = 0;

		while((cube(y) - x) > ROOT_APPROX_TOL && i < MAX_NEWTON_ITER) {
			y = (y * 2.0 + x / (y * y)) / 3.0;
			i++;
		}

		return y;
	}


	/// Compute the absolute value of a real number
	/// @param x A real number
	/// @return The absolute value of x
	///
	/// On x86 architectures, if `UROBORO_X86` is defined,
	/// the `fabs` instruction will be used.
	inline real abs(real x) {

#ifdef UROBORO_X86

	#ifdef MSVC_ASM
		__asm {
			fld x
			fabs
		}
	#else
		asm("fabs" : "+t"(x));
	#endif

		return x;
#else
		return x >= 0 ? x : -x;
#endif

	}


	/// Return the sign of x (1 if positive, -1 if negative, 0 if null)
	/// @param x A real number
	/// @return The sign of x
	inline int sgn(real x) {
		return (x > 0) ? 1 : (x < 0 ? -1 : 0);
	}


	/// Compute the floor of x
	/// Computes the maximum integer number that is smaller than x
	/// @param x A real number
	/// @return The floor of x
	///
	/// e.g. floor(1.6) = 1
	/// e.g. floor(-0.3) = -1
	inline int floor(real x) {

		if(x < 0 && x > -1)
			return -1;

		// Compute the biggest integer number
		// that is smaller than x
		return x - (int(x) % 1);
	}


	/// Compute the fractional part of a real number
	/// @param x A real number
	/// @return The fractional part of x
	///
	/// e.g. fract(2.5) = 0.5
	/// e.g. fract(-0.2) = 0.2
	inline real fract(real x) {
		return abs(x - floor(x));
	}


	/// Return the greatest number between two real numbers
	/// @param x A real number
	/// @param y A real number
	/// @return The greatest number between x and y
	///
	/// If `UROBORO_BRANCHLESS` is defined, a branchless
	/// implementation will be used
	inline real max(real x, real y) {
		
		#ifdef UROBORO_BRANCHLESS
			return (x + y + abs(x - y)) / 2.0;
		#else
			return x > y ? x : y;
		#endif
	}

	template<typename T>
	inline T max(T x, T y) {
		return x > y ? x : y;
	}


	/// Return the smallest number between two real numbers
	/// @param x A real number
	/// @param y A real number
	/// @return The smallest number between x and y
	///
	/// If `UROBORO_BRANCHLESS` is defined, a branchless
	/// implementation will be used
	inline real min(real x, real y) {

		#ifdef UROBORO_BRANCHLESS
			return (x + y - abs(x - y)) / 2.0;
		#else
			return x > y ? y : x;
		#endif
	}

	template<typename T>
	inline T min(T x, T y) {
		return x > y ? y : x;
	}


	/// Clamp x between a and b
	/// @param x A real number
	/// @param a A real number
	/// @param b A real number
	/// @return Returns x if x is between a and b,
	/// a if x is less than a, b if x is bigger than b
	///
	/// If `UROBORO_FORCE_BRANCHLESS` is defined,
	/// a (potentially slower) implementation will be used.
	inline real clamp(real x, real a, real b) {

#ifdef UROBORO_FORCE_BRANCHLESS

		// The branchless implementation might be slower or equal
		// on most compilers
		return min(max(x, a), b);
#else
		return x > b ? b : (x < a ? a : x);
#endif
	}


	// x86 instruction wrappers

#ifdef UROBORO_X86

	/// Compute \f$y log2(x)\f$ using x86 Assembly instructions
	inline real fyl2x(real x, real y) {

		real res;

		#ifdef MSVC_ASM

		// TO-DO

		#else
		asm ("fyl2x" : "=t"(res) : "0"(x), "u"(y) : "st(1)");
		#endif

		return res;
	}


	/// Compute \f$2^x - 1\f$ using x86 Assembly instructions
	///
	/// Domain: [-1, 1] \n
	/// May become particularly incorrect near boundaries
	inline real f2xm1(real x) {

		#ifdef MSVC_ASM
		__asm {
			fld x
			f2xm1
		}
		#else
		asm("f2xm1" : "+t"(x));
		#endif

		return x;
	}

#endif

	/// Compute the binary logarithm of a real number
	/// @param x A real number bigger than 0
	/// @return The binary logarithm of x
	///
	/// Domain: (0, +inf] \n
	/// On x86 architectures, if `UROBORO_X86` is defined,
	/// the `fyl2x` instruction will be used.
	inline real log2(real x) {

		if(x <= 0) {

			if(x == 0) {
				UMATH_ERROR("log2", x, OUT_OF_RANGE);
				return -inf();
			}

			UMATH_ERROR("log2", x, OUT_OF_DOMAIN);
			return nan();
		}

#ifdef UROBORO_X86
		return fyl2x(x, 1.0);
#else
		return nan();
#endif
	}


	/// Compute the base-10 logarithm of x
	/// @param x A real number bigger than 0
	/// @return The base-10 logarithm of x
	///
	/// Domain: (0, +inf] \n
	/// On x86 architectures, if `UROBORO_X86` is defined,
	/// the `fyl2x` instruction will be used.
	inline real log10(real x) {

		if(x <= 0) {

			if(x == 0) {
				UMATH_ERROR("log10", x, OUT_OF_RANGE);
				return -inf();
			}

			UMATH_ERROR("log10", x, OUT_OF_DOMAIN);
			return nan();
		}

#ifdef UROBORO_X86
		return fyl2x(x, 1.0 / LOG210);
#else
		return nan();
#endif
	}


	/// Compute the natural logarithm of x
	/// @param x A real number bigger than 0
	/// @return The natural logarithm of x
	///
	/// Domain: (0, +inf] \n
	/// On x86 architectures, if `UROBORO_X86` is defined,
	/// the `fyl2x` instruction will be used.
	inline real ln(real x) {

		if(x <= 0) {

			if(x == 0) {
				UMATH_ERROR("ln", x, OUT_OF_RANGE);
				return -inf();
			}

			UMATH_ERROR("ln", x, OUT_OF_DOMAIN);
			return nan();
		}

#ifdef UROBORO_X86
		return fyl2x(x, 1.0 / LOG2E);
#else
		return nan();
#endif
	}


	/// Compute the n-th power of x (where n is natural)
	/// @param x A real number
	/// @param n The integer exponent
	/// @return x to the power n
	template<typename T>
	inline T pow(T x, int n) {

		T res;
		T x2 = x * x;
		int i = 1;

		if(n > 0) {

			res = x;

			for(; i < (n / 2); i += 2)
				res *= x2;

			for(; i < n; ++i)
				res *= x;

		} else if(n < 0) {

			res = 1 / x;

			for(; i < (n / 2); i += 2)
				res /= x2;

			for(; i < -n; ++i)
				res /= x;
		} else {
			return 1.0;
		}

		return res;
	}


	/// Compute the n-th root of x
	/// @param x A real number
	/// @param n The root number
	/// @return The n-th real root of x
	///
	/// The Newton-Raphson method is used, limited by the
	/// `UROBORO_MAX_NEWTON_ITER` macro constant.
	inline real root(real x, int n) {

		if(n % 2 == 0 && x < 0 || n == 0) {
			UMATH_ERROR("root", n, OUT_OF_DOMAIN);
			return nan();
		}

		if(n < 0)
			return 1.0 / root(x, -n);

		if(x < 1) {

			if(x == 0)
				return 0;

			// Approximate root between 0 and 1
			// The root of the inverse is the inverse of the root
			// !!! Possible precision problems with smaller numbers
			return 1.0 / root(1.0 / x, n);
		}

		// Approximate n-th root using Newton-Raphson

		real y = x;
		int i = 0;

		while((pow(y, n) - x) > ROOT_APPROX_TOL && i < MAX_NEWTON_ITER) {
			y = (y * (n - 1) + x / pow(y, n - 1)) / (real) n;
			i++;
		}

		return y;
	}


	/// Compute the factorial of n
	inline long long fact(unsigned int n) {

		long long res = 1;
		for (int i = n; i > 1; --i)
			res *= i;

		return res;
	}


#ifdef UROBORO_X86

	/// Approximate \f$e^x\f$ using x86 Assembly instructions
	/// in the domain [0, 1]
	inline real exp_x86_norm(real x) {

		// e^x is calculated as 2^(x / ln2)
		return square(f2xm1(x / (2 * LN2)) + 1);
	}

#endif


	/// Compute \f$e^x\f$
	/// @param x A real number
	/// @return The exponential of x
	///
	/// The exponential is computed as \f$e^{floor(x)} e^{fract(x)}\f$,
	/// where \f$e^{floor(x)} = pow(e, floor(x))\f$ and \f$e^{fract(x)}\f$
	/// is either approximated using Taylor series on [0, 0.5] or,
	/// if `UROBORO_X86` is defined, using the `f2xm1` x86 assembly instruction.
	inline real exp(real x) {

	// Domain reduction to [0, +inf]
	if(x < 0)
		return 1.0 / exp(abs(x));

	real fract_x = fract(x);

#ifdef UROBORO_X86

		return pow(E, floor(x)) * exp_x86_norm(fract_x);

#else

	// Taylor series expansion
	// Compute e^floor(x) * e^fract(x)
	
	real res = 1;
	real s_n = 1;

	for (int i = 1; i < 9; ++i) {

		// Recurrence formula to improve
		// numerical stability and performance
		s_n *= (fract_x / 2.0) / static_cast<real>(i);
		res += s_n;
	}

	return pow(E, floor(x)) * res * res;

#endif
	}


	/// Approximate x elevated to a
	/// @param x A real number
	/// @return a A real exponent
	///
	/// Approximated as \f$e^{a ln(|x|) sgn(x)}\f$
	inline real powf(real x, real a) {

		if(a < 0)
			return 1.0 / exp(-a * ln(abs(x)) * sgn(x));

		// x^a = e^(a * ln(x))
		return exp(a * ln(abs(x)) * sgn(x));
	}


	/// Compute the sine of a real number
	/// @param x An angle in **radians**
	/// @return The sine of x
	///
	/// On x86 architectures, if `UROBORO_X86` is defined,
	/// the `fsin` instruction will be used.
	inline real sin(real x) {

#ifdef UROBORO_X86

	#ifdef MSVC_ASM
		__asm {
			fld x
			fsin
		}
	#else
		asm("fsin" : "+t"(x));
	#endif

		return x;

#else

		// Clamp x between -2PI and 2PI
		while(x >= 2 * PI)
			x -= 2 * PI;
		
		while(x <= -2 * PI)
			x += 2 * PI;

		// Domain reduction to [-PI, PI]
		if(x > PI) {
			x = PI - x;
		} else if(x < -PI) {
			x = -PI - x;
		}

		real res = 0;

		// Taylor series expansion
		// sin(x) = sum( (-1)^i * x^(2i+1) / (2i+1)! )

		for (int i = 0; i < TAYLOR_ORDER; ++i) {
			res += (i % 2 == 0 ? 1 : -1)
				* pow(x, 2 * i + 1)
				/ static_cast<real>(fact(2 * i + 1));
		}

		return res;
#endif
	}


	/// Compute the cosine of a real number
	/// @param x An angle in **radians**
	/// @return The cosine of x
	///
	/// On x86 architectures, if `UROBORO_X86` is defined,
	/// the `fcos` instruction will be used.
	inline real cos(real x) {

#ifdef UROBORO_X86

		#ifdef MSVC_ASM
		__asm {
			fld x
			fcos
		}
		#else
		asm("fcos" : "+t"(x));
		#endif

		return x;

#else

		// Clamp x between -2PI and 2PI
		while(x >= 2 * PI)
			x -= 2 * PI;
		
		while(x <= -2 * PI)
			x += 2 * PI;

		real res = 0;

		// Taylor series expansion
		// sin(x) = sum( (-1)^i * x^(2i) / (2i)! )

		for (int i = 0; i < TAYLOR_ORDER; ++i) {
			res += (i % 2 == 0 ? 1 : -1)
				* pow(x, 2 * i)
				/ static_cast<real>(fact(2 * i));
		}

		return res;
#endif
	}


	/// Compute the tangent of x
	/// @param x An angle in **radians**
	/// @return The tangent of x
	///
	/// On x86 architectures, if `UROBORO_X86` is defined,
	/// the `fsincos` instruction will be used.
	inline real tan(real x) {

		real s, c;

#ifdef UROBORO_X86

		#ifdef MSVC_ASM

		// TO-DO

		#else
		asm ("fsincos" : "=t"(c), "=u"(s) : "0"(x));
		#endif

#else
		s = sin(x);
		c = cos(x);
#endif

		if(c == 0) {
			UMATH_ERROR("tan", c, DIV_BY_ZERO);
			return nan();
		}

		return s / c;
	}


	/// Compute the cotangent of x
	/// @param x An angle in **radians**
	/// @return The cotangent of x
	///
	/// On x86 architectures, if `UROBORO_X86` is defined,
	/// the `fsincos` instruction will be used.
	inline real cot(real x) {

		real s, c;

#ifdef UROBORO_X86

		#ifdef MSVC_ASM

		// TO-DO

		#else
		asm ("fsincos" : "=t"(c), "=u"(s) : "0"(x));
		#endif

#else
		s = sin(x);
		c = cos(x);
#endif

		if(s == 0) {
			UMATH_ERROR("cot", s, DIV_BY_ZERO);
			return nan();
		}

		return c / s;
	}


	/// Compute the arctangent
	/// @param x An angle in **radians**
	/// @return The arctangent of x
	///
	/// A degree 9 interpolating polynomial through
	/// Chebyshev nodes is used to approximate \f$atan(x)\f$.
	/// Domain reduction to [-1, 1] is performed.
	inline real atan(real x) {

		// Domain reduction to [-1, 1]
		if(abs(x) > 1.0)
			return (PI2 - atan(1.0 / abs(x))) * sgn(x);

		real x2 = x * x;

		// Interpolating Chebyshev polynomial
		// of order 9
		return x * (0.999965
			+ x2 * (-0.331545
				+ x2 * (0.184464
					+ x2 * (-0.090752
						+ 0.023286 * x2))));
	}


	/// Compute the arcsine
	/// @param x A real number
	/// @return The arcsine of x
	///
	/// Domain: [-1, 1].
	/// The identity \f$asin(x) = atan(\frac{x}{\sqrt{1 - x^2}})\f$ is used.
	inline real asin(real x) {

		if(abs(x) > 1) {
			UMATH_ERROR("asin", x, OUT_OF_DOMAIN);
			return nan();
		}

		return atan(x / sqrt(1 - x * x));
	}


	/// Compute the arccosine
	/// @param x A real number
	/// @return The arccosine of x
	///
	/// Domain: [-1, 1].
	/// The identities \f$asin(x) = atan(\frac{sqrt{1 - x^2}{x}})\f$ and
	/// \f$asin(x) = atan(\frac{\sqrt{1 - x^2}{x}}) + \pi\f$ are used.
	inline real acos(real x) {

		if(abs(x) > 1) {
			UMATH_ERROR("acos", x, OUT_OF_DOMAIN);
			return nan();
		}

		if(x < 0)
			return atan(sqrt(1 - x * x) / x) + PI;
		else
			return atan(sqrt(1 - x * x) / x);
	}


	/// Compute the 2 argument arctangent
	/// @param y The y coordinate in cartesian space
	/// @param x The x coordinate in cartesian space
	/// @return The counterclockwise angle between the vector described by x and y
	/// and the x axis.
	///
	/// Computed using identities on atan(x).
	inline real atan2(real y, real x) {

		if(x == 0) {

			if(y == 0) {
				UMATH_ERROR("atan2", y, OUT_OF_DOMAIN);
				return nan();
			}

			return sgn(y) * PI2;
		}

		if(x > 0) {
			return sgn(y) * atan(y / x);
		} else {
			return sgn(y) * atan(y / -x) + PI2;
		}
	}


	/// Compute the hyperbolic sine
	/// @param x A real number
	/// @return The hyperbolic sine of x
	///
	/// \f$sinh = \frac{e^x - e^{-x}}{2}\f$
	inline real sinh(real x) {
		real exp_x = exp(x);
		return (exp_x - 1.0 / exp_x) / 2.0;
	}


	/// Compute the hyperbolic cosine
	/// @param x A real number
	/// @return The hyperbolic cosine of x
	///
	/// \f$cosh = \frac{e^x + e^{-x}}{2}\f$
	inline real cosh(real x) {
		real exp_x = exp(x);
		return (exp_x + 1.0 / exp_x) / 2.0;
	}


	/// Compute the hyperbolic tangent
	/// @param x A real number
	/// @return The hyperbolic tangent of x
	inline real tanh(real x) {
		real exp_x = exp(x);
		return (exp_x - 1.0 / exp_x) / (exp_x + 1.0 / exp_x);
	}


	/// Compute the hyperbolic cotangent
	/// @param x A real number
	/// @return The hyperbolic cotangent of x
	inline real coth(real x) {
		real exp_x = exp(x);
		return (exp_x + 1.0 / exp_x) / (exp_x - 1.0 / exp_x);
	}


	/// Compute the binomial coefficient
	/// @param n A natural number
	/// @param m A natural number smaller than n
	/// @return The binomial coefficient computed on (n, m)
	/// as \f$\frac{n!}{m!(n - m)!}\f$
	inline long long binomial_coeff(unsigned int n, unsigned int m) {

		if(n < m) {
			UMATH_ERROR("binomial_coeff", n, IMPOSSIBLE_OPERATION);
			return 0;
		}

		// TO-DO Check out of range

		long long res = 1;

		for (int i = n; i > m; --i)
			res *= i;

		return res / fact(n - m);
	}


	/// Convert degrees to radians
	/// @param degrees An angle in degrees
	/// @return The converted angle in radians
	///
	/// The `DEG2RAD` scalar factor is used.
	inline real radians(real degrees) {
		return degrees * DEG2RAD;
	}


	/// Convert radians to degrees
	/// @param radians An angle in radians
	/// @return The converted angle in degrees
	///
	/// The `RAD2DEG` scalar factor is used.
	inline real degrees(real radians) {
		return radians * RAD2DEG;
	}


	/// Kronecker delta, equals 1 if i is equal to j, 0 otherwise
	/// @param i The first value to compare
	/// @param j The second value to compare
	/// @param tol Maximum difference to tolerate (defaults to 0)
	/// @return 1 if i is equal to j, 0 otherwise
	template<typename T>
	inline T kronecker_delta(T i, T j, T tol = 0) {
		return (i - j < tol) ? 1 : 0;
	}

}

#endif
