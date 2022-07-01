#pragma once

#include <array>
#include <bitset>
#include <cctype>
#include <sstream>
#include <string>
#include <cstddef>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/utility/string_view.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include "my_type_traits.h"

static constexpr unsigned BitsInByte = 8;

template <typename Octet, std::size_t OctetsCount,char Delimiter>
class BasicAddress
{
public:
	virtual ~BasicAddress() = default;

	class OctetProxy
	{
	public:
		OctetProxy() = delete;
		OctetProxy(BasicAddress& owner, size_t position) : mOwner(owner), mPos(position) {}

		OctetProxy(const OctetProxy& other) : mOwner(other.mOwner),mPos(other.mPos){}
		OctetProxy& operator=(const OctetProxy& other)
		{
			mOwner = other.mOwner;
			mPos = other.mPos;

			return *this;
		}

		template<typename T>
		OctetProxy& operator=(T val)
		{
			mOwner.mAddress[mPos] = mOwner.IntegralToOctet(val);
			return *this;
		}
		OctetProxy& operator=(const std::string& str)
		{
			mOwner.mAddress[mPos] = mOwner.StringToOctet(str);
			return *this;
		}

		operator Octet()
		{
			return mOwner.mAddress[mPos];
		}
	private:
		BasicAddress& mOwner;
		size_t mPos;			
	};

	OctetProxy operator[](std::size_t position)
	{
		return OctetProxy(*this, position);
	}
	const Octet& operator[](std::size_t position) const
	{
		return mAddress[position];
	}

	friend std::istream& operator>>(std::istream& istrm, BasicAddress& address)
	{
		std::string buffer;
		istrm >> buffer;
		address.SetAddress(buffer);

		return istrm;
	}
	friend std::ostream& operator<<(std::ostream& ostrm, const  BasicAddress& address)
	{
		ostrm << address.GetAddress();
		return ostrm;
	}

	friend bool operator==(const BasicAddress& lhs, const BasicAddress& rhs)
	{
		return lhs.mAddress == rhs.mAddress;
	}
	friend bool operator!=(const BasicAddress& lhs, const BasicAddress& rhs)
	{
		return lhs.mAddress != rhs.mAddress;
	}
	friend bool operator<(const BasicAddress& lhs, const BasicAddress& rhs)
	{
		return lhs.mAddress < rhs.mAddress;
	}
	friend bool operator>(const BasicAddress& lhs, const BasicAddress& rhs)
	{
		return lhs.mAddress > rhs.mAddress;
	}
	friend bool operator<=(const BasicAddress& lhs, const BasicAddress& rhs)
	{
		return lhs.mAddress <= rhs.mAddress;
	}
	friend bool operator>=(const BasicAddress& lhs, const BasicAddress& rhs)
	{
		return lhs.mAddress >= rhs.mAddress;
	}

	void SetAddress(boost::string_view source) noexcept(false)
	{
		static const std::string delim{ Delimiter };

		std::vector<std::string> tokens;

		boost::split(tokens, source, boost::is_any_of(delim));

		if (tokens.size() != OctetsCount)
		{
			throw std::runtime_error("Invalid number of octets");
		}

		boost::transform(tokens, mAddress.begin(),
			[this](boost::string_view val) {return StringToOctet(val); });

	}
	template<typename T>
	std::enable_if_t<is_container_v<T>, void> SetAddress(const T& source) noexcept(false)
	{
		if (source.size() != OctetsCount)
		{
			throw std::runtime_error("The count of octets is incorrect");
		}

		boost::transform(source, mAddress.begin(), 
			[this](const auto& val) {return IntegralToOctet(val); });
	}
	template<typename T>
	std::enable_if_t<std::is_integral_v<T>, void> SetAddress(T source)
	{
		static constexpr size_t shift = sizeof(Octet) * BitsInByte;
		//TODO: replace numeric limits max with another construct
		constexpr T bitmask = std::numeric_limits<Octet>::max();

		for (Octet& octet : boost::adaptors::reverse(mAddress))
		{
			octet = static_cast<Octet>(source & bitmask);
			source >>= shift;
		}
	}

	std::string GetAddress() const
	{
		std::stringstream buffer;

		auto addressIter = mAddress.begin();
		while (addressIter != mAddress.end()-1)
		{
			buffer << this->OctetToString(*addressIter);
			buffer << Delimiter;

			++addressIter;
		}
		buffer << this->OctetToString(*addressIter);

		return buffer.str();
	}
protected:
	virtual std::string OctetToString(const Octet& octet) const = 0;
	virtual Octet StringToOctet(const boost::string_view& str) const = 0;
	template<typename T>
	std::enable_if_t<std::is_integral_v<T>, Octet> IntegralToOctet(const T& val) const
	{
		try
		{
			return boost::numeric_cast<Octet>(val);
		}
		catch (boost::bad_numeric_cast&)
		{
			throw std::runtime_error("The octet value are incorrect");
		}
	}

	std::array<Octet, OctetsCount> mAddress{};
};

