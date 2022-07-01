#pragma once

#include <array>
#include <list>
#include <sstream>
#include <string>
#include <cstddef>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include "my_type_traits.h"

static constexpr unsigned BitInByte = 8;

template <typename OctetType, std::size_t OctetCount, char Delimiter>
class BasicAddress
{
public:
	virtual ~BasicAddress() = default;

	class OctetProxy
	{
	public:
		OctetProxy() = delete;
		OctetProxy(BasicAddress& owner, OctetType& octet) : mOwner(owner), mOctet(octet) {}
		explicit operator OctetType()
		{
			return mOctet;
		}

		template<typename T>
		OctetProxy& operator=(T val)
		{
			static_assert(std::is_integral_v<T>, "is not a integral type");

			mOctet = mOwner.IntegralToOctet(val);
			return *this;
		}
		OctetProxy& operator=(const std::string& str)
		{
			mOctet = mOwner.ConvertStringToOctet(str);
			return *this;
		}
	private:
		BasicAddress& mOwner;
		OctetType& mOctet;
	};

	OctetProxy operator[](std::size_t numberOfOctet)
	{
		return OctetProxy(*this, mAddress[numberOfOctet]);
	}
	const OctetType& operator[](std::size_t numberOfOctet) const
	{
		return mAddress[numberOfOctet];
	}

	friend std::istream& operator>>(std::istream& is, BasicAddress& address)
	{
		std::string buffer;
		is >> buffer;
		address.SetAddress(buffer);

		return is;
	}
	friend std::ostream& operator<<(std::ostream& os, const  BasicAddress& address)
	{
		os << address.GetAddress();
		return os;
	}

	friend bool operator==(const BasicAddress& a1, const BasicAddress& a2)
	{
		return a1.mAddress == a2.mAddress;
	}
	friend bool operator!=(const BasicAddress& a1, const BasicAddress& a2)
	{
		return a1.mAddress != a2.mAddress;
	}
	friend bool operator<(const BasicAddress& a1, const BasicAddress& a2)
	{
		return a1.mAddress < a2.mAddress;
	}
	friend bool operator>(const BasicAddress& a1, const BasicAddress& a2)
	{
		return a1.mAddress > a2.mAddress;
	}
	friend bool operator<=(const BasicAddress& a1, const BasicAddress& a2)
	{
		return a1.mAddress <= a2.mAddress;
	}
	friend bool operator>=(const BasicAddress& a1, const BasicAddress& a2)
	{
		return a1.mAddress >= a2.mAddress;
	}

	void SetAddress(const std::string& source)
	{
		std::vector<std::string> tokens;

		boost::split(tokens, source, boost::is_any_of(std::string{ Delimiter }));

		if (tokens.size() != OctetCount)
		{
			throw std::runtime_error("The count of octets is incorrect");
		}

		auto fromString = [this](const std::string& val)
		{
			return this->ConvertStringToOctet(val);
		};
		boost::copy(tokens | boost::adaptors::transformed(fromString), mAddress.begin());

	}
	template<typename T>
	std::enable_if_t<is_container_v<T>, void> SetAddress(const T& source)
	{
		static_assert(std::is_integral_v<typename T::value_type>, "is not a integral type");

		if (source.size() != OctetCount)
		{
			throw std::runtime_error("The count of octets is incorrect");
		}
		
		auto fromIntegral = [this](const typename T::value_type& val)
		{
			return this->IntegralToOctet(val);
		};

		boost::copy(source | boost::adaptors::transformed(*IntegralToOctet()), mAddress.begin());
	}
	template<typename T>
	std::enable_if_t<std::is_integral_v<T>, void> SetAddress(const T& source)
	{
		std::list<OctetType>octets;
		constexpr OctetType bitmask = std::numeric_limits<OctetType>::max();
		constexpr size_t shift = sizeof(OctetType) * BitInByte;

		auto tmp = source;
		for (size_t i = 0; i < OctetCount; ++i)
		{ 
			octets.push_front(tmp & bitmask);
			tmp >>= shift;
		}

		boost::copy(octets, mAddress.begin());
	}

	std::string GetAddress() const
	{
		std::stringstream buffer;

		auto addressIter = mAddress.begin();
		for (; addressIter != mAddress.end()-1; ++addressIter)
		{
			buffer << this->ConvertOctetToString(*addressIter);
			buffer << Delimiter;
		}
		buffer << this->ConvertOctetToString(*addressIter);

		return buffer.str();
	}
protected:

	virtual OctetType ConvertStringToOctet(const std::string& str) const = 0;

	template<typename T>
	OctetType IntegralToOctet(const T& val) const
	{
		try
		{
			return boost::numeric_cast<OctetType>(val);
		}
		catch (boost::bad_numeric_cast&)
		{
			throw std::runtime_error("The octet value are incorrect");
		}
	}

	virtual std::string ConvertOctetToString(const OctetType& octet) const = 0;

	std::array<OctetType, OctetCount> mAddress{};
};

