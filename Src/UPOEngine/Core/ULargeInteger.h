#pragma once

#include "UBasic.h"


namespace UPO
{
	class uint128_t
	{
	private:
		uint64_t UPPER, LOWER;

	public:
		uint128_t();
		uint128_t(const uint128_t & rhs);
		uint128_t(const uint128_t && rhs);

		template <typename T> uint128_t(const T & rhs)
			: UPPER(0), LOWER(rhs)
		{}

		template <typename S, typename T> uint128_t(const S & upper_rhs, const T & lower_rhs)
			: UPPER(upper_rhs), LOWER(lower_rhs)
		{}

		uint128_t operator=(const uint128_t & rhs);
		uint128_t operator=(const uint128_t && rhs);

		template <typename T> uint128_t operator=(const T & rhs) {
			UPPER = 0;
			LOWER = rhs;
			return *this;
		}

		// Typecast Operators
		operator bool() const;
		operator char() const;
		operator int() const;
		operator uint8_t() const;
		operator uint16_t() const;
		operator uint32_t() const;
		operator uint64_t() const;

		// Bitwise Operators
		uint128_t operator&(const uint128_t & rhs) const;
		uint128_t operator|(const uint128_t & rhs) const;
		uint128_t operator^(const uint128_t & rhs) const;
		uint128_t operator&=(const uint128_t & rhs);
		uint128_t operator|=(const uint128_t & rhs);
		uint128_t operator^=(const uint128_t & rhs);
		uint128_t operator~() const;

		template <typename T> uint128_t operator&(const T & rhs) const {
			return uint128_t(0, LOWER & (uint64_t)rhs);
		}

		template <typename T> uint128_t operator|(const T & rhs) const {
			return uint128_t(UPPER, LOWER | (uint64_t)rhs);
		}

		template <typename T> uint128_t operator^(const T & rhs) const {
			return uint128_t(UPPER, LOWER ^ (uint64_t)rhs);
		}

		template <typename T> uint128_t operator&=(const T & rhs) {
			UPPER = 0;
			LOWER &= rhs;
			return *this;
		}

		template <typename T> uint128_t operator|=(const T & rhs) {
			LOWER |= (uint64_t)rhs;
			return *this;
		}

		template <typename T> uint128_t operator^=(const T & rhs) {
			LOWER ^= (uint64_t)rhs;
			return *this;
		}

		// Bit Shift Operators
		uint128_t operator<<(const uint128_t & rhs) const;
		uint128_t operator >> (const uint128_t & rhs) const;
		uint128_t operator<<=(const uint128_t & rhs);
		uint128_t operator>>=(const uint128_t & rhs);

		template <typename T>uint128_t operator<<(const T & rhs) const {
			return *this << uint128_t(rhs);
		}

		template <typename T>uint128_t operator >> (const T & rhs) const {
			return *this >> uint128_t(rhs);
		}

		template <typename T>uint128_t operator<<=(const T & rhs) {
			*this = *this << uint128_t(rhs);
			return *this;
		}

		template <typename T>uint128_t operator>>=(const T & rhs) {
			*this = *this >> uint128_t(rhs);
			return *this;
		}

		// Logical Operators
		bool operator!() const;
		bool operator&&(const uint128_t & rhs) const;
		bool operator||(const uint128_t & rhs) const;

		template <typename T> bool operator&&(const T & rhs) {
			return ((bool) *this && rhs);
		}

		template <typename T> bool operator||(const T & rhs) {
			return ((bool) *this || rhs);
		}

		// Comparison Operators
		bool operator==(const uint128_t & rhs) const;
		bool operator!=(const uint128_t & rhs) const;
		bool operator>(const uint128_t & rhs) const;
		bool operator<(const uint128_t & rhs) const;
		bool operator>=(const uint128_t & rhs) const;
		bool operator<=(const uint128_t & rhs) const;

		template <typename T> bool operator==(const T & rhs) const {
			return (!UPPER && (LOWER == (uint64_t)rhs));
		}

		template <typename T> bool operator!=(const T & rhs) const {
			return (UPPER | (LOWER != (uint64_t)rhs));
		}

		template <typename T> bool operator>(const T & rhs) const {
			return (UPPER || (LOWER > (uint64_t)rhs));
		}

		template <typename T> bool operator<(const T & rhs) const {
			return (!UPPER) ? (LOWER < (uint64_t)rhs) : false;
		}

		template <typename T> bool operator>=(const T & rhs) const {
			return ((*this > rhs) | (*this == rhs));
		}

		template <typename T> bool operator<=(const T & rhs) const {
			return ((*this < rhs) | (*this == rhs));
		}

		// Arithmetic Operators
		uint128_t operator+(const uint128_t & rhs) const;
		uint128_t operator+=(const uint128_t & rhs);
		uint128_t operator-(const uint128_t & rhs) const;
		uint128_t operator-=(const uint128_t & rhs);
		uint128_t operator*(const uint128_t & rhs) const;
		uint128_t operator*=(const uint128_t & rhs);

	private:
		std::pair <uint128_t, uint128_t> divmod(const uint128_t & lhs, const uint128_t & rhs) const;

	public:
		uint128_t operator/(const uint128_t & rhs) const;
		uint128_t operator/=(const uint128_t & rhs);
		uint128_t operator%(const uint128_t & rhs) const;
		uint128_t operator%=(const uint128_t & rhs);

		template <typename T> uint128_t operator+(const T & rhs) const {
			return uint128_t(UPPER + ((LOWER + (uint64_t)rhs) < LOWER), LOWER + (uint64_t)rhs);
		}

		template <typename T> uint128_t operator+=(const T & rhs) {
			UPPER = UPPER + ((LOWER + rhs) < LOWER);
			LOWER = LOWER + rhs;
			return *this;
		}

		template <typename T> uint128_t operator-(const T & rhs) const {
			return uint128_t((uint64_t)(UPPER - ((LOWER - rhs) > LOWER)), (uint64_t)(LOWER - rhs));
		}

		template <typename T> uint128_t operator-=(const T & rhs) {
			*this = *this - rhs;
			return *this;
		}

		template <typename T> uint128_t operator*(const T & rhs) const {
			return (*this) * (uint128_t(rhs));
		}

		template <typename T> uint128_t operator*=(const T & rhs) {
			*this = *this * uint128_t(rhs);
			return *this;
		}

		template <typename T> uint128_t operator/(const T & rhs) const {
			return *this / uint128_t(rhs);
		}

		template <typename T> uint128_t operator/=(const T & rhs) {
			*this = *this / uint128_t(rhs);
			return *this;
		}

		template <typename T> uint128_t operator%(const T & rhs) const {
			return *this - (rhs * (*this / rhs));
		}

		template <typename T> uint128_t operator%=(const T & rhs) {
			*this = *this % uint128_t(rhs);
			return *this;
		}

		// Increment Operator
		uint128_t operator++();
		uint128_t operator++(int);

		// Decrement Operator
		uint128_t operator--();
		uint128_t operator--(int);

		// Get private values
		const uint64_t & upper() const;
		const uint64_t & lower() const;

		// Get bitsize of value
		uint8_t bits() const;

		// Get string representation of value
		std::string str(uint8_t base = 10, const unsigned int & len = 0) const;
	};

	// Useful values
	extern UAPI const uint128_t uint128_0;
	extern UAPI const uint128_t uint128_1;
	extern UAPI const uint128_t uint128_64;
	extern UAPI const uint128_t uint128_128;

	// lhs type T as first arguemnt
	// If the output is not a bool, casts to type T

	// Bitwise Operators
	template <typename T> T operator&(const T & lhs, const uint128_t & rhs) {
		return (T)(lhs & (T)rhs.lower());
	}

	template <typename T> T operator|(const T & lhs, const uint128_t & rhs) {
		return (T)(lhs | (T)rhs.lower());
	}

	template <typename T> T operator^(const T & lhs, const uint128_t & rhs) {
		return (T)(lhs ^ (T)rhs.lower());
	}

	template <typename T> T operator&=(T & lhs, const uint128_t & rhs) {
		lhs &= (T)rhs.lower(); return lhs;
	}

	template <typename T> T operator|=(T & lhs, const uint128_t & rhs) {
		lhs |= (T)rhs.lower(); return lhs;
	}

	template <typename T> T operator^=(T & lhs, const uint128_t & rhs) {
		lhs ^= (T)rhs.lower(); return lhs;
	}

	// Comparison Operators
	template <typename T> bool operator==(const T & lhs, const uint128_t & rhs) {
		return (!rhs.upper() && ((uint64_t)lhs == rhs.lower()));
	}

	template <typename T> bool operator!=(const T & lhs, const uint128_t & rhs) {
		return (rhs.upper() | ((uint64_t)lhs != rhs.lower()));
	}

	template <typename T> bool operator>(const T & lhs, const uint128_t & rhs) {
		return (!rhs.upper()) && ((uint64_t)lhs > rhs.lower());
	}

	template <typename T> bool operator<(const T & lhs, const uint128_t & rhs) {
		if (rhs.upper()) {
			return true;
		}
		return ((uint64_t)lhs < rhs.lower());
	}

	template <typename T> bool operator>=(const T & lhs, const uint128_t & rhs) {
		if (rhs.upper()) {
			return false;
		}
		return ((uint64_t)lhs >= rhs.lower());
	}

	template <typename T> bool operator<=(const T & lhs, const uint128_t & rhs) {
		if (rhs.upper()) {
			return true;
		}
		return ((uint64_t)lhs <= rhs.lower());
	}

	// Arithmetic Operators
	template <typename T> T operator+(const T & lhs, const uint128_t & rhs) {
		return (T)(rhs + lhs);
	}

	template <typename T> T & operator+=(T & lhs, const uint128_t & rhs) {
		lhs = (T)(rhs + lhs);
		return lhs;
	}

	template <typename T> T operator-(const T & lhs, const uint128_t & rhs) {
		return (T)(uint128_t(lhs) - rhs);
	}

	template <typename T> T & operator-=(T & lhs, const uint128_t & rhs) {
		lhs = (T)(uint128_t(lhs) - rhs);
		return lhs;
	}

	template <typename T> T operator*(const T & lhs, const uint128_t & rhs) {
		return lhs * (T)rhs.lower();
	}

	template <typename T> T & operator*=(T & lhs, const uint128_t & rhs) {
		lhs *= (T)rhs.lower();
		return lhs;
	}

	template <typename T> T operator/(const T & lhs, const uint128_t & rhs) {
		return (T)(uint128_t(lhs) / rhs);
	}

	template <typename T> T & operator/=(T & lhs, const uint128_t & rhs) {
		lhs = (T)(uint128_t(lhs) / rhs);
		return lhs;
	}

	template <typename T> T operator%(const T & lhs, const uint128_t & rhs) {
		return (T)(uint128_t(lhs) % rhs);
	}

	template <typename T> T & operator%=(T & lhs, const uint128_t & rhs) {
		lhs = (T)(uint128_t(lhs) % rhs);
		return lhs;
	}

	// IO Operator
	std::ostream & operator<<(std::ostream & stream, const uint128_t & rhs);

















	class uint256_t {
	private:
		uint128_t UPPER, LOWER;

	public:
		// Constructors
		uint256_t();
		uint256_t(const uint256_t & rhs);
		uint256_t(const uint256_t && rhs);

		template <typename T> uint256_t(const T & rhs)
			: UPPER(uint128_0), LOWER(rhs)
		{}

		template <typename S, typename T> uint256_t(const S & upper_rhs, const T & lower_rhs)
			: UPPER(upper_rhs), LOWER(lower_rhs)
		{}

		//  RHS input args only

		// Assignment Operator
		uint256_t operator=(const uint256_t & rhs);
		uint256_t operator=(const uint256_t && rhs);

		template <typename T> uint256_t operator=(const T & rhs) {
			UPPER = uint128_0;
			LOWER = rhs;
			return *this;
		}

		// Typecast Operators
		operator bool() const;
		operator char() const;
		operator uint8_t () const;
		operator uint16_t () const;
		operator uint32_t () const;
		operator uint64_t () const;
		operator uint128_t () const;

		// Bitwise Operators
		uint256_t operator&(const uint256_t & rhs) const;
		uint256_t operator|(const uint256_t & rhs) const;
		uint256_t operator^(const uint256_t & rhs) const;
		uint256_t operator&=(const uint256_t & rhs);
		uint256_t operator|=(const uint256_t & rhs);
		uint256_t operator^=(const uint256_t & rhs);
		uint256_t operator~() const;

		template <typename T> uint256_t operator&(const T & rhs) const {
			return uint256_t(0, LOWER & (uint128_t)rhs);
		}

		template <typename T> uint256_t operator|(const T & rhs) const {
			return uint256_t(UPPER, LOWER | uint128_t(rhs));
		}

		template <typename T> uint256_t operator^(const T & rhs) const {
			return uint256_t(UPPER, LOWER ^ (uint128_t)rhs);
		}

		template <typename T> uint256_t operator&=(const T & rhs) {
			UPPER = uint128_0;
			LOWER &= rhs;
			return *this;
		}

		template <typename T> uint256_t operator|=(const T & rhs) {
			LOWER |= (uint128_t)rhs;
			return *this;
		}

		template <typename T> uint256_t operator^=(const T & rhs) {
			LOWER ^= (uint128_t)rhs;
			return *this;
		}

		// Bit Shift Operators
		uint256_t operator<<(const uint256_t & shift) const;
		uint256_t operator >> (const uint256_t & shift) const;
		uint256_t operator<<=(const uint256_t & shift);
		uint256_t operator>>=(const uint256_t & shift);

		template <typename T>uint256_t operator<<(const T & rhs) const {
			return *this << uint256_t(rhs);
		}

		template <typename T>uint256_t operator >> (const T & rhs) const {
			return *this >> uint256_t(rhs);
		}

		template <typename T>uint256_t operator<<=(const T & rhs) {
			*this = *this << uint256_t(rhs);
			return *this;
		}

		template <typename T>uint256_t operator>>=(const T & rhs) {
			*this = *this >> uint256_t(rhs);
			return *this;
		}

		// Logical Operators
		bool operator!() const;
		bool operator&&(const uint256_t & rhs) const;
		bool operator||(const uint256_t & rhs) const;

		template <typename T> bool operator&&(const T & rhs) const {
			return ((bool) *this && rhs);
		}

		template <typename T> bool operator||(const T & rhs) const {
			return ((bool) *this || rhs);
		}

		// Comparison Operators
		bool operator==(const uint256_t & rhs) const;
		bool operator!=(const uint256_t & rhs) const;
		bool operator>(const uint256_t & rhs) const;
		bool operator<(const uint256_t & rhs) const;
		bool operator>=(const uint256_t & rhs) const;
		bool operator<=(const uint256_t & rhs) const;

		template <typename T> bool operator==(const T & rhs) const {
			return (!UPPER && (LOWER == uint128_t(rhs)));
		}

		template <typename T> bool operator!=(const T & rhs) const {
			return ((bool)UPPER | (LOWER != uint128_t(rhs)));
		}

		template <typename T> bool operator>(const T & rhs) const {
			return ((bool)UPPER | (LOWER > uint128_t(rhs)));
		}

		template <typename T> bool operator<(const T & rhs) const {
			return (!UPPER) ? (LOWER < uint128_t(rhs)) : false;
		}

		template <typename T> bool operator>=(const T & rhs) const {
			return ((*this > rhs) | (*this == rhs));
		}

		template <typename T> bool operator<=(const T & rhs) const {
			return ((*this < rhs) | (*this == rhs));
		}

		// Arithmetic Operators
		uint256_t operator+(const uint256_t & rhs) const;
		uint256_t operator+=(const uint256_t & rhs);
		uint256_t operator-(const uint256_t & rhs) const;
		uint256_t operator-=(const uint256_t & rhs);
		uint256_t operator*(const uint256_t & rhs) const;
		uint256_t operator*=(const uint256_t & rhs);

	private:
		std::pair <uint256_t, uint256_t> divmod(const uint256_t & lhs, const uint256_t & rhs) const;

	public:
		uint256_t operator/(const uint256_t & rhs) const;
		uint256_t operator/=(const uint256_t & rhs);
		uint256_t operator%(const uint256_t & rhs) const;
		uint256_t operator%=(const uint256_t & rhs);

		template <typename T> uint256_t operator+(const T & rhs) const {
			return uint256_t(UPPER + ((LOWER + (uint128_t)rhs) < LOWER), LOWER + (uint128_t)rhs);
		}

		template <typename T> uint256_t operator+=(const T & rhs) {
			UPPER = UPPER + ((LOWER + rhs) < LOWER);
			LOWER = LOWER + rhs;
			return *this;
		}

		template <typename T> uint256_t operator-(const T & rhs) const {
			return uint256_t(UPPER - ((LOWER - rhs) > LOWER), LOWER - rhs);
		}

		template <typename T> uint256_t operator-=(const T & rhs) {
			*this = *this - rhs;
			return *this;
		}

		template <typename T> uint256_t operator*(const T & rhs) const {
			return *this * uint256_t(rhs);
		}

		template <typename T> uint256_t operator*=(const T & rhs) {
			*this = *this * uint256_t(rhs);
			return *this;
		}

		template <typename T> uint256_t operator/(const T & rhs) const {
			return *this / uint256_t(rhs);
		}

		template <typename T> uint256_t operator/=(const T & rhs) {
			*this = *this / uint256_t(rhs);
			return *this;
		}

		template <typename T> uint256_t operator%(const T & rhs) const {
			return *this % uint256_t(rhs);
		}

		template <typename T> uint256_t operator%=(const T & rhs) {
			*this = *this % uint256_t(rhs);
			return *this;
		}

		// Increment Operators
		uint256_t operator++();
		uint256_t operator++(int);

		// Decrement Operators
		uint256_t operator--();
		uint256_t operator--(int);

		// Get private values
		const uint128_t & upper() const;
		const uint128_t & lower() const;

		// Get bitsize of value
		uint16_t bits() const;

		// Get string representation of value
		std::string str(uint8_t base = 10, const unsigned int & len = 0) const;
	};

	extern UAPI const uint256_t uint256_0;
	extern UAPI const uint256_t uint256_1;


	// Bitwise Operators
	template <typename T> T operator&(const T & lhs, const uint256_t & rhs) {
		return (T)(lhs & rhs.lower());
	}

	template <typename T> T operator|(const T & lhs, const uint256_t & rhs) {
		return (T)(lhs | rhs.lower());
	}

	template <typename T> T operator^(const T & lhs, const uint256_t & rhs) {
		return (T)(lhs ^ (T)rhs.lower());
	}

	template <typename T> T operator&=(const T & lhs, const uint256_t & rhs) {
		lhs &= (T)rhs.lower();
		return lhs;
	}

	template <typename T> T operator|=(const T & lhs, const uint256_t & rhs) {
		lhs |= (T)rhs.lower();
		return lhs;
	}

	template <typename T> T operator^=(const T & lhs, const uint256_t & rhs) {
		lhs ^= (T)rhs.lower();
		return lhs;
	}

	// Comparison Operators
	template <typename T> bool operator==(const T & lhs, const uint256_t & rhs) {
		return (!rhs.upper() && (uint128_t)lhs == rhs.lower());
	}

	template <typename T> bool operator!=(const T & lhs, const uint256_t & rhs) {
		return (rhs.upper() | ((uint128_t)lhs != rhs.lower()));
	}

	template <typename T> bool operator>(const T & lhs, const uint256_t & rhs) {
		return rhs.upper() ? false : ((uint128_t)lhs > rhs.lower());
	}

	template <typename T> bool operator<(const T & lhs, const uint256_t & rhs) {
		return rhs.upper() ? true : ((uint128_t)lhs < rhs.lower());
	}

	template <typename T> bool operator>=(const T & lhs, const uint256_t & rhs) {
		return rhs.upper() ? false : ((uint128_t)lhs >= rhs.lower());
	}

	template <typename T> bool operator<=(const T & lhs, const uint256_t & rhs) {

		return (rhs.upper()) ? true : ((uint128_t)lhs <= rhs.lower());
	}

	// Arithmetic Operators
	template <typename T> T operator+(const T & lhs, const uint256_t & rhs) {
		return (T)(rhs + lhs);
	}

	template <typename T> T & operator+=(const T & lhs, const uint256_t & rhs) {
		lhs = (T)(rhs + lhs);
		return lhs;
	}

	template <typename T> T operator-(const T & lhs, const uint256_t & rhs) {
		return (T)(uint256_t(lhs) - rhs);
	}

	template <typename T> T & operator-=(const T & lhs, const uint256_t & rhs) {
		lhs = (T)(uint256_t(lhs) - rhs);
		return lhs;
	}

	template <typename T> T operator*(const T & lhs, const uint256_t & rhs) {
		return lhs * rhs.lower();
	}

	template <typename T> T & operator*=(const T & lhs, const uint256_t & rhs) {
		lhs = (T)(lhs * rhs.lower());
		return lhs;
	}

	template <typename T> T operator/(const T & lhs, const uint256_t & rhs) {
		return (T)(uint256_t(lhs) / rhs);
	}

	template <typename T> T & operator/=(const T & lhs, const uint256_t & rhs) {
		lhs = (T)(uint256_t(lhs) / rhs);
		return lhs;
	}

	template <typename T> T operator%(const T & lhs, const uint256_t & rhs) {
		return (T)(uint256_t(lhs) % rhs);
	}

	template <typename T> T & operator%=(const T & lhs, const uint256_t & rhs) {
		lhs = (T)(uint256_t(lhs) % rhs);
		return lhs;
	}

	// IO Operator
	std::ostream & operator<<(std::ostream & stream, const uint256_t & rhs);


	typedef uint128_t uint128;
	typedef uint256_t uint256;
};