
/// 
/// @file vec.h Vector class and operations
/// 

#ifndef UROBORO_VECTOR_H
#define UROBORO_VECTOR_H

#ifndef UROBORO_NO_PRINT
#include <sstream>
#include <ostream>
#endif

#include <array>

#include "../error.h"
#include "../real_analysis.h"
#include "../vec_buff.h"


namespace uroboro {

	/// 
	/// @class vec
	/// An N-dimensional vector of T type elements.
	/// 
	template<unsigned int N, typename T = real>
	class vec {

		public:

		static constexpr unsigned int size = N;

		T data[N];


		vec() = default;

		/// Initialize all elements to the same value
		vec(T a) {
			for (int i = 0; i < N; ++i) {
				data[i] = a;
			}
		}

		/// Copy constructor
		vec(const vec<N, T>& other) {
			for (int i = 0; i < N; ++i) {
				data[i] = other.data[i];
			}
		}

		/// Copy from other
		vec<N, T>& operator=(const vec<N, T>& other) {
			for (int i = 0; i < N; ++i) {
				data[i] = other.data[i];
			}
			return *this;
		}

		/// Initialize from a list, e.g. {1, 2, 3}
		vec(std::initializer_list<T> l) {

			if(l.size() != N) {
				UMATH_ERROR("vec::vec(initializer_list<T>)", l.size(),
					INVALID_ARGUMENT);
				// Set all elements to NaN
				*this = vec<N, T>(nan());
				return;
			}

			std::copy(l.begin(), l.end(), &data[0]);
		}

		~vec() = default;


		// Operators

		/// Identity
		inline vec<N, T> operator+() const {
			return *this;
		}

		/// Vector sum (v + w = (v.x + w.x, ...))
		inline vec<N, T> operator+(const vec<N, T>& other) const {
			vec<N, T> result;

			for (int i = 0; i < N; ++i) {
				result.data[i] = data[i] + other.data[i];
			}

			return result;
		}

		/// Opposite vector
		inline vec<N, T> operator-() const {
			return *this * -1;
		}

		inline vec<N, T> operator-(const vec<N, T>& other) const {
			vec<N, T> result;

			for (int i = 0; i < N; ++i) {
				result.data[i] = data[i] - other.data[i];
			}

			return result;
		}

		/// Scalar multiplication (av = (v.x * a, ...))
		inline vec<N, T> operator*(T scalar) const {
			vec<N, T> result;

			for (int i = 0; i < N; ++i) {
				result.data[i] = scalar * data[i];
			}

			return result;
		}

		/// Scalar division (v / a = (v.x / a, ...))
		inline vec<N, T> operator/(T scalar) const {
			vec<N, T> result;

			for (int i = 0; i < N; ++i) {
				result.data[i] = data[i] / scalar;
			}

			return result;
		}


		/// Dot product between vectors (v * w = v.x * w.x + ...)
		inline T dot(const vec<N, T>& other) const {

			T result = 0;

			for (int i = 0; i < N; ++i) {
				result += data[i] * other.data[i];
			}

			return result;
		}

		/// Dot product between vectors (v * w = v.x * w.x + ...)
		inline T operator*(const vec<N, T>& other) const {
			return dot(other);
		}


		/// Cross product between vectors
		inline vec<3> cross(const vec<3>& other) const {

			if(N != 3) {
				UMATH_ERROR("vec::cross", N, IMPOSSIBLE_OPERATION);
				return vec<N, T>(nan());
			}

			vec<3> res;

			res.data[0] = data[1] * other.data[2] - data[2] * other.data[1];
			res.data[1] = data[2] * other.data[0] - data[0] * other.data[2];
			res.data[2] = data[0] * other.data[1] - data[1] * other.data[0];

			return res;
		}


		inline void operator+=(const vec<N, T>& other) {

			for (int i = 0; i < N; ++i)
				data[i] += other.data[i];
		}

		inline void operator-=(const vec<N, T>& other) {

			for (int i = 0; i < N; ++i)
				data[i] -= other.data[i];
		}

		inline void operator*=(T scalar) {

			for (int i = 0; i < N; ++i)
				data[i] *= scalar;
		}

		inline void operator/=(T scalar) {

			for (int i = 0; i < N; ++i)
				data[i] /= scalar;
		}


		/// Magnitude of vector (sqrt(v * v))
		inline T magnitude() const {

			T m = 0;
			for (int i = 0; i < N; ++i)
				m += data[i] * data[i];

			return sqrt(m);
		}

		/// Alias for magnitude()
		inline T lenght() {
			return magnitude();
		}

		/// Square magnitude of vector (v * v)
		inline T square_magnitude() const {
			T m = 0;
			for (int i = 0; i < N; ++i) {
				m += data[i] * data[i];
			}
			return m;
		}


		/// Alias for square_magnitude()
		inline real square_lenght() const {
			return square_magnitude();
		}

		/// Access i-th component
		inline T& operator[](int i) {
			return data[i];
		}

		/// Access i-th element
		inline T& at(int i) {
			return data[i];
		}

		/// Getters and setters
		inline T get(int i) const {
			return data[i];
		}

		/// Set the i-th element
		inline void set(int i, T x) {
			data[i] = x;
		}


		/// Vector normalization (v / |v|)
		inline void normalize() {

			real m = magnitude();

			if(m == 0) {
				UMATH_ERROR("vec::normalize", m, DIV_BY_ZERO);
				*this = vec<N, T>(nan());
			}

			for (int i = 0; i < N; ++i) {
				data[i] /= m;
			}
		}

		/// Return the normalized vector (v / |v|)
		inline vec<N, T> normalized() const {

			vec<N, T> result = vec<N, T>();
			real m = magnitude();

			if(m == 0) {
				UMATH_ERROR("vec::normalize", m, DIV_BY_ZERO);
				return vec<N, T>(nan());
			}

			for (int i = 0; i < N; ++i)
				result[i] = data[i] / m;

			return result;
		}


		/// Check whether all elements of both vectors are equal
		inline bool operator==(const vec<N, T>& other) const {
			for (int i = 0; i < N; ++i) {
				if(data[i] != other[i])
					return false;
			}

			return true;
		}

		/// Convert a vec<N, T> to a vec_buff
		inline vec_buff to_vec_buff() {
			
			vec_buff res;
			for (int i = 0; i < N; ++i)
				res.push_back(static_cast<real>(data[i]));

			return res;
		}


#ifndef UROBORO_NO_PRINT

			/// Convert the vector to string representation
			inline std::string to_string(std::string separator = ", ") const {

				std::stringstream res;

				res << "(";
				for (int i = 0; i < N; ++i) {
					res << data[i];
					if(i != N - 1)
						res << separator;
				}
				res << ")";

				return res.str();
			}


			/// Stream the vector in string representation to an output stream (std::ostream)
			friend std::ostream& operator<<(std::ostream& out, const vec<N, T>& obj) {
				return out << obj.to_string();
			}

#endif

	};

	// Common vector types

	/// A 2-dimensional vector with real elements
	using vec2 = vec<2, real>;

	/// A 3-dimensional vector with real elements
	using vec3 = vec<3, real>;

	/// A 4-dimensional vector with real elements
	using vec4 = vec<4, real>;


	/// Compute the (Euclidian) distance between two values
	real distance(real v1, real v2) {
		return abs(v1 - v2);
	}


	/// Compute the distance between two points
	template<unsigned int N, typename T>
	T distance(vec<N, T> v1, vec<N, T> v2) {
		return (v1 - v2).lenght();
	}


	/// Compute the Lp norm of a vector.
	/// \f$L_p(\vec v) = (\Sigma_i \ |v_i|^p)^{1/p}\f$
	template<unsigned int N, typename T>
	real lp_norm(vec<N, T> v, unsigned int p) {

		real sum = 0;

		for (unsigned int i = 0; i < N; ++i)
			sum += pow(abs(v[i]), p);

		return root(sum, p);
	}


	/// Compute the Linf norm of a vector.
	/// \f$L_{inf}(\vec v) = max(|v_i|)\f$
	template<unsigned int N, typename T>
	real linf_norm(vec<N, T> v) {

		real res = abs(v[0]);

		for (unsigned int i = 1; i < N; ++i)
			res = max(res, abs(v[i]));

		return res;
	}


	/// Compute the dot product of two vectors
	template<unsigned int N, typename T>
	real dot(vec<N, T> v1, vec<N, T> v2) {
		return v1.dot(v2);
	}


	/// Compute the cross product of two vectors.
	/// The vectors have to be 3-dimensional
	template<typename T>
	vec<3, T> cross(const vec<3, T>& v1, const vec<3, T>& v2) {
		return v1.cross(v2);
	}

}


#endif
