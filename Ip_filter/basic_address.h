#pragma once

#include <array>
#include <vector>
#include <string>
#include <sstream>
#include <cstddef>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/utility/string_view.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include "my_type_traits.h"
#include <boost/operators.hpp>


static constexpr unsigned BitsInByte = 8;

template <typename Implementation>
struct address_traits;

template <typename Implementation>
class BasicAddress : boost::totally_ordered<BasicAddress<Implementation>>
{
public:
	using Octet = typename address_traits<Implementation>::Octet;
	static constexpr size_t OctetsCount= address_traits<Implementation>::OctetsCount;
	static constexpr char Delimiter = address_traits<Implementation>::Delimiter;

	virtual ~BasicAddress() = default;

	class OctetProxy
	{
	public:
		OctetProxy() = delete;
		OctetProxy(BasicAddress& owner, size_t position) : mOwner(owner), mPos(position) {}

		OctetProxy(const OctetProxy& other) : mOwner(other.mOwner),mPos(other.mPos){}
		OctetProxy& operator=(const OctetProxy& other)
		{
			if(other != this)
			{
				mOwner = other.mOwner;
				mPos = other.mPos;
			}

			return *this;
		}

		template<typename T>
		std::enable_if_t<std::is_literal_type_v<T>, OctetProxy&> operator=(T val)
		{
			mOwner.mAddress[mPos] = boost::numeric_cast<Octet>(val);
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
	friend bool operator<(const BasicAddress& lhs, const BasicAddress& rhs)
	{
		return lhs.mAddress < rhs.mAddress;
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
			[this](boost::string_view val) {return address_traits<Implementation>::StringToOctet(val); });

	}
	template<typename T>
	std::enable_if_t<is_container_v<T>, void> SetAddress(const T& source) noexcept(false)
	{
		if (source.size() != mAddress.size())
		{
			throw std::runtime_error("The count of octets is incorrect");
		}

		boost::transform(source, mAddress.begin(), 
			[this](const auto& val) {return boost::numeric_cast<Octet>(val); });
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
			buffer << address_traits<Implementation>::OctetToString(*addressIter);
			buffer << Delimiter;

			++addressIter;
		}
		buffer << address_traits<Implementation>::OctetToString(*addressIter);

		return buffer.str();
	}
protected:
	std::array<Octet, OctetsCount> mAddress{};
};

	