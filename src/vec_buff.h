
///
/// @file vec_buff.h Allocation and operations on data sets
///

#ifndef UROBORO_VEC_BUFF_H
#define UROBORO_VEC_BUFF_H

#include <vector>
#include "./constants.h"


namespace uroboro {


	/// A dynamically allocated variable-size container
	/// Defined by default as an alias for `std::vector<real>`
	using vec_buff = std::vector<real>;


	/// Compute the product of a set of values
	inline real product(const vec_buff& X) {

		if(!X.size()) {
			UMATH_ERROR("product", X.size(), INVALID_ARGUMENT);
			return nan();
		}

		real res = 1;
		for(int i = 0; i < X.size(); i++)
			res *= X[i];

		return res;
	}


	/// Sum the products of two sets of values
	inline real product_sum(const vec_buff& X, const vec_buff& Y) {

		if(X.size() != Y.size()) {
			UMATH_ERROR("product_sum", X.size(), INVALID_ARGUMENT);
			return nan();
		}

		real res = 0;
		for(int i = 0; i < X.size(); i++)
			res += X[i] * Y[i];

		return res;
	}


    /// Sum the products of three sets of values
    inline real product_sum(const vec_buff& X, const vec_buff& Y, const vec_buff& Z) {

		if(X.size() != Y.size() || X.size() != Z.size()) {
			UMATH_ERROR("product_sum", X.size(), INVALID_ARGUMENT);
			return nan();
		}

		real res = 0;
		for(int i = 0; i < X.size(); i++)
			res += X[i] * Y[i] * Z[i];

		return res;
    }


    /// Sum the quotients of two sets of values
	inline real quotient_sum(const vec_buff& X, const vec_buff& Y) {

		if(X.size() != Y.size()) {
			UMATH_ERROR("quotient_sum", X.size(), INVALID_ARGUMENT);
			return nan();
		}

		real res = 0;
		for(int i = 0; i < X.size(); i++) {

			if(Y[i] == 0) {
				UMATH_ERROR("quotient_sum", Y[i], DIV_BY_ZERO);
				return nan();
			}

			res += X[i] / Y[i];
		}

		return res;
	}


    /// Sum the squares of a set of values
    inline real sum_squares(const vec_buff& X) {

		real res = 0;
		for(int i = 0; i < X.size(); i++)
			res += X[i] * X[i];

		return res;
    }


    /// Sum together a set of values
	inline real sum(const vec_buff& X) {

		real res = 0;
		for(int i = 0; i < X.size(); i++)
			res += X[i];

		return res;
	}


	/// Apply a function to a set of values
	inline void apply(vec_buff& X, real(*f)(real)) {

		for (real& x : X)
			x = f(x);
	}

}

#endif
