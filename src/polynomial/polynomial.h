
///
/// @file polynomial.h Polynomial storage and manipulation
///

#ifndef UROBORO_POLYNOMIAL_H
#define UROBORO_POLYNOMIAL_H

#ifndef UROBORO_NO_PRINT
#include <sstream>
#include <ostream>
#endif

#include "../vec_buff.h"
#include "../real_analysis.h"
#include "../algebra/vec.h"
#include "../complex/complex.h"
#include "../complex/complex_analysis.h"


namespace uroboro {

	/// @class polynomial
	/// A polynomial of arbitrary order
	template<typename T = real>
	class polynomial {
		public:
			std::vector<T> coeff;

			polynomial() : coeff() {}

			polynomial(const std::vector<T>& c) : coeff(c) {}
			
			~polynomial() {}

			polynomial(std::initializer_list<T> l) : coeff(l) {}

			// Access i-th coefficient
			inline T& at(int i) {
				return coeff[i];
			}


			// Access i-th coefficient
			inline T& operator[](int i) {
				return at(i);
			}


			// Get the i-th by value
			inline T get(int i) const {
				return coeff[i];
			}


			// Evaluate the polynomial using x as variable
			inline T eval(T x) const {

				if(!coeff.size())
					return 0;

				T sum = 0;

				// Evaluate using Horner's method
				for (int i = 0; i < coeff.size(); ++i)
					sum = coeff[coeff.size() - i - 1] + x * sum;

				// TO-DO Compare performance using fma x86 Assembly instruction
				// sum = fma(x, sum, coeff[coeff.size() - i - 1]);

				return sum;
			}


			// Evaluate the polynomial using x as variable
			inline T operator()(T x) const {
				return eval(x);
			}


			// Return the nth order coefficient
			inline T& operator[](unsigned int i) {
				return coeff[i];
			}


			// Find the true order of the polynomial (ignoring null coefficients)
			inline int find_order() const {

				for (int i = coeff.size() - 1; i >= 0; --i) {
					if(coeff[i] != 0)
						return i;
				}

				return 0;
			}


			// Remove higher order null coefficients
			inline void trim() {

				for (int i = coeff.size() - 1; i >= 0; --i) {
					if(coeff[i] != 0)
						break;

					coeff.pop_back();
				}
			}


			// Get the number of coefficients
			inline size_t size() const {
				return coeff.size();
			}


			// Sum two polynomials
			inline polynomial operator+(const polynomial& p) const {

				polynomial r(coeff);

				for (int i = 0; i < uroboro::min(r.size(), p.size()); ++i) {
					r[i] += p.get(i);
				}

				return r;
			}


			// Subtract a polynomial from another
			inline polynomial operator-(const polynomial& p) const {
				
				polynomial r(coeff);

				for (int i = 0; i < uroboro::min(r.size(), p.size()); ++i) {
					r[i] -= p.get(i);
				}

				return r;
			}


			// Multiply two polynomials
			inline polynomial operator*(const polynomial& p) const {

				polynomial r = polynomial();
				r.coeff.resize(this->size() + p.size() - 1);

				for (int i = 0; i < size(); ++i) {
					for (int j = 0; j < p.size(); ++j) {
						r[i + j] += coeff[i] * p.get(j);
					}
				}

				return r;
			}


			// TO-DO Polynomial division


			// Sum a polynomial to this one
			inline polynomial& operator+=(const polynomial& p) {

				// Make room for the new coefficients
				if(coeff.size() < p.size())
					coeff.resize(p.size(), T(0));

				for (int i = 0; i < uroboro::min(size(), p.size()); ++i)
					coeff[i] += p.get(i);

				return *this;
			}


			// Subtract a polynomial from this one
			inline polynomial& operator-=(const polynomial& p) {

				// Make room for the new coefficients
				if(coeff.size() < p.size())
					coeff.resize(p.size(), T(0));

				for (int i = 0; i < uroboro::min(size(), p.size()); ++i)
					coeff[i] -= p.get(i);

				return *this;
			}


			// Multiply two polynomials
			inline polynomial& operator*=(const polynomial& p) {

				polynomial r = polynomial();
				r.coeff.resize(this->size() + p.size() - 1);

				for (int i = 0; i < size(); ++i) {
					for (int j = 0; j < p.size(); ++j) {
						r[i + j] += coeff[i] * p.get(j);
					}
				}

				*this = r;
				return *this;
			}


			// Multiply a polynomial by a scalar value
			inline polynomial& operator*=(T a) {

				for (int i = 0; i < coeff.size(); ++i)
					coeff[i] *= a;
				
				return *this;
			}


			// Divide a polynomial by a scalar value
			inline polynomial& operator/=(T a) {

				if(a == 0) {
					UMATH_ERROR("polynomial::operator/=", a, DIV_BY_ZERO);
					return *this;
				}

				for (int i = 0; i < coeff.size(); ++i)
					coeff[i] /= a;
				
				return *this;
			}


			// Check whether two polynomials are equal
			inline bool operator==(const polynomial& other) const {

				for (int i = 0; i < min(other.size(), this->size()); ++i) {
					if(other.coeff[i] != coeff[i])
						return false;
				}

				if(size() > other.size()) {
					for (int i = 0; i < size(); ++i) {
						if(coeff[i] != 0)
							return false;
					}
				} else {
					for (int i = 0; i < other.size(); ++i) {
						if(other.coeff[i] != 0)
							return false;
					}
				}

				return true;
			}


			// Compute the roots of a quadratic polynomial
			inline vec<2, complex> quadratic_roots() const {

				int order = find_order();

				// Check that the polynomial is quadratic
				if(order != 2) {
					UMATH_ERROR("quadratic_roots", order, IMPOSSIBLE_OPERATION);
					return vec<2, complex>({nan(), nan()});
				}

				// Complex square root
				complex d = sqrt(complex(coeff[1] * coeff[1] - coeff[2] * coeff[0] * 4));

				// Compute the two roots of the polynomial
				// using the quadratic formula
				return {
					(d * -1	+ coeff[1] * -1) / (coeff[2] * 2),
					(d		+ coeff[1] * -1) / (coeff[2] * 2)
				};
			}


			// Construct a polynomial from its roots
			inline static polynomial<T> from_roots(std::vector<T> roots) {

				polynomial<T> P = {1};

				// P = product((x - r_i))
				for (int i = 0; i < roots.size(); ++i)
					P *= polynomial<T>({roots[i] * -1, 1});

				return P;
			}


#ifndef UROBORO_NO_PRINT

			// Convert the polynomial to string representation
			inline std::string to_string(const std::string& unknown = "x") const {

				std::stringstream res;

				for (int i = 0; i < coeff.size(); ++i) {
					if(i) {
						res << (coeff[i] >= 0 ? " + " : " - ")
							 << abs(coeff[i]) << unknown << "^" << i;
					} else {
						res << coeff[i];
					}	
				}

				return res.str();
			}


			// Stream the polynomial in string representation
			// to an output stream (std::ostream)
			friend std::ostream& operator<<(std::ostream& out, const polynomial& obj) {
				return out << obj.to_string();
			}

#endif

		
	};
	
}

#endif
