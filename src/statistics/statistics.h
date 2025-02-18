
///
/// @file statistics.h Statistical functions
///

#ifndef UROBORO_STATISTICS_H
#define UROBORO_STATISTICS_H

#include "../constants.h"
#include "../algebra/vec.h"
#include "../vec_buff.h"
#include "../real_analysis.h"


namespace uroboro {


	/// Compute the arithmetic mean of a set of values
	inline real arithmetic_mean(const vec_buff& data) {

		if(!data.size()) {
			UMATH_ERROR("arithmetic_mean", data.size(), DIV_BY_ZERO);
			return nan();
		}

		// Sum of x_i / N
		return sum(data) / (real) data.size();
	}


	/// Compute the arithmetic mean of a set of values
	/// Alias for arithmetic_mean
	inline real mean(const vec_buff& data) {
		return arithmetic_mean(data);
	}


	/// Compute the harmonic mean of a set of values
	inline real harmonic_mean(const vec_buff& data) {

		if(!data.size()) {
			UMATH_ERROR("harmonic_mean", data.size(), DIV_BY_ZERO);
			return nan();
		}

		real res = 0;

		for (int i = 0; i < data.size(); ++i) {

			if(data[i] == 0) {
				UMATH_ERROR("harmonic_mean", data[i], DIV_BY_ZERO);
				return nan();
			}

			res += 1.0 / data[i];
		}

		return static_cast<real>(data.size()) / res;
	}


	/// Compute the geometric mean of a set of values
	/// as \f$\sqrt[n]{\Pi_i x_i}\f$
	inline real geometric_mean(const vec_buff& data) {
		return root(product(data), data.size());
	}


	/// Compute the weighted mean of a set of values
	/// <data> and <weights> must have the same size
	inline real weighted_mean(const vec_buff& data, const vec_buff& weights) {

		// Sum of x_i * w_i / Sum of w_i
		return product_sum(data, weights) / sum(weights);
	}


	/// Propagate error of a sum of values
	/// as sqrt(sigma_x^2 + sigma_y^2 + ...)
	inline real propagate_sum(const vec_buff& sigma) {

		return sqrt(sum_squares(sigma));
	}


	/// Propagate error of a product (or quotient) of values
	/// as sqrt((sigma_x / x_mean)^2 + (sigma_y / y_mean)^2 + ...)
	/// The result is the propagated relative error
	inline real propagate_product(const vec_buff& sigma, const vec_buff& mean) {

		if(sigma.size() != mean.size()) {
			UMATH_ERROR("propagate_product", sigma.size(), INVALID_ARGUMENT);
			return nan();
		}

		// Compute sum of squares of (i_sigma / i_mean)
		real sum = 0;
		for (int i = 0; i < sigma.size(); ++i) {
			sum += square(sigma[i] / abs(mean[i]));
		}

		return sqrt(sum);
	}


	/// Root Mean Square
	/// Computed as sqrt(sum(x_i^2) / N)
	inline real rms(const vec_buff& X) {

		if(!X.size()) {
			UMATH_ERROR("rms", X.size(), INVALID_ARGUMENT);
			return nan();
		}

		return sqrt(sum_squares(X) / X.size());
	}


	/// Total sum of squares (TSS)
	/// Computed as sum(square(x_i - x_mean))
	inline real total_sum_squares(const vec_buff& X) {

		if(!X.size()) {
			UMATH_ERROR("total_sum_squares", X.size(), INVALID_ARGUMENT);
			return nan();
		}

		real tss = 0;
		real x_m = mean(X);
		for (auto x : X) {
			tss += square(x - x_m);
		}

		return tss;
	}

	/// Total sum of squares (TSS)
	/// Computed as sum(square(x_i - x_mean))
	inline real tss(const vec_buff& X) {
		return total_sum_squares(X);
	}


	/// Compute the variance of a population
	inline real variance(const vec_buff& data) {

		if(!data.size()) {
			UMATH_ERROR("variance", data.size(), INVALID_ARGUMENT);
			return nan();
		}

		return total_sum_squares(data) / (real) data.size();
	}


	/// Compute the variance of a sample
	/// This function uses Bessel correction
	inline real sample_variance(const vec_buff& data) {

		if(!data.size()) {
			UMATH_ERROR("sample_variance", data.size(), INVALID_ARGUMENT);
			return nan();
		}

		// Bessel correction (N - 1)
		return total_sum_squares(data) / (real) (data.size() - 1);
	}


	/// Compute the standard deviation of a population
	inline real standard_deviation(const vec_buff& data) {
		return sqrt(variance(data));
	}


	/// Compute the standard deviation of a population
	inline real stdev(const vec_buff& data) {
		return standard_deviation(data);
	}


	/// Compute the standard deviation of a sample
	inline real sample_standard_deviation(const vec_buff& data) {
		return sqrt(sample_variance(data));
	}


	/// Compute the standard deviation of a sample
	inline real smpl_stdev(const vec_buff& data) {
		return sample_standard_deviation(data);
	}


	/// Compute the relative error on a population measure
	/// using standard deviation
	inline real standard_relative_error(const vec_buff& X) {

		real x_mean = mean(X);

		if(x_mean == 0) {
			UMATH_ERROR("standard_relative_error", x_mean, DIV_BY_ZERO);
			return nan();
		}

		return standard_deviation(X) / abs(x_mean);
	}


	/// Compute the relative error on a sample measure
	/// using standard deviation
	inline real sample_standard_relative_error(const vec_buff& X) {

		real x_mean = mean(X);

		if(x_mean == 0) {
			UMATH_ERROR("standard_relative_error", x_mean, DIV_BY_ZERO);
			return nan();
		}

		return sample_standard_deviation(X) / abs(x_mean);
	}


	/// Compute the standard deviation on the mean of a set of values
	inline real mean_standard_deviation(const vec_buff& data) {
		return sqrt(variance(data)) / sqrt(data.size());
	}


	/// Compute the standard deviation on the mean of a set of values
	inline real stdom(const vec_buff& data) {
		return mean_standard_deviation(data);
	}


	/// Compute the standard deviation on the mean of a set of measures
	/// Bessel correction is used in the calculation of variance
	inline real sample_mean_standard_deviation(const vec_buff& data) {
		return sqrt(sample_variance(data)) / sqrt(data.size());
	}


	/// Compute the standard deviation on the mean of a set of measures
	/// Bessel correction is used in the calculation of variance
	inline real smpl_stdom(const vec_buff& data) {
		return sample_mean_standard_deviation(data);
	}


	/// Compute the covariance of two sets of measures
	inline real covariance(const vec_buff& X, const vec_buff& Y) {

		if(X.size() != Y.size()) {
			UMATH_ERROR("covariance", X.size(), INVALID_ARGUMENT);
			return nan();
		}

		real sum = 0;
		real X_mean = mean(X);
		real Y_mean = mean(Y);

		for (int i = 0; i < X.size(); ++i)
			sum += (X[i] - X_mean) * (Y[i] - Y_mean);

		return sum / (real) X.size();
	}


	/// Compute the covariance between two sets of sample measures
	/// This function uses Bessel correction
	inline real sample_covariance(const vec_buff& X, const vec_buff& Y) {

		if(X.size() != Y.size()) {
			UMATH_ERROR("sample_covariance", X.size(), INVALID_ARGUMENT);
			return nan();
		}

		real sum = 0;
		real X_mean = mean(X);
		real Y_mean = mean(Y);

		for (int i = 0; i < X.size(); ++i)
			sum += (X[i] - X_mean) * (Y[i] - Y_mean);

		// Bessel correction (N - 1)
		return sum / (real) (X.size() - 1);
	}


	/// Pearson's correlation coefficient R for a population
	inline real correlation_coefficient(const vec_buff& X, const vec_buff& Y) {
		return covariance(X, Y) / (stdev(X) * stdev(Y));
	}


	/// Pearson's correlation coefficient r for a sample
	inline real sample_correlation_coefficient(const vec_buff& X, const vec_buff& Y) {
		return sample_covariance(X, Y) / (smpl_stdev(X) * smpl_stdev(Y));
	}


	/// Normal distribution chi-square with 4 intervals
	/// calculated on a sample of measures
	inline real chi_square_sigma(const vec_buff& X) {

		if(!X.size()) {
			UMATH_ERROR("chi_square_sigma", X.size(), INVALID_ARGUMENT);
			return nan();
		}

		const unsigned int N = X.size();

		unsigned int Ok_1 = 0;
		unsigned int Ok_2 = 0;
		unsigned int Ok_3 = 0;
		unsigned int Ok_4 = 0;

		real M = mean(X);
		real sigma = sample_standard_deviation(X);

		for (auto x : X) {
			if(x < M - sigma)
				Ok_1++;
			else if((x > M - sigma) && (x < M))
				Ok_2++;
			else if((x > M) && (x < M + sigma))
				Ok_3++;
			else if(x > M + sigma)
				Ok_4++;
		}

		// Sum of (Ok - Ek)^2 / Ek
		// where Ek = N * Pk
		real chi_2 = ((Ok_1 - (N * 0.16)) * (Ok_1 - (N * 0.16))) / (N * 0.16);
		chi_2 += ((Ok_2 - (N * 0.34)) * (Ok_2 - (N * 0.34))) / (N * 0.34);
		chi_2 += ((Ok_3 - (N * 0.34)) * (Ok_3 - (N * 0.34))) / (N * 0.34);
		chi_2 += ((Ok_4 - (N * 0.16)) * (Ok_4 - (N * 0.16))) / (N * 0.16);

		return chi_2;
	}


	/// Compute the intercept of the minimum squares linearization of X and Y
	inline real least_squares_linear_intercept(
		const vec_buff& X, const vec_buff& Y) {

		if(X.size() != Y.size()) {
			UMATH_ERROR("least_squares_linear_intercept", X.size(), INVALID_ARGUMENT);
			return nan();
		}

		real Delta = X.size() * sum_squares(X) - square(sum(X));
		real A = (sum_squares(X) * sum(Y) - sum(X) * product_sum(X, Y)) / Delta;

		return A;
	}


	/// Compute the intercept of the minimum squares linearization of X and Y
	inline real lst_sqrs_lin_intercept(
		const vec_buff& X, const vec_buff& Y) {
		return least_squares_linear_intercept(X, Y);
	}


	/// Compute the error on the intercept (A)
	inline real least_squares_linear_sigma_A(
		const vec_buff& X, const vec_buff& Y, real sigma_y) {

		real Delta = X.size() * sum_squares(X) - square(sum(X));
		return sqrt(sum_squares(X) / Delta) * abs(sigma_y);
	}


	/// Compute the slope of the minimum squares linearization of X and Y
	inline real least_squares_linear_slope(const vec_buff& X, const vec_buff& Y) {

		if(X.size() != Y.size()) {
			UMATH_ERROR("least_squares_linear_slope", X.size(), INVALID_ARGUMENT);
			return nan();
		}

		real Delta = X.size() * sum_squares(X) - square(sum(X));
		real B = (X.size() * product_sum(X, Y) - sum(X) * sum(Y)) / Delta;

		return B;
	}


	/// Compute the slope of the minimum squares linearization of X and Y
	inline real lst_sqrs_lin_slope(const vec_buff& X, const vec_buff& Y) {
		return least_squares_linear_slope(X, Y);
	}


	/// Compute the error on the slope coefficient (B)
	inline real least_squares_linear_sigma_B(
		const vec_buff& X, const vec_buff& Y, real sigma_y) {

		real Delta = X.size() * sum_squares(X) - square(sum(X));
		return sqrt(X.size() / Delta) * abs(sigma_y);
	}


	/// Compute the error of the minimum squares linearization of a sample
	inline real least_squares_linear_error(
		const vec_buff& X, const vec_buff& Y,
		real intercept, real slope) {

		if(X.size() != Y.size()) {
			UMATH_ERROR("least_squares_linear_error", X.size(), INVALID_ARGUMENT);
			return nan();
		}

		real err = 0;
		for (int i = 0; i < X.size(); ++i) {
			err += square(Y[i] - intercept - slope * X[i]);
		}

		// Correction by degrees of freedom (N - 2)
		return sqrt(err / (real) (X.size() - 2));
	}


	/// Compute the error of the minimum squares linearization of a sample
	inline real lst_sqrs_lin_error(
		const vec_buff& X, const vec_buff& Y,
		real intercept, real slope) {
		return least_squares_linear_error(X, Y, intercept, slope);
	}


	/// Compute the chi-square on a linearization
	inline real chi_square_linearization(
		const vec_buff& X, const vec_buff& Y,
		const vec_buff& sigma, real intercept, real slope) {

		if(X.size() != Y.size() || X.size() != sigma.size()) {
			UMATH_ERROR(
				"chi_square_linearization",
				X.size(), INVALID_ARGUMENT);
			return nan();
		}

		real chi_squared = 0;
		for (int i = 0; i < X.size(); ++i) {
			chi_squared += square((Y[i] - intercept - slope * X[i]) / sigma[i]);
		}

		return chi_squared;
	}


	/// Compute the reduced chi-squared on a linearization
	inline real reduced_chi_square_linearization(
		const vec_buff& X, const vec_buff& Y, const vec_buff& sigma,
		real intercept, real slope) {

		// Divide by degrees of freedom (N - 2)
		return chi_square_linearization(X, Y, sigma, intercept, slope)
			/ (real) (Y.size() - 2);
	}


	/// Compute the intercept of the weighted minimum squares linearization of X and Y
	inline real least_squares_weighted_linear_intercept(
		const vec_buff& X, const vec_buff& Y, const vec_buff& W) {

		if(X.size() != Y.size() || X.size() != W.size()) {
			UMATH_ERROR(
				"least_squares_weighted_linear_intercept",
				X.size(), INVALID_ARGUMENT);
			return nan();
		}

		real Delta = sum(W) * product_sum(X, X, W) - square(product_sum(X, W));

		real A = (product_sum(X, X, W) * product_sum(Y, W) -
			product_sum(X, W) * product_sum(X, Y, W)) / Delta;

		return A;
	}


	/// Compute the intercept of the weighted minimum squares linearization of X and Y
	inline real lst_sqrs_weight_lin_intercept(
		const vec_buff& X, const vec_buff& Y, const vec_buff& W) {
		return least_squares_weighted_linear_intercept(X, Y, W);
	}


	/// Compute the slope of the weighted minimum squares linearization of X and Y
	inline real least_squares_weighted_linear_slope(
		const vec_buff& X, const vec_buff& Y, const vec_buff& W) {

		if(X.size() != Y.size() || X.size() != W.size()) {
			UMATH_ERROR(
				"least_squares_weighted_linear_slope",
				X.size(), INVALID_ARGUMENT);
			return nan();
		}

		real Delta = sum(W) * product_sum(X, X, W) - square(product_sum(X, W));

		real B = (sum(W) * product_sum(X, Y, W) -
			product_sum(X, W) * product_sum(Y, W)) / Delta;

		return B;
	}


	/// Compute the slope of the weighted minimum squares linearization of X and Y
	inline real lst_sqrs_weight_lin_slope(
		const vec_buff& X, const vec_buff& Y, const vec_buff& W) {
		return least_squares_weighted_linear_slope(X, Y, W);
	}


}

#endif
