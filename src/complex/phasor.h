
///
/// @file phasor.h Phasor (complex number in exponential form)
///

#ifndef UROBORO_PHASOR
#define UROBORO_PHASOR

#ifndef UROBORO_NO_PRINT
#include <sstream>
#include <ostream>
#endif

#include "../real_analysis.h"
#include "./complex.h"


namespace uroboro {


	/// @class phasor
	/// Complex number in exponential form \f$\rho e^{i \theta}\f$
	class phasor {
		public:

			real phase;
			real modulus;

			/// Initialize as 0/0
			phasor() : phase(0), modulus(0) {}

			/// Initialize from two real numbers
			phasor(real modulus, real phase) {
				this->phase = phase;
				this->modulus = modulus;
			}

			/// Initialize from another phasor
			phasor(const phasor& other) {
				phase = other.phase;
				modulus = other.modulus;
			}

			/// Initialize from a complex number
			phasor(const complex& z) {
				phase = z.arg();
				modulus = z.modulus();
			}

			~phasor() = default;


			/// Add two phasors
			inline phasor operator+(const phasor& other) const {

				if(phase == other.phase)
					return phasor(modulus + other.modulus, phase);

				return phasor(to_complex() + other.to_complex());
			}


			/// Subtract two phasors
			inline phasor operator-(const phasor& other) const {

				if(phase == other.phase)
					return phasor(modulus - other.modulus, phase);

				return phasor(to_complex() - other.to_complex());
			}


			/// Multiply two phasors
			inline phasor operator*(const phasor& other) const {
				return phasor(
					modulus * other.modulus,
					phase + other.phase);
			}


			/// Divide two phasors
			inline phasor operator/(const phasor& other) const {

				if(other.modulus == 0) {
					UMATH_ERROR("phasor::operator/", other.modulus, DIV_BY_ZERO);
					return phasor(nan(), nan());
				}
					

				return phasor(
					modulus / other.modulus,
					phase - other.phase);
			}


			/// Add a phasor to this one
			inline phasor& operator+=(const phasor& other) {

				if(phase == other.phase) {
					modulus += other.modulus;
				} else {
					*this = phasor(to_complex() + other.to_complex());
				}

				return *this;
			}


			/// Subtract a phasor from this one
			inline phasor& operator-=(const phasor& other) {

				if(phase == other.phase)
					modulus -= other.modulus;
				else
					*this = phasor(to_complex() - other.to_complex());

				return *this;
			}


			/// Multiply this phasor by another one
			inline phasor& operator*=(const phasor& other) {

				modulus *= other.modulus;
				phase += other.phase;

				return *this;
			}


			/// Divide this phasor by another one
			inline phasor& operator/=(const phasor& other) {

				if(other.modulus == 0) {
					UMATH_ERROR("phasor::operator/=", other.modulus, DIV_BY_ZERO);
					modulus = nan();
					phase = nan();
					return *this;
				}

				modulus /= other.modulus;
				phase -= other.phase;

				return *this;
			}


			/// Transform a phasor to a complex number
			complex to_complex() const {
				return complex(
					modulus * cos(phase),
					modulus * sin(phase));
			}


#ifndef UROBORO_NO_PRINT

			/// Convert the phasor to string representation
			inline std::string to_string(std::string separator = ", ") const {

				std::stringstream res;
				res << modulus << "/" << phase;
				return res.str();
			}


			/// Stream the phasor in string representation
			/// to an output stream (std::ostream)
			friend std::ostream& operator<<(std::ostream& out, const phasor& obj) {
				return out << obj.to_string();
			}

#endif

	};

}

#endif
